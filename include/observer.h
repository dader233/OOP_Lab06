#pragma once

#include <iostream>
#include <fstream>
#include <memory>
#include "npc.h"

class Observer {
public:
    virtual ~Observer() = default;
    virtual void update(const Npc &attacker, const Npc &defender) = 0;
};

class FileObserver : public Observer {
public:
    void update(const Npc &attacker, const Npc &defender) override;
};

class StdinObserver : public Observer {
public:
    void update(const Npc &attacker, const Npc &defender) override;
};