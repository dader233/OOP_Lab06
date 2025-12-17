#include "../include/observer.h"

void FileObserver::update(const Npc &attacker, const Npc &defender) {
    std::ofstream log_file("../log.txt", std::ios::app);

    if (log_file.is_open()) {
        log_file << "[file observer]: ";
        attacker.save(log_file);
        log_file << " killed ";
        defender.save(log_file);
        log_file << '\n';
        log_file.close();
    } else {
        throw std::logic_error("File log.txt could not be opened");
    }
}

void StdinObserver::update(const Npc &attacker, const Npc &defender) {
    std::cout << "[stdin observer]: ";
    attacker.print(std::cout);
    std::cout << " killed ";
    defender.print(std::cout);
    std::cout << '\n';
}