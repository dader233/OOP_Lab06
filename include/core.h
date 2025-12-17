#pragma once

#include <set>
#include <memory>
#include "npc.h"

void start_fight(std::set<std::shared_ptr<Npc>> &set_npc, const int distance);