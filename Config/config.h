#include <fstream>
#include <filesystem>
#include <mutex>
#include <string>
#include <vector>
#include <iostream>
#include <sstream>
#include <regex>


class Config {
    private:
        std::string path = "/etc/ospfallacieux/config";
        std::ifstream inFile;
        std::ofstream outFile;
        std::mutex mutex;

        bool checkConfiguration(const std::string& text) const{
            size_t pos = text.find(':');
            if (pos == std::string::npos) return false;

            std::string nom = text.substr(0, pos);
            std::string adresse = text.substr(pos + 1);

            // Vérifie la longueur et caractères du nom
            if (nom.empty() || nom.length() > 8) return false;
            for (char c : nom) {
                if (!std::isalnum(c) && c != '_') return false;
            }

            // Vérifie le format général IPv4 avec regex
            std::regex ipv4Regex(R"(^(\d{1,3}\.){3}\d{1,3}$)");
            if (!std::regex_match(adresse, ipv4Regex)) return false;

            // Vérifie que chaque octet est entre 0 et 255
            std::istringstream ss(adresse);
            std::string segment;
            while (std::getline(ss, segment, '.')) {
                int octet = std::stoi(segment);
                if (octet < 0 || octet > 255) return false;
            }

            return true;
        }

        void createConfiguration(const std::string& text) {
            if(!checkConfiguration(text)){ 
                std::cerr << "Mauvais format de configuration." << std::endl;
                return;
            }
            std::lock_guard<std::mutex> lock(mutex);
            outFile.open(path, std::fstream::app);

            if (outFile) {
                outFile << text << '\n';
                outFile.close();
            } else {
                std::cerr << "Erreur lors de l'ajout du réseau." << std::endl;
            }
            std::cout << text << " a été ajouté." << std::endl;
        }

        void deleteConfiguration(const std::string& param) {
            std::lock_guard<std::mutex> lock(mutex);

            inFile.open(path);
            if (!inFile) {
                std::cerr << "Erreur lors de l'ouverture du fichier pour lecture." << std::endl;
                return;
            }

            std::vector<std::string> lignes;
            std::string ligne;
            bool ligneSupprimee = false;

            while (std::getline(inFile, ligne)) {
                size_t pos = ligne.find(':');
                if (pos != std::string::npos) {
                    std::string nom = ligne.substr(0, pos);
                    std::string adresse = ligne.substr(pos + 1);

                    // Supprimer la ligne si le paramètre correspond au nom ou à l’adresse
                    if (param == nom || param == adresse || param == ligne) {
                        ligneSupprimee = true;
                        continue; // ne pas ajouter cette ligne
                    }
                }

                lignes.push_back(ligne);
            }

            inFile.close();

            if (ligneSupprimee) {
                outFile.open(path, std::ios::trunc);
                if (!outFile) {
                    std::cerr << "Erreur lors de l'ouverture du fichier pour écriture." << std::endl;
                    return;
                }

                for (const auto& l : lignes) {
                    outFile << l << '\n';
                }

                outFile.close();
            }
            else{
                std::cerr << "La configuration recherchée n'existe pas." << std::endl;
            }
        }

        void showConfiguration() {
            std::lock_guard<std::mutex> lock(mutex);
            std::string contenuDuFichier;

            inFile.open(path, std::ios::in);
            int cmp = 1;
            std::cout << "OSPFallacieux Configuration :" << std::endl;
            while (getline (inFile, contenuDuFichier)){
                std::cout << cmp << " - " << contenuDuFichier << std::endl;
                cmp++;
            }
            inFile.close(); 
        }

        void startService(){
            system("systemctl start server.service"); //mettre la commande pour lancer le service
        }

        void stopService(){
            system("systemctl stop server.service"); //mettre la commande pour stopper le service
        }

        void setName(const std::string& param){
            if(param.size() > 8){
                std::cerr << "Le nom du router ne peut dépasser les 8 caracteres." << std::endl;
                return;
            }
            std::lock_guard<std::mutex> lock(mutex);

            inFile.open(path);
            if (!inFile) {
                std::cerr << "Erreur lors de l'ouverture du fichier pour lecture." << std::endl;
                return;
            }

            std::vector<std::string> lignes;
            std::string ligne;

            while (std::getline(inFile, ligne)) {
                size_t pos = ligne.find(':');
                if (pos != std::string::npos) {
                    std::string nom = ligne.substr(0, pos);

                    // Supprimer la ligne si le paramètre correspond au nom ou à l’adresse
                    if ('/' == nom[0]) {
                        continue; // ne pas ajouter cette ligne
                    }
                }

                lignes.push_back(ligne);
            }

            inFile.close();

            outFile.open(path, std::ios::trunc);
            if (!outFile) {
                std::cerr << "Erreur lors de l'ouverture du fichier pour écriture." << std::endl;
                return;
            }
            outFile << "/name:" << param << '\n';
            for (const auto& l : lignes) {
                outFile << l << '\n';
            }
            
            outFile.close();
        }

    public:
        Config(){
            std::filesystem::path filePath(path);
            std::filesystem::path dirPath = filePath.parent_path();

            if (!std::filesystem::exists(dirPath) && !std::filesystem::create_directories(dirPath))
                std::cerr << "Erreur lors de la création du dossier." << std::endl;
            

            if (!std::filesystem::exists(filePath)) {
                std::ofstream file(filePath);
                if (file)
                    std::cout << "Fichier créé : " << filePath << std::endl;
                else
                    std::cerr << "Erreur lors de la création du fichier." << std::endl;
            }
        }

        ~Config() {
            if (inFile.is_open())
                inFile.close();
            if (outFile.is_open())
                outFile.close();
        }

        //commandes possibles
        //config -s
        //config -c <name>:<ip>
        //config -d <ip> || <name>
        //config start
        //config stop
        //config name <name>
        static int checkParameters(int argc, char** argv){
            if(argc < 2)
                std::cout << "pas assez de param chef" << std::endl;
            else{
                char option = argv[1][1];
                std::string long_option = argv[1];

                if(long_option == "start")
                    return 4;
                else if(long_option == "stop")
                    return 5;
                else if(long_option == "name")
                    return 6;
                else if(option == 's')
                    return 1;
                else if(option == 'c' && argc > 2)
                    return 2;
                else if(option == 'd' && argc > 2)
                    return 3;
                else
                    return 0;
            }
            return 0; 
        }

        void chooseOption(int option, char* parameter){
            switch(option){
                case 1:
                    showConfiguration();
                    break;
                case 2:
                    createConfiguration(parameter);
                    break;
                case 3:
                    deleteConfiguration(parameter);
                    break;
                case 4:
                    startService();
                    break;
                case 5:
                    stopService();
                    break;
                case 6:
                    setName(parameter);
                    break;
                default:   
                    std::cout << "t'es bourré josé" << std::endl;
                    break;
            }
        }
};