#include <iostream>
#include <chrono>
#include <thread>

int main() {
    int count = 0;

    while (true) {
        // Afficher un message à chaque tick
        std::cout << "Tick #"
                  << std::endl;

        // Attendre 10 secondes
        std::chrono::system_clock::to_time_t(std::chrono::system_clock::now());
        std::this_thread::sleep_for(std::chrono::seconds(1));
    }

    return 0;
}