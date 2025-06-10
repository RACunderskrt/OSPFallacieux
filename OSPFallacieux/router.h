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
    
    public :

        Router(): name(""){};
        Router(std::string name_): name(name_){};
        Router(std::string name_, std::string path): name(name_){
            std::mutex mutex;
            std::lock_guard<std::mutex> lock(mutex);
            std::string contenuDuFichier;
            
            std::ifstream inFile;
            inFile.open(path, std::ios::in);

            while (getline (inFile, contenuDuFichier)){
                size_t pos = contenuDuFichier.find(':');
                if (pos == std::string::npos) continue;
                    
                std::string ip = contenuDuFichier.substr(pos + 1);
                std::string name = contenuDuFichier.substr(0, pos);

                neighbors.push_back(std::make_tuple(Router(), Reseau(name, ip), ""));
            }
            inFile.close(); 
        };

        void setName(std::string name_){
            name = name_;
        };

        std::string getName() const{
            return name;
        };

        std::vector<std::tuple<Router,Reseau, std::string>> getNeighbors() const{
            return neighbors;
        };

        void addNeighbor(Router rout, Reseau res, std::string i_){
            neighbors.push_back(std::make_tuple(rout, res, i_));
        };

        friend std::ostream& operator<<(std::ostream& os, const Router& router);   

        std::map<std::string, float> calculateShortestPaths(const std::vector<Router>& allRouters) {
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

                for (const auto& neighborInfo : currentRouter.getNeighbors()) {
                    Router neighbor = std::get<0>(neighborInfo);
                    Reseau link = std::get<1>(neighborInfo);
                    std::string neighborName = neighbor.getName();

                    if (!link.isActive()) continue;

                    float cost = link.getPoids();

                    if (distances[current] + cost < distances[neighborName]) {
                        distances[neighborName] = distances[current] + cost;
                        pq.push(neighborName);
                    }
                }
            }

            return distances;
        };

        static std::vector<uint8_t> routers_to_binary(const std::vector<std::pair<std::string, std::vector<int>>>& data) {
            std::vector<uint8_t> buffer;

            auto append_int = [&](int32_t val) {
                uint8_t* p = reinterpret_cast<uint8_t*>(&val);
                buffer.insert(buffer.end(), p, p + sizeof(int32_t));
            };

            for (const auto& [str, vec] : data) {
                // Ajoute string sur 8 octets (padded avec '\0')
                std::string s = str.substr(0, 8);
                s.resize(8, '\0');
                buffer.insert(buffer.end(), s.begin(), s.end());

                // Ajoute la taille du vector<int>
                append_int(vec.size());

                // Ajoute les int
                for (int v : vec) {
                    append_int(v);
                }
            }

            return buffer;
        };

        static std::vector<std::pair<std::string, std::vector<int>>> from_binary_to_routers(const uint8_t* data, size_t size) {
            std::vector<std::pair<std::string, std::vector<int>>> result;
            size_t offset = 0;

            auto read_int = [&]() -> int32_t {
                int32_t val;
                memcpy(&val, data + offset, sizeof(int32_t));
                offset += sizeof(int32_t);
                return val;
            };

            while (offset + 8 + sizeof(int32_t) <= size) {
                std::string str(reinterpret_cast<const char*>(data + offset), 8);
                str = str.c_str();
                offset += 8;

                int32_t vec_len = read_int();

                if (offset + vec_len * sizeof(int32_t) > size) break;

                std::vector<int> vec;
                for (int i = 0; i < vec_len; ++i) {
                    vec.push_back(read_int());
                }

                result.emplace_back(str, vec);
            }

            return result;
        };

};

std::ostream& operator<<(std::ostream& os, const Router& router) {
    os << "Router Name: " << router.name << "\n";
    os << "Neighbors:\n";
    for (const auto& neighbor : router.neighbors) {
        os << " - " << std::get<0>(neighbor).name
        << " (" << std::get<1>(neighbor) << ")\n"
        << "via " << std::get<2>(neighbor) << std::endl;
    }
    return os;
}