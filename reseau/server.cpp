#include <iostream>
#include <fstream>
#include <cstring>
#include <unistd.h>
#include <netinet/in.h>
#include <chrono>
#include <thread>
#include <arpa/inet.h>
#include <ctime>

std::string getCurrentTimestamp() {
    std::time_t now = std::time(nullptr);
    char buf[100];
    std::strftime(buf, sizeof(buf), "%Y-%m-%d %H:%M:%S", std::localtime(&now));
    return std::string(buf);
}

void logMessage(const std::string &message, const std::string logFile = "/home/etudiant/helloOSPF/log/server_log.txt") {
    std::ofstream logfile(logFile, std::ios_base::app);
    if (logfile.is_open()) {
        logfile << "[" << getCurrentTimestamp() << "] " << message << std::endl;
    }
}

int main() {
    auto lastReset = std::chrono::steady_clock::now();
    while (true) {
        int server_fd = socket(AF_INET, SOCK_DGRAM, 0);
        if (server_fd < 0) {
            logMessage("socket failed: " + std::string(strerror(errno)));
            continue;
        }

        struct sockaddr_in serv_addr, client_addr;
        memset(&serv_addr, 0, sizeof(serv_addr));
        serv_addr.sin_family = AF_INET;
        serv_addr.sin_addr.s_addr = INADDR_ANY;
        serv_addr.sin_port = htons(8080);

        if (bind(server_fd, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) < 0) {
            logMessage("bind failed: " + std::string(strerror(errno)));
            close(server_fd);
            continue;
        }

        logMessage("[SERVER] En attente de messages UDP sur le port 8080...");

        socklen_t addrlen = sizeof(client_addr);
        char buffer[1024] = {0};

        while (true) {
            memset(buffer, 0, sizeof(buffer));
            int recv_len = recvfrom(server_fd, buffer, sizeof(buffer), 0,
                                    (struct sockaddr *)&client_addr, &addrlen);
            if (recv_len < 0) {
                logMessage("recvfrom failed: " + std::string(strerror(errno)));
                break;
            }

            std::string receivedMsg = "[SERVER] Message reçu de " +
                std::string(inet_ntoa(client_addr.sin_addr)) + " : " + buffer;
            logMessage(receivedMsg);

            std::this_thread::sleep_for(std::chrono::seconds(1));

            const char *response = "REPONSE SERVEUR";
            ssize_t sent = sendto(server_fd, response, strlen(response), 0,
                                  (struct sockaddr *)&client_addr, addrlen);
            if (sent < 0) {
                logMessage("[SERVER] Erreur envoi réponse: " + std::string(strerror(errno)));
            } else {
                logMessage("[SERVER] Réponse envoyée à " + std::string(inet_ntoa(client_addr.sin_addr)));
            }

            // Reset du fichier log toutes les 60 secondes
            auto now = std::chrono::steady_clock::now();
            if (std::chrono::duration_cast<std::chrono::seconds>(now - lastReset).count() >= 60) {
                std::ofstream resetLog("/home/etudiant/helloOSPF/log/server_log.txt", std::ios::trunc);
                if (resetLog.is_open()) {
                    resetLog << "[" << getCurrentTimestamp() << "] Log reset" << std::endl;
                    resetLog.close();
                }
                lastReset = now;
            }
        }
        close(server_fd);
    }

    return 0;
}
