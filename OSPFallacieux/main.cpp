#include <iostream>
#include <string>
#include <vector>
#include <algorithm> 
#include <cstring>
#include <unordered_map>

#include "reseau.h"
#include "router.h"
#include "topology.h"
#include "log.h"

using namespace std;


Topology t = Topology("/etc/ospfallacieux/config");

void test2(){
    Router r4 = Router("/etc/ospfallacieux/config");   
    cout << r4 << endl;
    Topology topo;
    topo.add(r4);
}

void test3(){
    Topology topo, topo2;
    topo.init_test();
    topo2.init_test_desactivate();
    cout << "topo avant : \n" << topo << endl;
    topo.add(topo2.getTopology());
    cout << "topo apres : \n" << topo << endl;
}

void test5(){
    Router r4 = Router("/etc/ospfallacieux/config");
    Topology topo2;
    topo2.add(r4);

    Topology topo;
    topo.init_test();
    cout << "topo avant : \n" << topo << endl;

    std::vector<uint8_t> res, rout;
    
    topo.normalize(rout, res);
    topo2.from_serialized(rout, res);
    cout << "topo apres : \n" << topo2 << endl;
}


void printDistances(const std::map<std::string, float>& distances) {
    for (const auto& entry : distances) {
        std::cout << "Routeur: " << entry.first << ", Distance: ";
        if (entry.second == std::numeric_limits<float>::infinity()) {
            std::cout << "infini";
        } else {
            std::cout << entry.second;
        }
        std::cout << std::endl;
    }
}



void test8(){
    Topology topo;
    topo.init_test();

    cout << topo << endl;

    std::unordered_map<std::string, std::vector<std::string>> paths = topo.dijkstra();
    string gateway; 
    vector<std::string> networks;
    topo.setup_for_routing(topo.getTopology()[2], paths, gateway, networks);
    cout << "==============================" << endl;
    for (const auto& [target, path] : paths) {
        for (const auto& node : path) {
            std::cout << node;
            if (&node != &path.back()) std::cout << " -> ";
        }
        std::cout << std::endl;
    }
    cout << "==============================" << endl;
    cout << "GW > " << gateway << endl;
    cout << " Reseau : " << endl;
    for(auto i : networks)
        cout << i << endl;
}




int main(){
    test8();
    return 0;
}