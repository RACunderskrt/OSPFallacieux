#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>
#include <cstring>
#include <unistd.h>
#include <arpa/inet.h>
#include <chrono>
#include <thread>
#include <map>
#include <ctime>

std::string getCurrentTimestamp() {
    std::time_t now = std::time(nullptr);
    char buf[100];
    std::strftime(buf, sizeof(buf), "%Y-%m-%d %H:%M:%S", std::localtime(&now));
    return std::string(buf);
}

void logMessage(const std::string &message, const std::string logFile = "/home/etudiant/helloOSPF/log/client_log.txt") {
    std::ofstream logfile(logFile, std::ios_base::app);
    if (logfile.is_open()) {
        logfile << "[" << getCurrentTimestamp() << "] " << message << std::endl;
    }
}

std::vector<std::string> getBroadcastAddresses(const std::string &filename) {
    std::ifstream infile(filename);
    std::string line;
    std::vector<std::string> broadcastAddresses;

    while (std::getline(infile, line)) {
        std::istringstream iss(line);
        std::string key, ip;
        if (std::getline(iss, key, ':') && std::getline(iss, ip)) {
            struct in_addr addr;
            if (inet_aton(ip.c_str(), &addr)) {
                unsigned long net = ntohl(addr.s_addr);
                net |= 0x000000FF; // broadcast /24
                addr.s_addr = htonl(net);
                broadcastAddresses.push_back(inet_ntoa(addr));
            }
        }
    }

    return broadcastAddresses;
}

int main() {
    auto lastReset = std::chrono::steady_clock::now();
    const char *message = "HELLO OSPF";
    std::vector<std::string> broadcastAddresses = getBroadcastAddresses("/home/etudiant/helloOSPF/config");

    int sock = socket(AF_INET, SOCK_DGRAM, 0);
    if (sock < 0) {
        logMessage("socket creation error: " + std::string(strerror(errno)));
        return 1;
    }

    int broadcastEnable = 1;
    if (setsockopt(sock, SOL_SOCKET, SO_BROADCAST, &broadcastEnable, sizeof(broadcastEnable)) < 0) {
        logMessage("Error setting broadcast option: " + std::string(strerror(errno)));
        close(sock);
        return 1;
    }

    // Bind explicite avec port fixe pour recevoir correctement
    struct sockaddr_in client_addr;
    memset(&client_addr, 0, sizeof(client_addr));
    client_addr.sin_family = AF_INET;
    client_addr.sin_addr.s_addr = INADDR_ANY;
    client_addr.sin_port = htons(55555); // Port fixe
    if (bind(sock, (struct sockaddr *)&client_addr, sizeof(client_addr)) < 0) {
        logMessage("bind failed: " + std::string(strerror(errno)));
        close(sock);
        return 1;
    }

    while (true) {
        for (const std::string &broadcastIP : broadcastAddresses) {
            struct sockaddr_in server_addr;
            memset(&server_addr, 0, sizeof(server_addr));
            server_addr.sin_family = AF_INET;
            server_addr.sin_port = htons(8080);
            server_addr.sin_addr.s_addr = inet_addr(broadcastIP.c_str());

            ssize_t sentBytes = sendto(sock, message, strlen(message), 0,
                                       (struct sockaddr *)&server_addr, sizeof(server_addr));
            if (sentBytes < 0) {
                logMessage("sendto failed: " + std::string(strerror(errno)));
            } else {
                logMessage("[CLIENT] Broadcast envoyé à " + broadcastIP + " : " + message);
            }

            // Réception de réponse
            char buffer[1024] = {0};
            struct sockaddr_in from_addr;
            socklen_t addrlen = sizeof(from_addr);
            int recv_len = recvfrom(sock, buffer, sizeof(buffer), MSG_DONTWAIT,
                                    (struct sockaddr *)&from_addr, &addrlen);
            if (recv_len > 0) {
                std::string reply = "[CLIENT] Réponse reçue de " + std::string(inet_ntoa(from_addr.sin_addr)) + " : " + buffer;
                logMessage(reply);
            }
        }

        // Reset du fichier log toutes les 60 secondes
        auto now = std::chrono::steady_clock::now();
        if (std::chrono::duration_cast<std::chrono::seconds>(now - lastReset).count() >= 60) {
            std::ofstream resetLog("/home/etudiant/helloOSPF/log/client_log.txt", std::ios::trunc);
            if (resetLog.is_open()) {
                resetLog << "[" << getCurrentTimestamp() << "] Log reset" << std::endl;
                resetLog.close();
            }
            lastReset = now;
        }
        std::this_thread::sleep_for(std::chrono::seconds(2));
    }

    close(sock);
    return 0;
}
