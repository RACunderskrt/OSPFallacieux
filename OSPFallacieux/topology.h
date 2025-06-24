#include <iostream>
#include <string>
#include <cstring>
#include <vector>
#include <cstdint>
#include <stdexcept>
#include <algorithm>
#include <arpa/inet.h> 

class Topology{
    private:
        std::vector<Router> topology;
    
    public:
        Topology(){};
        Topology(std::vector<Router> topology_):topology(topology_){};

        std::vector<Router> getTopology(){
            return topology;
        }

        size_t size(){
            return topology.size();
        }

        void add(Router router){
            topology.push_back(router);
        };

        void add(std::vector<Router> routers){
            for(auto& t: topology){
                
                if(t.getName() == topology[0].getName())
                    continue;

                for(auto& r: routers){ //si le router existe deja dans la topology, il est mis a jour
                    if(r.getName() == t.getName())
                        t = r;
                    else
                        topology.push_back(r); //sinon, il est ajouté a la topology
                }
            }
        };

        void from_serialized(std::vector<uint8_t> routers, std::vector<uint8_t> reseaux){ //transforme les 2 vector<uint8_t> en une topology
            std::vector<Reseau> reseaux_deserialized = Reseau::from_binary_to_reseaux(reseaux.data(), reseaux.size());
            std::vector<std::pair<std::string,std::vector<int>>> routers_deserialized = Router::from_binary_to_routers(routers.data(), routers.size());

            for(int i = 0; i < routers_deserialized.size(); i++){
                bool isIn = false;
                for(int j = 0; j < topology.size(); j++){
                    if(std::get<0>(routers_deserialized[i]) == topology[j].getName()) 
                        isIn = true;
                }
                if(!isIn){
                    Router newRouter = Router("",std::get<0>(routers_deserialized[i]));
                    for(auto res_id : std::get<1>(routers_deserialized[i])){
                        newRouter.addNeighbor(Router(),reseaux_deserialized[res_id],"");
                    }

                    topology.push_back(newRouter);
                }
            }
        }

        void init_test(){ //créer une topology de base pour les tests
            Router r1 = Router();
            r1.setName("R1");
            Router r2 = Router();
            r2.setName("R2");
            Router r3 = Router();
            r3.setName("R3");
            Reseau l1 = Reseau("L1","0.0.0.0",15);
            Reseau l2 = Reseau("L2","1.1.1.1",20);

            r1.addNeighbor(r2,l1,"0.0.0.1");
            r2.addNeighbor(r1,l1,"0.0.0.2");
            r2.addNeighbor(r3,l2,"1.1.1.3");
            r3.addNeighbor(r2,l2,"1.1.1.2");
            topology = {r1, r2, r3};
        };

        void normalize(std::vector<uint8_t>& routers_serialized, std::vector<uint8_t>& reseaux_serialized) { //stocke la topology normalisé+serialisé dans 2 vector<uint8_t> passé en parametre
            std::vector<Reseau> reseaux;

            for(auto router : topology){ //on garde chaque reseau dans un vector
                for(auto voisin : router.getNeighbors()){
                    Reseau buffer = std::get<1>(voisin);
                    bool isInVector = false;
                    for(auto b : reseaux){
                        if(buffer == b)
                            isInVector = true;
                    }

                    if(!isInVector)
                        reseaux.push_back(buffer);
                }
            }

            reseaux_serialized = Reseau::reseaux_to_binary(reseaux); //on le serialise

            std::vector<std::pair<std::string,std::vector<int>>> routers;

            for (const auto& router : topology) { //on garde chaque router avec l'index des reseaus dans le vector precedent
                std::vector<int> reseau_ids;
                for (const auto& voisin : router.getNeighbors()) {
                    Reseau buffer = std::get<1>(voisin);
                    auto it = std::find(reseaux.begin(), reseaux.end(), buffer);
                    if (it != reseaux.end()) {
                        int index = it - reseaux.begin();
                        reseau_ids.push_back(index);
                    }
                }
                routers.push_back(std::make_pair(router.getName(), reseau_ids));
            }

            routers_serialized = Router::routers_to_binary(routers); //on le serialise
        };

        void dijkstra(std::map<std::string, std::string> &predecessorMap, std::map<std::string, float> &shortestPaths){
            shortestPaths = topology[0].calculateShortestPaths(topology, predecessorMap);
        }

        friend std::ostream& operator<<(std::ostream& os, const Topology& r); //surcharge de l'opérateur <<  pour print l'objet
};

std::ostream& operator<<(std::ostream& os, const Topology& r) {
    for(auto i : r.topology)
        std::cout << i << std::endl;
    return os;
}