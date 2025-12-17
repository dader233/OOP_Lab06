#include "../include/core.h"
#include "../include/visitor_sim_fight.h"

void start_fight(std::set<std::shared_ptr<Npc>> &set_npc, const int distance) {
    std::set<std::shared_ptr<Npc>> dead_list = simulate_fight(set_npc, distance);
    // Удаляем убитых
    for (auto &dead_npc : dead_list) {
        set_npc.erase(dead_npc);
    }
}