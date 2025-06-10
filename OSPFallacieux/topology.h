#include <iostream>
#include <string>
#include <cstring>
#include <vector>
#include <cstdint>
#include <stdexcept>
#include <arpa/inet.h> 

class Topology{
    private:
        std::vector<Router> topology;
        std::vector<uint8_t> routers_serialized;
        std::vector<uint8_t> reseaux_serialized;
    
    public:
        Topology(){};
        Topology(std::vector<Router> topology_):topology(topology_){};

        void init_test(){
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
            topology = {r1, r2, r3};
        };

        void normalize() {
            std::vector<Reseau> reseaux;

            for(auto router : topology){
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

            for(auto b : reseaux){
                std::cout << b << std::endl;
            }

            reseaux_serialized = Reseau::reseaux_to_binary(reseaux);

            std::vector<std::pair<std::string,std::vector<int>>> routers;

            for(auto router : topology){
                std::vector<int> reseaux;
                for(auto voisin : router.getNeighbors()){
                    Reseau buffer = std::get<1>(voisin);
                    auto it = std::find(reseaux.begin(), reseaux.end(), buffer);
                    if (it != reseaux.end())
                        reseaux.push_back(it - reseaux.begin());   
                }
            
                routers.push_back(std::make_pair(router.getName(),reseaux));
            }

            routers_serialized = Router::routers_to_binary(routers);
        };

        friend std::ostream& operator<<(std::ostream& os, const Topology& r);
};

std::ostream& operator<<(std::ostream& os, const Topology& r) {
    for(auto i : r.topology)
        std::cout << i << std::endl;
    return os;
}