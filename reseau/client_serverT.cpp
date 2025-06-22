#include <iostream>
#include <cstring>
#include <string>
#include <sys/ioctl.h>
#include <net/if.h>
#include <net/route.h>
#include <sstream>
#include <fstream>
#include <vector>
#include <unistd.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <chrono>
#include <map>
#include <thread>
#include <ctime>

#include "OSPFallacieux/reseau.h"
#include "OSPFallacieux/router.h"
#include "OSPFallacieux/topology.h"
#include "OSPFallacieux/log.h"

std::string getLocalIPAddress(const std::string& interface = "eth0") {
    int fd = socket(AF_INET, SOCK_DGRAM, 0);
    if (fd < 0) return "";

    struct ifreq ifr;
    std::strncpy(ifr.ifr_name, interface.c_str(), IFNAMSIZ);
    if (ioctl(fd, SIOCGIFADDR, &ifr) == -1) {
        close(fd);
        return "";
    }

    close(fd);

    struct sockaddr_in* ipaddr = (struct sockaddr_in*)&ifr.ifr_addr;
    return std::string(inet_ntoa(ipaddr->sin_addr));
}

int add_static_route(const std::string& destination, const std::string& gateway, const std::string& netmask) {
    int sockfd = socket(AF_INET, SOCK_DGRAM, 0);
    if (sockfd < 0) {
        perror("socket");
        return -1;
    }

    struct rtentry route;
    memset(&route, 0, sizeof(route));

    struct sockaddr_in* addr;

    // Destination
    addr = (struct sockaddr_in*)&route.rt_dst;
    addr->sin_family = AF_INET;
    inet_pton(AF_INET, destination.c_str(), &addr->sin_addr);

    // Gateway
    addr = (struct sockaddr_in*)&route.rt_gateway;
    addr->sin_family = AF_INET;
    inet_pton(AF_INET, gateway.c_str(), &addr->sin_addr);

    // Netmask
    addr = (struct sockaddr_in*)&route.rt_genmask;
    addr->sin_family = AF_INET;
    inet_pton(AF_INET, netmask.c_str(), &addr->sin_addr);

    route.rt_flags = RTF_UP | RTF_GATEWAY;
    route.rt_metric = 1;

    if (ioctl(sockfd, SIOCADDRT, &route) < 0) {
        perror("ioctl - SIOCADDRT");
        close(sockfd);
        return -1;
    }

    close(sockfd);
    return 0;
}

std::string getCurrentTimestamp() {
    std::time_t now = std::time(nullptr);
    char buf[100];
    std::strftime(buf, sizeof(buf), "%Y-%m-%d %H:%M:%S", std::localtime(&now));
    return std::string(buf);
}

void logMessageServer(const std::string &message, const std::string logFile = "/home/etudiant/helloOSPF/log/server_logT.txt") {
    std::ofstream logfile(logFile, std::ios_base::app);
    if (logfile.is_open()) {
        logfile << "[" << getCurrentTimestamp() << "] " << message << std::endl;
    }
}

void logMessageClient(const std::string &message, const std::string logFile = "/home/etudiant/helloOSPF/log/client_logT.txt") {
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

void server(){
    auto lastReset = std::chrono::steady_clock::now();
    while (true) {
        int server_fd = socket(AF_INET, SOCK_DGRAM, 0);
        if (server_fd < 0) {
            logMessageServer("socket failed: " + std::string(strerror(errno)));
            continue;
        }

        struct sockaddr_in serv_addr, client_addr;
        memset(&serv_addr, 0, sizeof(serv_addr));
        serv_addr.sin_family = AF_INET;
        serv_addr.sin_addr.s_addr = INADDR_ANY;
        serv_addr.sin_port = htons(9090);

        if (bind(server_fd, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) < 0) {
            logMessageServer("bind failed: " + std::string(strerror(errno)));
            close(server_fd);
            continue;
        }

        logMessageServer("[SERVER] En attente de messages UDP sur le port 9090...");

        socklen_t addrlen = sizeof(client_addr);
        char buffer[1024] = {0};

        while (true) {
            memset(buffer, 0, sizeof(buffer));
            int recv_len = recvfrom(server_fd, buffer, sizeof(buffer), 0,
                                    (struct sockaddr *)&client_addr, &addrlen);
            if (recv_len < 0) {
                logMessageServer("recvfrom failed: " + std::string(strerror(errno)));
                break;
            }

            std::string receivedMsg = "[SERVER] Message reçu de " +
                std::string(inet_ntoa(client_addr.sin_addr)) + " : " + buffer;
            logMessageServer(receivedMsg);

            std::this_thread::sleep_for(std::chrono::seconds(1));

            const char *response = "REPONSE SERVEUR";
            ssize_t sent = sendto(server_fd, response, strlen(response), 0,
                                  (struct sockaddr *)&client_addr, addrlen);
            if (sent < 0) {
                logMessageServer("[SERVER] Erreur envoi réponse: " + std::string(strerror(errno)));
            } else {
                logMessageServer("[SERVER] Réponse envoyée à " + std::string(inet_ntoa(client_addr.sin_addr)));
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
}

void client(){
    auto lastReset = std::chrono::steady_clock::now();
    Router r = Router("/home/etudiant/helloOSPF/config");
    std::stringstream ss;
    ss << r;
    std::string dataToSend = ss.str();
    const char *message = "HELLO OSPF";
    std::vector<std::string> broadcastAddresses = getBroadcastAddresses("/home/etudiant/helloOSPF/config");

    int sock = socket(AF_INET, SOCK_DGRAM, 0);
    if (sock < 0) {
        logMessageClient("socket creation error: " + std::string(strerror(errno)));
    }

    int broadcastEnable = 1;
    if (setsockopt(sock, SOL_SOCKET, SO_BROADCAST, &broadcastEnable, sizeof(broadcastEnable)) < 0) {
        logMessageClient("Error setting broadcast option: " + std::string(strerror(errno)));
        close(sock);
    }

    // Bind explicite avec port fixe pour recevoir correctement
    struct sockaddr_in client_addr;
    memset(&client_addr, 0, sizeof(client_addr));
    client_addr.sin_family = AF_INET;
    client_addr.sin_addr.s_addr = INADDR_ANY;
    client_addr.sin_port = htons(55556); // Port fixe
    if (bind(sock, (struct sockaddr *)&client_addr, sizeof(client_addr)) < 0) {
        logMessageClient("bind failed: " + std::string(strerror(errno)));
        close(sock);
    }

    std::string myIP = getLocalIPAddress("enp0s8");

    while (true) {
        for (const std::string &broadcastIP : broadcastAddresses) {
            struct sockaddr_in server_addr;
            memset(&server_addr, 0, sizeof(server_addr));
            server_addr.sin_family = AF_INET;
            server_addr.sin_port = htons(9090);
            server_addr.sin_addr.s_addr = inet_addr(broadcastIP.c_str());

            ssize_t sentBytes = sendto(sock, dataToSend.c_str(), dataToSend.size(), 0,
                                       (struct sockaddr *)&server_addr, sizeof(server_addr));
            if (sentBytes < 0) {
                logMessageClient("sendto failed: " + std::string(strerror(errno)));
            } else {
                logMessageClient("[CLIENT] Broadcast envoyé à " + broadcastIP + " : " + dataToSend);
            }

            // Réception de réponse
            char buffer[1024] = {0};
            struct sockaddr_in from_addr;
            socklen_t addrlen = sizeof(from_addr);
            int recv_len = recvfrom(sock, buffer, sizeof(buffer), MSG_DONTWAIT,
                                    (struct sockaddr *)&from_addr, &addrlen);

            std::string fromIP = inet_ntoa(from_addr.sin_addr);

            if (fromIP == myIP) {
                logMessageClient("[CLIENT] Réponse ignorée (venant de soi-même)");
                continue;
            }
            
            if (recv_len > 0) {
                std::string reply = "[CLIENT] Réponse reçue de " + std::string(inet_ntoa(from_addr.sin_addr)) + " : " + buffer;
                logMessageClient(reply);
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
        std::this_thread::sleep_for(std::chrono::seconds(10));
    }

    close(sock);
}

int main(){
    //CREATION DES THREADS
    std::thread server_thread(server);
    std::thread client_thread(client);

    server_thread.join();
    client_thread.join();

    return 0;
}