#include "../include/squirrel.h"
#include "../include/visitor_sim_fight.h"

Squirrel::Squirrel(unsigned int x, unsigned int y, const std::string &name) : Npc(x, y, name) {}

Squirrel::Squirrel(Squirrel &&other) noexcept : Npc(std::move(other)) {}

std::string Squirrel::info() const {
    return "squirrel(" + std::to_string(this->x) + ' ' + std::to_string(this->y) + ") " + 
           '[' + this->unique_name + ']';
}

bool Squirrel::accept(std::shared_ptr<Npc> defender) const {
    // Эта белка атакует defender
    VisitorFightSquirrel visitor;
    return visitor.fight(defender);
}