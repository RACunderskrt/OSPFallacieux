// client.cpp
#include <iostream>
#include <cstring>
#include <unistd.h>
#include <arpa/inet.h>
#include <chrono>
#include <thread>

int main() {
    while(true){
        int sock;
        struct sockaddr_in server_addr;
        char buffer[1024] = {0};
        const char *message = "HELLO OSPF";

        // Création de la socket
        sock = socket(AF_INET, SOCK_DGRAM, 0);
        if (sock < 0) {
            perror("socket creation error");
        }

        // Activer l'option de broadcast
        int broadcastEnable = 1;
        if (setsockopt(sock, SOL_SOCKET, SO_BROADCAST, &broadcastEnable, sizeof(broadcastEnable)) < 0) {
            perror("Error setting broadcast option");
            close(sock);
        }

        // Définir l'adresse du serveur (broadcast)
        memset(&server_addr, 0, sizeof(server_addr));
        server_addr.sin_family = AF_INET;
        server_addr.sin_port = htons(8080);
        server_addr.sin_addr.s_addr = inet_addr("255.255.255.255");

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

    return 0;
}
