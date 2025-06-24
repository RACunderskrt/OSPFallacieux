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

    std::vector<uint8_t> res, rout;
    topo.normalize(rout, res);
    topo.from_serialized(rout, res);
    cout << "==============================" << endl;
    cout << "topo apres : \n" << topo << endl;
}




void test7(){
    Topology topo;
    topo.init_test();
    cout << "topo avant : \n" << topo << endl;
    auto topo_oui = topo.getTopology();
    std::map<std::string, std::string> predecessorMap;
    std::map<std::string, float> shortestPaths = topo_oui[0].calculateShortestPaths(topo_oui, predecessorMap);

    //cout << topo.find_interface("R3", predecessorMap) << endl; 
    //cout << topo.get_commun_network("R3", predecessorMap) << endl;
    //cout << topo.count_network_occurence("1.1.1.1") << endl;
    //adresse gw, vector<std::string>
    string gateway; 
    vector<std::string> networks;
    topo.setup_for_routing("R2", predecessorMap, gateway, networks);

    cout << "GW > " << gateway << endl;
    cout << " Reseau : " << endl;
    for(auto i : networks)
        cout << i << endl;
}




int main(){
    test7();
    return 0;
}