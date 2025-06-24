#include <iostream>
#include <string>
#include <cstring>
#include <vector>
#include <cstdint>
#include <stdexcept>
#include <arpa/inet.h> 

class Reseau{
    private :
        std::string name;
        std::string addr;
        float poids;

        static std::string ip_to_string(const uint8_t ip[4]) {
            char str[INET_ADDRSTRLEN];
            inet_ntop(AF_INET, ip, str, INET_ADDRSTRLEN);
            return std::string(str);
        }
    
    public :

        Reseau(){};
        Reseau(std::string name_): name(name_), addr("0.0.0.0"), poids(0){};
        Reseau(std::string name_, std::string addr_): name(name_), addr(addr_), poids(10){};
        Reseau(std::string name_, std::string addr_, float poids_): name(name_), addr(addr_), poids(poids_){};
        Reseau(std::string name_, std::string addr_, float poids_, bool act_): name(name_), addr(addr_), poids(poids_){};
        
        std::string getName() const{
            return name;
        };

        void setName(std::string name_){
            name = name_;
        };

        std::string getAddr() const{
            return addr;
        };

        void setAddr(std::string addr_){
            addr = addr_;
        };

        float getPoids() const{
            return poids;
        };

        void getPoids(float poids_){
            poids = poids_;
        };

        bool includes(std::string str){ //check si la string passé en param est le nom ou l'adresse du réseau
            return str == name || str == addr;
        };

        static std::vector<Reseau> from_binary_to_reseaux(const uint8_t* data, size_t size) { //serialize le reseau en tableau binaire
            constexpr size_t RESEAU_SIZE = 17; //ça doit être des paquets de 17, si c'est pas le cas le fonction quitte
            if (size % RESEAU_SIZE != 0)
                throw std::runtime_error("Données corrompues ou incomplètes");

            std::vector<Reseau> reseaux;
            size_t count = size / RESEAU_SIZE;
            size_t offset = 0;

            for (size_t i = 0; i < count; ++i) {
                Reseau r;

                r.name = std::string(reinterpret_cast<const char*>(data + offset), 8); //récupere le nom du reseau
                r.name = r.name.c_str();
                offset += 8;

                uint8_t ip[4];
                memcpy(ip, data + offset, 4);
                std::string test = ip_to_string(ip);
                r.setAddr(test); //son ip

                offset += 4;

                memcpy(&r.poids, data + offset, sizeof(float)); //son poids
                offset += 4;

                reseaux.push_back(r);
            }

            return reseaux;
        };

        static std::vector<uint8_t> reseaux_to_binary(const std::vector<Reseau>& reseaux) { //deserialize un tableau binaire en router
            std::vector<uint8_t> data;

            for (const auto& r : reseaux) { 
                for (size_t i = 0; i < 8; ++i) { //on récupere le nom de chaque reseau, comblé par des /0 pour atteindre les 8 char.
                    if (i < r.name.size()) {
                        data.push_back(static_cast<uint8_t>(r.name[i]));
                    } else {
                        data.push_back(0);
                    }
                }

                for (int i = 0; i < 4; ++i) {
                    data.push_back(r.addr[i]);
                }

                uint8_t poidsBytes[4]; //ajoute le poids en tant que uint8_t
                std::memcpy(poidsBytes, &r.poids, sizeof(float));
                data.insert(data.end(), poidsBytes, poidsBytes + 4);
            }

            return data;
        }

        friend std::ostream& operator<<(std::ostream& os, const Reseau& r); //surcharge de l'opérateur <<  pour print l'objet

        bool operator==(const Reseau& reseau) const { //surcharge de l'opérateur == pour pouvoir utiliser certaines fonctions tel que std::find
            return addr == reseau.addr || name == reseau.name;
        };
};

std::ostream& operator<<(std::ostream& os, const Reseau& r) {
    os << "Network Name: " << r.name;
    os << "Address: " << r.addr << "\n";
    os << "Poids: " << r.poids << "\n";
    return os;
}