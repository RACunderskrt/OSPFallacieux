// server.cpp
#include <iostream>
#include <cstring>
#include <unistd.h>
#include <netinet/in.h>

int main() {
    //ECOUTE EN PERMANENCE
    while(true){
 int server_fd;
    struct sockaddr_in serv_addr, client_addr;
    socklen_t addrlen = sizeof(client_addr);
    char buffer[1024] = {0};

    // Création de la socket
    server_fd = socket(AF_INET, SOCK_DGRAM, 0);
    if (server_fd < 0) {
        perror("socket failed");
    }

    // Initialiser l'adresse du serveur
    memset(&serv_addr, 0, sizeof(serv_addr));
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_addr.s_addr = INADDR_ANY;
    serv_addr.sin_port = htons(8080);

    if (bind(server_fd, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) < 0) {
        perror("bind failed");
        close(server_fd);
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

    return 0;
}
