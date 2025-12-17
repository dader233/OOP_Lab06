#include "../include/druid.h"
#include "../include/visitor_sim_fight.h"

Druid::Druid(unsigned int x, unsigned int y, const std::string &name) : Npc(x, y, name) {}

Druid::Druid(Druid &&other) noexcept : Npc(std::move(other)) {}

std::string Druid::info() const {
    return "druid(" + std::to_string(this->x) + ' ' + std::to_string(this->y) + ") " + 
           '[' + this->unique_name + ']';
}

bool Druid::accept(std::shared_ptr<Npc> defender) const {
    // Этот друид атакует defender
    VisitorFightDruid visitor;
    return visitor.fight(defender);
}