#include <iostream>
#include <string>
#include <vector>
#include <algorithm> 
#include <cstring>

#include "reseau.h"
#include "router.h"
#include "topology.h"
#include "log.h"

using namespace std;

void test2(){
    Router r4 = Router("/etc/ospfallacieux/config");   
    cout << r4 << endl;
    Topology topo;
    topo.add(r4);
}

void test3(){
    
}

void test5(){
    Router r4 = Router("/etc/ospfallacieux/config");
    Topology topo2;
    topo2.add(r4);

    Topology topo;
    topo.init_test();
    cout << "topo avant : \n" << topo << endl;
    topo2.normalize();
    topo.from_serialized(topo2.getRoutersSerialized(), topo2.getReseauxSerialized());
    cout << "==============================" << endl;
    cout << "topo apres : \n" << topo << endl;
}

void test6(){
    Topology topo;
    topo.init_test();
    cout << "topo avant : \n" << topo << endl;
    auto topo_oui = topo.getTopology();
    std::map<std::string, std::string> interfaceMap;
    std::map<std::string, float> shortestPaths = topo_oui[0].calculateShortestPaths_2(topo_oui, interfaceMap);
    // Afficher les distances
    std::cout << "Distances:" << std::endl;
    for (const auto& entry : shortestPaths) {
        std::cout << "Node: " << entry.first << " -> Distance: " << entry.second << std::endl;
    }

    // Afficher les interfaces
    std::cout << "\nInterfaces:" << std::endl;
    for (const auto& entry : interfaceMap) {
        std::cout << "Node: " << entry.first << " -> Interface: " << entry.second << std::endl;
    }
}

string printPath(const std::string& targetNode, const std::map<std::string, std::string>& predecessorMap) {
    std::vector<std::string> path;
    std::string current = targetNode;

    // Reconstituer le chemin en remontant les prédécesseurs
    while (predecessorMap.find(current) != predecessorMap.end()) {
        path.push_back(current);
        current = predecessorMap.at(current);
    }

    return path.back();
}

void test7(){
    Topology topo;
    topo.init_test();
    cout << "topo avant : \n" << topo << endl;
    auto topo_oui = topo.getTopology();
    std::map<std::string, std::string> predecessorMap;
    std::map<std::string, float> shortestPaths = topo_oui[0].calculateShortestPaths_3(topo_oui, predecessorMap);

    // Supposons que nous voulons afficher le chemin de "NodeA" à "NodeB"
    cout << printPath("R3", predecessorMap) << endl;
    cout << topo_oui[0].findInterface(printPath("R3", predecessorMap)) << endl;
    
}




int main(){
    test2();
    return 0;
}