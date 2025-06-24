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

        void setName(std::string name_){
            name = name_;
        };

        std::string getName() const{
            return name;
        };

        bool isActive(){
            return active;
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

        std::map<std::string, float> calculateShortestPaths(const std::vector<Router>& allRouters,std::map<std::string, std::string>& predecessorMap){  //calcul le chemin le plus court et redonne le chemin
            std::map<std::string, float> distances;
            std::map<std::string, bool> visited;

            for (const Router& r : allRouters) {
                distances[r.getName()] = std::numeric_limits<float>::infinity();
                visited[r.getName()] = false;
            }

            distances[this->name] = 0.0f;

            auto cmp = [&distances](const std::string& left, const std::string& right) {
                return distances[left] > distances[right];
            };

            std::priority_queue<std::string, std::vector<std::string>, decltype(cmp)> pq(cmp);
            pq.push(this->name);

            std::map<std::string, Router> nameToRouter;
            for (const Router& r : allRouters) {
                nameToRouter[r.getName()] = r;
            }

            while (!pq.empty()) {
                std::string current = pq.top();
                pq.pop();

                if (visited[current]) continue;
                visited[current] = true;

                Router currentRouter = nameToRouter[current];

                if (!currentRouter.isActive()) continue;

                for (const auto& neighborInfo : currentRouter.getNeighbors()) {
                    Router neighbor = std::get<0>(neighborInfo);
                    Reseau link = std::get<1>(neighborInfo);
                    std::string neighborName = neighbor.getName();

                    float cost = link.getPoids();

                    if (distances[current] + cost < distances[neighborName]) {
                        distances[neighborName] = distances[current] + cost;
                        pq.push(neighborName);

                        // Met à jour le prédécesseur du voisin
                        predecessorMap[neighborName] = current;
                    }
                }
            }

            return distances;
        };

        static std::vector<uint8_t> routers_to_binary(const std::vector<std::pair<std::string, std::vector<int>>>& data) { //serialize le router en tableau binaire
            std::vector<uint8_t> buffer;

            auto append_int = [&](int32_t val) { //transforme un int en uint_8
                uint8_t* p = reinterpret_cast<uint8_t*>(&val);
                buffer.insert(buffer.end(), p, p + sizeof(int32_t));
            };

            for (const auto& [str, vec] : data) {
                std::string s = str.substr(0, 8); // Ajoute string sur 8 octets (padded avec '\0')
                s.resize(8, '\0');
                buffer.insert(buffer.end(), s.begin(), s.end());

                append_int(vec.size()); // Ajoute la taille du vector<int>

                for (int v : vec) //on ajoute les indexs des reseaux
                    append_int(v);
            }

            return buffer;
        };

        static std::vector<std::pair<std::string, std::vector<int>>> from_binary_to_routers(const uint8_t* data, size_t size) { //deserialize un tableau binaire en router
            std::vector<std::pair<std::string, std::vector<int>>> result;
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

                int32_t vec_len = read_int(); //récupere le nombre de reseau disponible

                if (offset + vec_len * sizeof(int32_t) > size) break;

                std::vector<int> vec;
                for (int i = 0; i < vec_len; ++i) {
                    vec.push_back(read_int());
                }

                result.emplace_back(str, vec);
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