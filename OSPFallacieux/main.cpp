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
}

void test3(){
    Topology topo;
    topo.init_test();
    cout << "topo : \n" << topo << endl;
    topo.normalize();
}

int main(){
    test3();
    return 0;
}