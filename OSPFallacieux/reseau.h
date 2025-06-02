#include <iostream>
#include <string>

class Reseau{
    private :
        std::string name;
        std::string addr;
        float poids;
        bool active;
    
    public :

        Reseau(std::string name_): name(name_), addr("0.0.0.0"), poids(0), active(true){};
        Reseau(std::string name_, std::string addr_): name(name_), addr(addr_), poids(0), active(true){};
        Reseau(std::string name_, std::string addr_, float poids_): name(name_), addr(addr_), poids(poids_), active(true){};
        Reseau(std::string name_, std::string addr_, float poids_, bool act_): name(name_), addr(addr_), poids(poids_), active(act_){};
        std::string getName() const{
            return name;
        };

        std::string getAddr() const{
            return addr;
        };

        float getPoids() const{
            return poids;
        };

        bool isActive() const{
            return active;
        }

        void desactivate(){
            active = false;
        };

        friend std::ostream& operator<<(std::ostream& os, const Reseau& r);
};

std::ostream& operator<<(std::ostream& os, const Reseau& r) {
    os << "Network Name: " << r.name;
    if(r.isActive())
        os << " - Active";
    os << "\n";
    os << "Address: " << r.addr << "\n";
    os << "Poids: " << r.poids << "\n";
    return os;
}