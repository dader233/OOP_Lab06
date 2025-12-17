#pragma once

#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <memory>
#include <algorithm>
#include <set>

class Observer;

class Npc {
protected:
    unsigned int x;
    unsigned int y;
    std::string unique_name;
    std::vector<std::shared_ptr<Observer>> observers;

public:
    Npc() = default;
    Npc(const unsigned int x, const unsigned int y, const std::string &name);
    Npc(Npc &&other) noexcept;
    virtual ~Npc() = default;

    virtual std::string info() const = 0;
    virtual bool accept(std::shared_ptr<Npc> defender) const = 0;

    unsigned int get_x() const;
    unsigned int get_y() const;
    std::string get_name() const;

    void attach(std::shared_ptr<Observer> observer);
    void detach(std::shared_ptr<Observer> observer);
    void notify(Npc &defender);

    virtual std::ofstream &save(std::ofstream &os) const;
    virtual std::ostream &print(std::ostream &os) const;

    friend std::istream &operator>>(std::istream &is, Npc &npc);
};

std::ostream &print_all_npc(std::ostream &os, const std::set<std::shared_ptr<Npc>> &set_npc);