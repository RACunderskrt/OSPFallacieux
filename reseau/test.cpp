#include <iostream>
#include <cstring>
#include <unistd.h>
#include <sys/ioctl.h>
#include <net/route.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <sys/socket.h>

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

int main() {
    std::string destination = "192.168.2.0";
    std::string gateway = "192.168.1.1";
    std::string netmask = "255.255.255.0";

    if (add_static_route(destination, gateway, netmask) == 0) {
        std::cout << "Route ajoutée avec succès !" << std::endl;
    } else {
        std::cerr << "Échec de l'ajout de la route." << std::endl;
    }

    return 0;
}