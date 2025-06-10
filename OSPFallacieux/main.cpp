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
    Router r4 = Router("R4","/etc/ospfallacieux/config");   
    cout << r4 << endl;
    Topology topo;
    topo.add(r4);
}

void test3(){
    Topology topo;
    topo.init_test();
    cout << "topo avant : \n" << topo << endl;
    topo.normalize();
    topo.from_serialized(topo.getRoutersSerialized(), topo.getReseauxSerialized());
    cout << "topo apres : \n" << topo.size() << endl;
}

void test5(){
    Router r4 = Router("R4","/etc/ospfallacieux/config");
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

int main(){
    test5();
    return 0;
}