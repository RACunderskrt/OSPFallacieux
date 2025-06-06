#include <iostream>
#include <string>
#include <vector>

#include "reseau.h"
#include "router.h"
#include "log.h"

using namespace std;

void test1(){
    Router r1 = Router();
    r1.setName("R1");
    Router r2 = Router();
    r2.setName("R2");
    Router r3 = Router();
    r3.setName("R3");
    Reseau l1 = Reseau("L1","0.0.0.0",15);
    Reseau l2 = Reseau("L2","1.1.1.1",20, false);

    r1.addNeighbor(r2,l1,"0.0.0.1");
    r2.addNeighbor(r1,l1,"0.0.0.2");
    r2.addNeighbor(r3,l2,"1.1.1.3");
    r3.addNeighbor(r2,l2,"1.1.1.2");
    l2.desactivate();
    cout << l2 << endl;
    cout << r1 << endl << r2 << endl << r3 << endl;

    std::vector<Router> tousLesRouteurs = {r1, r2, r3};
    std::map<std::string, float> chemins = r1.calculateShortestPaths(tousLesRouteurs);

    for (const auto& [nom, cout] : chemins) {
        std::cout << "Chemin vers " << nom << " : " << cout << std::endl;
    }
}

int main(){
    Router r4 = Router("R4","/etc/ospfallacieux/config");
    cout << r4 << endl;
    return 0;
}