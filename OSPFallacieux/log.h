#include <iostream>
#include <fstream>
#include <string>
#include <mutex>
#include <chrono>
#include <iomanip>
#include <ctime>

class Log {
    private:
        std::ofstream logFile;
        std::mutex mutex;

        std::string timestamp() {
            auto now = std::chrono::system_clock::now();
            std::time_t now_time = std::chrono::system_clock::to_time_t(now);
            std::tm tm;
            localtime_r(&now_time, &tm);
            std::ostringstream oss;
            oss << std::put_time(&tm, "%Y-%m-%d %H:%M:%S");
            return oss.str();
        }
    
    public:

    Log(const std::string& filename)
        : logFile(filename, std::ios::app) // Ouvre en mode ajout
    {
        if (!logFile.is_open()) {
            throw std::runtime_error("Impossible d'ouvrir le fichier de log.");
        }
    }

    ~Log() {
        if (logFile.is_open()) {
            logFile.close();
        }
    }

    void log(const std::string& message) {
        std::lock_guard<std::mutex> lock(mutex);  // Pour thread safety

        logFile << timestamp() << " - " << message << std::endl;
    }


};