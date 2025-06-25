#include <iostream>
#include <string>
#include <cstring>
#include <vector>
#include <cstdint>
#include <stdexcept>
#include <algorithm>
#include <arpa/inet.h>
#include <tuple>
#include <unordered_map>
#include <set>
#include <limits>

class Topology{
    private:
        std::vector<Router> topology;
    
    public:
        Topology(){};
        Topology(std::vector<Router> topology_):topology(topology_){};
        Topology(std::string link){
            Router r1 = Router(link);
            add(r1);
        }

        std::vector<Router> getTopology(){
            return topology;
        }

        size_t size(){
            return topology.size();
        }

        void add(Router router){
            topology.push_back(router);
        };

        void add(std::vector<Router> routers) {
            try {
                for (auto& t : routers) {
                    if (t.getName() == topology[0].getName())
                        continue;

                    bool isIn = false;
                    for (int i = 1; i < topology.size(); i++) {
                        if (t.getName() == topology[i].getName()) {
                            isIn = true;
                            topology[i] = t;
                            break;
                        }
                    }
                    if (!isIn)
                        topology.push_back(t);
                }
            } catch (const std::out_of_range& e) {
                std::cerr << "Il faut au minimum 1 router dans la topology : " << e.what() << std::endl;
            } catch (const std::exception& e) {
                std::cerr << "Exception capturée: " << e.what() << std::endl;
            }
        }

        void from_serialized(std::vector<uint8_t> routers, std::vector<uint8_t> reseaux){ //transforme les 2 vector<uint8_t> en une topology
            std::vector<Reseau> reseaux_deserialized = Reseau::from_binary_to_reseaux(reseaux.data(), reseaux.size());
            std::vector<std::pair<Router,std::vector<int>>> routers_deserialized = Router::from_binary_to_routers(routers.data(), routers.size());

            std::vector<Router> bufRouters;
            for(int i = 0; i < routers_deserialized.size(); i++){  
                Router newRouter = std::get<0>(routers_deserialized[i]);
                for(auto res_id : std::get<1>(routers_deserialized[i])){
                    newRouter.addNeighbor(Router(),reseaux_deserialized[res_id],"");
                }

                bufRouters.push_back(newRouter);
            }
            add(bufRouters);
        }

        void init_test(){ //créer une topology de base pour les tests
            Router r1 = Router();
            r1.setName("R1");
            Router r2 = Router();
            r2.setName("R2");
            Router r3 = Router();
            r3.setName("R3");
            Router r4 = Router();
            r4.setName("R4");
            Reseau l1 = Reseau("L1","0.0.0.0",15);
            Reseau l2 = Reseau("L2","1.1.1.1",20);
            Reseau l3 = Reseau("L3","2.2.2.2",35);
            r1.addNeighbor(r2,l1,"0.0.0.1");
            r2.addNeighbor(r1,l1,"0.0.0.2");
            r2.addNeighbor(r3,l2,"1.1.1.3");
            r3.addNeighbor(r2,l2,"1.1.1.2");
            r3.addNeighbor(Router(),l3,"2.2.2.5");
            topology = {r1, r2, r3};
        };

        void init_test_desactivate(){ //créer une topology de base pour les tests
            Router r1 = Router();
            r1.setName("R1");
            r1.desactivate();
            Router r2 = Router();
            r2.setName("R2");
            r2.desactivate();
            Router r3 = Router();
            r3.setName("R3");
            r3.desactivate();
            Router r4 = Router();
            r4.setName("R4");
            Reseau l1 = Reseau("L1","0.0.0.0",15);
            Reseau l2 = Reseau("L2","1.1.1.1",20);
            Reseau l3 = Reseau("L3","2.2.2.2",35);
            r1.addNeighbor(r2,l1,"0.0.0.1");
            r2.addNeighbor(r1,l1,"0.0.0.2");
            r1.addNeighbor(r4,l3,"2.2.2.4");
            r2.addNeighbor(r3,l2,"1.1.1.3");
            r3.addNeighbor(r2,l2,"1.1.1.2");
            r3.addNeighbor(Router(),l3,"2.2.2.5");
            topology = {r1, r2, r3, r4};
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

            std::vector<std::pair<Router,std::vector<int>>> routers;

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
                routers.push_back(std::make_pair(router, reseau_ids));
            }

            routers_serialized = Router::routers_to_binary(routers); //on le serialise
        };


        static std::string find_last_router(std::string nameRouter, std::unordered_map<std::string, std::vector<std::string>>& predecessorMap){
            if (predecessorMap.find(nameRouter) != predecessorMap.end()) {
                std::vector<std::string> chemin = predecessorMap[nameRouter];
                return chemin[chemin.size() - 2];
            } else {
                std::cout << "Router n'existe pas dans le chemin." << std::endl;
            }
            return ""; 
        };

        std::string get_commun_network(const std::string& targetNodeA, std::unordered_map<std::string, std::vector<std::string>>& predecessorMap){
            const std::string targetNodeB = find_last_router(targetNodeA, predecessorMap);
            std::vector<Router> buffRouter;
            for(auto r : topology){
                if(r.getName() == targetNodeA || r.getName() == targetNodeB)
                    buffRouter.push_back(r);
            }

            if(buffRouter.size() != 2) throw std::logic_error("targetNodeA ou targetNodeB n'existe pas dans la topology");

            for(auto reseauA : buffRouter[0].getReseaux()){
                for(auto reseauB : buffRouter[1].getReseaux()){
                    if(reseauA.getAddr() == reseauB.getAddr())
                        return reseauA.getAddr();
                }
            }
            return "";
        }

        std::string find_first_interface(std::string nameRouter, std::unordered_map<std::string, std::vector<std::string>>& predecessorMap){
            if (predecessorMap.find(nameRouter) != predecessorMap.end()) {
                std::vector<std::string> chemin = predecessorMap[nameRouter];
                return topology[0].findInterface(chemin[1]);
            } else {
                std::cout << "Clé non trouvée." << std::endl;
            }
            return ""; 
        };

        int count_network_occurence(const std::string addr){
            int res = 0;
            for(auto router: topology){
                for(auto reseau : router.getReseaux())
                    res += reseau.getAddr() == addr;
            }
            return res;
        }

        void setup_for_routing(Router actualRouter, std::unordered_map<std::string, std::vector<std::string>>& predecessorMap, std::string& gateway, std::vector<std::string>& networks){
            if(!actualRouter.isActive()) return;
            
            gateway = find_first_interface(actualRouter.getName(), predecessorMap);
            
            networks.push_back(get_commun_network(actualRouter.getName(), predecessorMap));
            
            for(auto r : actualRouter.getReseaux()){
                if(count_network_occurence(r.getAddr()) == 1)
                    networks.push_back(r.getAddr());
            }
        }

        std::unordered_map<std::string, std::vector<std::string>> dijkstra(){
            return topology[0].dijkstra_all_paths(topology);
        };


        friend std::ostream& operator<<(std::ostream& os, const Topology& r); //surcharge de l'opérateur <<  pour print l'objet
};

std::ostream& operator<<(std::ostream& os, const Topology& r) {
    for(auto i : r.topology)
        std::cout << i << std::endl;
    return os;
}