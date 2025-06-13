#include <iostream>
#include <cstring>
#include <unistd.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <chrono>
#include <thread>


void server(){
    int server_fd;
    struct sockaddr_in serv_addr, client_addr;
    socklen_t addrlen = sizeof(client_addr);
    char buffer[1024] = {0};

    // Création de la socket
    server_fd = socket(AF_INET, SOCK_DGRAM, 0);
    if (server_fd < 0) {
        perror("socket failed");
        return;
    }

    // Initialiser l'adresse du serveur
    memset(&serv_addr, 0, sizeof(serv_addr));
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_addr.s_addr = INADDR_ANY;
    serv_addr.sin_port = htons(8080);

    if (bind(server_fd, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) < 0) {
        perror("bind failed");
        close(server_fd);
        return;
    }

    std::cout << "[SERVER] En attente de messages UDP sur le port 8080..." << std::endl;

    while(true){
        memset(buffer, 0, sizeof(buffer));

        // Réception du message
        int recv_len = recvfrom(server_fd, buffer, sizeof(buffer), 0,
                                (struct sockaddr *)&client_addr, &addrlen);
        if (recv_len < 0) {
            perror("recvfrom failed");
            break;
        }

        std::cout << "[SERVER] Message reçu : " << buffer << std::endl;

        // Répondre au client
        const char *response = "HELLO REÇU";
        sendto(server_fd, response, strlen(response), 0,
               (struct sockaddr *)&client_addr, addrlen);
    }

    close(server_fd);
}

void client(std::string addr){
    while(true){
        int sock;
        struct sockaddr_in server_addr;
        char buffer[1024] = {0};
        const char *message = "HELLO OSPF";

        // Création de la socket
        sock = socket(AF_INET, SOCK_DGRAM, 0);
        if (sock < 0) {
            perror("socket creation error");
            return;
        }

        // Activer l'option de broadcast
        int broadcastEnable = 1;
        if (setsockopt(sock, SOL_SOCKET, SO_BROADCAST, &broadcastEnable, sizeof(broadcastEnable)) < 0) {
            perror("Error setting broadcast option");
            close(sock);
            return;
        }

        // Définir l'adresse du serveur (broadcast)
        memset(&server_addr, 0, sizeof(server_addr));
        server_addr.sin_family = AF_INET;
        server_addr.sin_port = htons(8080);
        server_addr.sin_addr.s_addr = inet_addr(addr.c_str());

        // Envoyer le message
        ssize_t sentBytes = sendto(sock, message, strlen(message), 0,
                                   (struct sockaddr *)&server_addr, sizeof(server_addr));
        if (sentBytes < 0) {
            perror("sendto failed");
        } else {
            std::cout << "[CLIENT] Broadcast envoyé : " << message << std::endl;
        }

        // Recevoir la réponse
        socklen_t addrlen = sizeof(server_addr);
        int recv_len = recvfrom(sock, buffer, sizeof(buffer), 0,
                                (struct sockaddr *)&server_addr, &addrlen);
        if (recv_len < 0) {
            perror("recvfrom failed");
        } else {
            std::cout << "[CLIENT] Réponse reçue : " << buffer << std::endl;
        }

        // Attente
        std::this_thread::sleep_for(std::chrono::seconds(10));

        close(sock);
    }
}

int main(){
    //CREATION DES THREADS
    std::thread server_thread(server);
    std::thread client_thread(client);

    server_thread.join();
    client_thread.join();

    return 0;
}