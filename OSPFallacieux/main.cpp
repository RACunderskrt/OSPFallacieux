#include <iostream>
#include <string>
#include <vector>
#include <algorithm> 

#include "reseau.h"
#include "router.h"
#include "log.h"

using namespace std;

// Ces fonctions sont à adapter à ton vrai code !

void normaliserRéseauxEtRouteurs(const std::vector<Router>& tousLesRouteurs) {
    vector<Reseau> reseau_unique;

    for(auto router : tousLesRouteurs){
        for(auto voisin : router.getNeighbors()){
            Reseau buffer = std::get<1>(voisin);
            bool isInVector = false;
            for(auto b : reseau_unique){
                if(buffer == b)
                    isInVector = true;
            }

            if(!isInVector)
                reseau_unique.push_back(buffer);
        }
    }

    for(auto b : reseau_unique){
        cout << b << endl;
    }

    std::vector<std::pair<std::string,vector<int>>> routers;

    for(auto router : tousLesRouteurs){
        vector<int> reseaux;
        for(auto voisin : router.getNeighbors()){
            Reseau buffer = std::get<1>(voisin);
            auto it = std::find(reseau_unique.begin(), reseau_unique.end(), buffer);
            if (it != reseau_unique.end())
                reseaux.push_back(it - reseau_unique.begin());   
        }
    
        routers.push_back(std::make_pair(router.getName(),reseaux));
    }

    for (const auto& router : routers) {
        std::cout << "Router: " << router.first << " -> ";
        for (const int& value : router.second) {
            std::cout << value << " ";
        }
        std::cout << std::endl;
    }

}

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
    //cout << l2 << endl;
    cout << r1 << endl << r2 << endl << r3 << endl;
    cout << endl << endl << "=============================="<< endl << endl;

    std::vector<Router> tousLesRouteurs = {r1, r2, r3};
    /*std::map<std::string, float> chemins = r1.calculateShortestPaths(tousLesRouteurs);

    for (const auto& [nom, cout] : chemins) {
        std::cout << "Chemin vers " << nom << " : " << cout << std::endl;
    }*/

    normaliserRéseauxEtRouteurs(tousLesRouteurs);
}

void test2(){
    Router r4 = Router("R4","/etc/ospfallacieux/config");   
    cout << r4 << endl;
}

int main(){
    test1();
    return 0;
}