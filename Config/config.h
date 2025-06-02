#include <fstream>
#include <filesystem>
#include <mutex>
#include <string>
#include <vector>
#include <iostream>


class Config {
    private:
        std::string path = "/etc/ospfallacieux/config";
        std::ifstream inFile;
        std::ofstream outFile;
        std::mutex mutex;


        void createConfiguration(const std::string& text) {
            std::lock_guard<std::mutex> lock(mutex);
            outFile.open(path, std::fstream::app);

            if (outFile) {
                outFile << text << '\n';
                outFile.close(); // Fermer après écriture (à toi de voir si tu veux le garder ouvert)
            } else {
                std::cerr << "Erreur lors de l'ajout du réseau." << std::endl;
            }

            std::cout << text << " a été ajouté." << std::endl;
        }

        void deleteConfiguration(const std::string& text) {
            std::lock_guard<std::mutex> lock(mutex);
            const std::string path = "/etc/ospfallacieux/config";

            inFile.open(path);
            if (!inFile) {
                std::cerr << "Erreur lors de l'ouverture du fichier pour lecture." << std::endl;
                return;
            }

            std::vector<std::string> lines;
            std::string line;
            bool parameterExist = true;
            while (std::getline(inFile, line)) {
                if (line.find(text) == std::string::npos){
                    lines.push_back(line);
                    parameterExist = false;
                }
            }
            inFile.close();

            if(parameterExist){
                std::cout << "Chef le parametre '" << text << "' que tu veux delete existe pas" << std::endl;
                return;
            }

            outFile.open(path, std::ios::out | std::ios::trunc);
            if (!outFile) {
                std::cerr << "Erreur lors de l'ouverture du fichier pour écriture." << std::endl;
                return;
            }

            for (const auto& l : lines)
                outFile << l << "\n";

            outFile.close();
            std::cout << text << " a été supprimé." << std::endl;
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

    public:
        Config(){
            std::filesystem::path filePath(path);
            std::filesystem::path dirPath = filePath.parent_path();

            if (!std::filesystem::exists(dirPath)) {
                if (std::filesystem::create_directories(dirPath)) {
                    std::cout << "Dossier de configuration créé : " << dirPath << std::endl;
                } else {
                    std::cerr << "Erreur lors de la création du dossier." << std::endl;
                }
            }

            if (!std::filesystem::exists(filePath)) {
                std::ofstream file(filePath);
                if (file) {
                    std::cout << "Fichier créé : " << filePath << std::endl;
                } else {
                    std::cerr << "Erreur lors de la création du fichier." << std::endl;
                }
            } else {
                std::cout << "Le fichier existe déjà." << std::endl;
            }
            std::cout << std::endl;
        }

        ~Config() {
            if (inFile.is_open()) {
                inFile.close();
            }
            if (outFile.is_open()) {
                outFile.close();
            }
        }

        //commande possible
        //config -s
        //config -c <ip>
        //config -d <ip>
        static int checkParameters(int argc, char** argv){
            if(argc < 2)
                std::cout << "pas assez de param chef" << std::endl;
            else{
                char option = argv[1][1];

                if(option == 's')
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
                default:   
                    std::cout << "t'es bourré josé" << std::endl;
                    break;
            }
        }
};