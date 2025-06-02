#include <iostream>
#include <string>
#include <vector>
#include <tuple>
#include <queue>
#include <limits>
#include <map>
#include <set>
#include <utility>

class Router{
    private :
        std::string name;
        std::vector<std::tuple<Router,Reseau, std::string>> neighbors;
    
    public :

        Router(): name(""){};
        Router(std::string name_): name(name_){};

        std::string getName() const{
            return name;
        };

        std::vector<std::tuple<Router,Reseau, std::string>> getNeighbors() const{
            return neighbors;
        };

        void addNeighbor(Router rout, Reseau res, std::string i_){
            neighbors.push_back(std::make_tuple(rout, res, i_));
        }

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

                    if (!link.isActive()) continue; // ⚠️ Ne pas tenir compte si le réseau est désactivé

                    float cost = link.getPoids();

                    if (distances[current] + cost < distances[neighborName]) {
                        distances[neighborName] = distances[current] + cost;
                        pq.push(neighborName);
                    }
                }
            }

            return distances;
        }

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