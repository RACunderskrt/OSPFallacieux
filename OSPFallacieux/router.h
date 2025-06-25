#include <iostream>
#include <string>
#include <vector>
#include <tuple>
#include <queue>
#include <limits>
#include <map>
#include <set>
#include <utility>
#include <fstream>
#include <mutex>

class Router{
    private :
        std::string name;
        std::vector<std::tuple<Router,Reseau, std::string>> neighbors;
        bool active;

        void loadConfigurationFile(std::string path, std::string name_ = "R1"){
            std::mutex mutex;
            std::lock_guard<std::mutex> lock(mutex);
            std::string contenuDuFichier;
            
            std::ifstream inFile;
            inFile.open(path, std::ios::in);

            while (getline (inFile, contenuDuFichier)){
                size_t pos = contenuDuFichier.find(':');
                if (pos == std::string::npos) continue;
                    
                std::string before = contenuDuFichier.substr(0, pos); //la donnée est coupée en 2 par un ':'
                std::string after = contenuDuFichier.substr(pos + 1);
                
                if(before == "/name"){
                    name = after;
                    continue;
                }
                else
                    neighbors.push_back(std::make_tuple(Router(), Reseau(before, after), ""));
            }
            inFile.close(); 

            if(!name.size()){
                std::cerr << "Le router n'est pas nommé, cela peut poser des problèmes." << std::endl;
                name = name_;
            }
        }
    
    public :

        Router(): name(""), active(true){};
        Router(std::string path): active(true){
            loadConfigurationFile(path);
        };
        Router(std::string path, std::string name_): active(true){
            if(!path.size())
                loadConfigurationFile(path, name_);
            else
                name = name_;
        };
        Router(std::string name_, bool active_):name(name_), active(active_){};

        void setName(std::string name_){
            name = name_;
        };

        std::string getName() const{
            return name;
        };

        bool isActive(){
            return active;
        }

        void activate(){
            active = true;
        }

        void desactivate(){
            active = false;
        }

        std::vector<std::tuple<Router,Reseau, std::string>> getNeighbors() const{
            return neighbors;
        };

        void addNeighbor(Router rout, Reseau res, std::string i_){
            neighbors.push_back(std::make_tuple(rout, res, i_));
        };

        std::vector<Reseau> getReseaux(){
            std::vector<Reseau> res;
            for(auto r: neighbors){
                res.push_back(std::get<1>(r));
            }
            return res;
        }

        std::string findInterface(std::string routerName){ //grâce au nom du router cible, on retrouve l'interface necessaire au routage
            for(auto voisin: neighbors){
                if(std::get<0>(voisin).getName() == routerName) //pour obtenir un objet d'un tuple c'est std::get<i> avec i l'index dans le tuple de l'objet
                    return std::get<2>(voisin);
            }
            return "";
        };

        std::unordered_map<std::string, std::vector<std::string>> dijkstra_all_paths(std::vector<Router> routers) {
            std::unordered_map<std::string, float> distances;
            std::unordered_map<std::string, std::string> previous;
            std::set<std::pair<float, std::string>> pq;

            for (auto r : routers) {
                distances[r.getName()] = std::numeric_limits<float>::infinity();
            }

            distances[name] = 0.0f;
            pq.insert({0.0f, name});

            while (!pq.empty()) {
                auto [dist, current_name] = *pq.begin();
                pq.erase(pq.begin());

                Router current = Router();
                for (auto r : routers) {
                    if (r.getName() == current_name) {
                        current = r;
                        break;
                    }
                }

                if (!current.isActive()) continue;

                for (auto& [neighbor, reseau, _] : current.getNeighbors()) {
                    if (!neighbor.isActive()) continue;

                    float new_dist = dist + reseau.getPoids();
                    if (new_dist < distances[neighbor.getName()]) {
                        pq.erase({distances[neighbor.getName()], neighbor.getName()});
                        distances[neighbor.getName()] = new_dist;
                        previous[neighbor.getName()] = current_name;
                        pq.insert({new_dist, neighbor.getName()});
                    }
                }
            }

            // Reconstituer les chemins vers tous les points
            std::unordered_map<std::string, std::vector<std::string>> paths;

            for (auto& [target, _] : distances) {
                if (target == name) continue;

                std::vector<std::string> path;
                std::string current = target;
                while (previous.find(current) != previous.end()) {
                    path.push_back(current);
                    current = previous[current];
                }
                if (current == name) {
                    path.push_back(name);
                    std::reverse(path.begin(), path.end());
                    paths[target] = path;
                }
            }

            return paths;
        };

        static std::vector<uint8_t> routers_to_binary(const std::vector<std::pair<Router, std::vector<int>>>& data) { //serialize le router en tableau binaire
            std::vector<uint8_t> buffer;

            auto append_int = [&](int32_t val) { //transforme un int en uint_8
                uint8_t* p = reinterpret_cast<uint8_t*>(&val);
                buffer.insert(buffer.end(), p, p + sizeof(int32_t));
            };

            for (const auto& pair : data) {
                Router bufRout = std::get<0>(pair);
                std::vector<int> vec = std::get<1>(pair);
                std::string s = bufRout.getName().substr(0, 8); // Ajoute string sur 8 octets (padded avec '\0')
                s.resize(8, '\0');
                buffer.insert(buffer.end(), s.begin(), s.end());

                buffer.push_back(bufRout.isActive());

                append_int(vec.size()); // Ajoute la taille du vector<int>

                for (int v : vec) //on ajoute les indexs des reseaux
                    append_int(v);
            }

            return buffer;
        };

        static std::vector<std::pair<Router, std::vector<int>>> from_binary_to_routers(const uint8_t* data, size_t size) { //deserialize un tableau binaire en router
            std::vector<std::pair<Router, std::vector<int>>> result;
            size_t offset = 0;

            auto read_int = [&]() -> int32_t { //transforme plusieurs uint8_t en int
                int32_t val;
                memcpy(&val, data + offset, sizeof(int32_t));
                offset += sizeof(int32_t);
                return val;
            };

            while (offset + 8 + sizeof(int32_t) <= size) {
                std::string str(reinterpret_cast<const char*>(data + offset), 8); //récupere les 8 octets du nom
                str = str.c_str();
                offset += 8;

                bool activated = data[offset];
                offset += 1;
                
                std::cout << "activated " << activated << " " << str << std::endl;

                int32_t vec_len = read_int(); //récupere le nombre de reseau disponible

                if (offset + vec_len * sizeof(int32_t) > size) break;

                std::vector<int> vec;
                for (int i = 0; i < vec_len; ++i) {
                    vec.push_back(read_int());
                }

                result.emplace_back(Router(str,activated), vec);
            }

            return result;
        };

        friend std::ostream& operator<<(std::ostream& os, const Router& router); //surcharge de l'opérateur <<  pour print l'objet

};

std::ostream& operator<<(std::ostream& os, const Router& router) {
    os << "Router Name: " << router.name;
    if(router.active)
        os << " - Active";
    os << "\n";
    os << "Neighbors:\n";
    for (const auto& neighbor : router.neighbors) {
        os << " - " << std::get<0>(neighbor).name
        << " (" << std::get<1>(neighbor) << ")\n"
        << "via " << std::get<2>(neighbor) << std::endl;
    }
    return os;
}
