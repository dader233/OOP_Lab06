#include <iostream>
#include <memory>
#include <fstream>

#include "include/core.h"
#include "include/factory_npc.h"
#include "include/observer.h"

int main() {
    try {
        // Очищаем логи
        std::ofstream log_file("../log.txt", std::ios::out);
        if (!log_file.is_open()) {
            std::cerr << "Warning: Cannot open log.txt for writing\n";
        }
        log_file.close();

        std::cout << "================ NPC Factory Test ================\n\n";
        
        // Создание NPC через фабрику
        std::shared_ptr<Npc> orc_ptr = FactoryNpc::create_npc(TypeNpc::orc, "Grom", 10, 20);
        std::shared_ptr<Npc> squirrel_ptr = FactoryNpc::create_npc(TypeNpc::squirrel, "Chip", 15, 25);
        std::shared_ptr<Npc> druid_ptr = FactoryNpc::create_npc(TypeNpc::druid, "Malfurion", 30, 40);

        std::cout << "Created NPCs:\n";
        orc_ptr->print(std::cout) << '\n';
        squirrel_ptr->print(std::cout) << '\n';
        druid_ptr->print(std::cout) << "\n\n";

        // Тест загрузки из файла
        std::ifstream in;
        in.open("../input.txt", std::ios::in);
        
        if (!in.is_open()) {
            std::cerr << "Warning: input.txt not found, creating default NPCs\n";
            // Создаем несколько NPC для тестирования в новом формате
            std::ofstream default_input("../input.txt");
            default_input << "squirrel 100 200 ForestSquirrel\n";
            default_input << "druid 150 250 WiseDruid\n";
            default_input << "orc 200 300 BattleOrc\n";
            default_input.close();
            in.open("../input.txt", std::ios::in);
        }

        std::cout << "Loading NPCs from file:\n";
        std::shared_ptr<Npc> squirrel_file = FactoryNpc::create_npc_from_file(in);
        std::shared_ptr<Npc> druid_file = FactoryNpc::create_npc_from_file(in);
        std::shared_ptr<Npc> orc_file = FactoryNpc::create_npc_from_file(in);
        std::cout << squirrel_file->info() << '\n';
        std::cout << druid_file->info() << '\n';
        std::cout << orc_file->info() << "\n\n";

        in.close();

        // Сохранение в файл
        std::cout << "Saving NPCs to output.txt:\n";
        std::ofstream os("../output.txt", std::ios::out);
        
        orc_ptr->save(os) << '\n';
        squirrel_ptr->save(os) << '\n';
        druid_ptr->save(os) << '\n';
        squirrel_file->save(os) << '\n';
        druid_file->save(os) << '\n';
        orc_file->save(os) << '\n';
        
        os.close();
        std::cout << "NPCs saved to 'output.txt'\n\n";

        // Тестирование правил боя
        std::cout << "================ Combat Rules Test ================\n\n";
        
        std::cout << "1. Orc attacks Druid: ";
        std::cout << (orc_ptr->accept(druid_ptr) ? "Orc KILLS Druid RIGHT_VAR" : "Orc does NOT kill Druid") << '\n';
        
        std::cout << "2. Druid attacks Squirrel: ";
        std::cout << (druid_ptr->accept(squirrel_ptr) ? "Druid KILLS Squirrel RIGHT_VAR" : "Druid does NOT kill Squirrel") << '\n';
        
        std::cout << "3. Squirrel attacks Orc: ";
        std::cout << (squirrel_ptr->accept(orc_ptr) ? "Squirrel KILLS Orc" : "Squirrel does NOT kill Orc (peaceful) RIGHT_VAR") << '\n';
        
        std::cout << "4. Orc attacks Squirrel: ";
        std::cout << (orc_ptr->accept(squirrel_ptr) ? "Orc KILLS Squirrel" : "Orc does NOT kill Squirrel RIGHT_VAR") << '\n';
        
        std::cout << "5. Druid attacks Orc: ";
        std::cout << (druid_ptr->accept(orc_ptr) ? "Druid KILLS Orc" : "Druid does NOT kill Orc RIGHT_VAR") << '\n';
        
        std::cout << "6. Squirrel attacks Druid: ";
        std::cout << (squirrel_ptr->accept(druid_ptr) ? "Squirrel KILLS Druid" : "Squirrel does NOT kill Druid (peaceful) RIGHT_VAR") << "\n\n";

        // Тестирование Observer паттерна
        std::cout << "================ Observer Test ================\n\n";
        
        auto stdin_observer = std::make_shared<StdinObserver>();
        auto log_observer = std::make_shared<FileObserver>();
        
        orc_ptr->attach(stdin_observer);
        orc_ptr->attach(log_observer);
        
        std::cout << "Testing notification (with both observers):\n";
        orc_ptr->notify(*druid_ptr);
        
        std::cout << "\nAfter detaching stdin observer:\n";
        orc_ptr->detach(stdin_observer);
        orc_ptr->notify(*druid_ptr);  // Только в файл
        
        std::cout << "(Check log.txt for file observer output)\n\n";

        // Основная симуляция боя
        std::cout << "================ Battle Simulation ================\n\n";
        
        std::set<std::shared_ptr<Npc>> npc_set;
        npc_set.insert(orc_ptr);
        npc_set.insert(squirrel_ptr);
        npc_set.insert(druid_ptr);
        npc_set.insert(squirrel_file);
        npc_set.insert(druid_file);
        npc_set.insert(orc_file);

        // Прикрепляем наблюдателей ко всем NPC
        for (auto &npc : npc_set) {
            npc->attach(stdin_observer);
            npc->attach(log_observer);
        }

        std::cout << "NPCs before battle (" << npc_set.size() << " total):\n";
        print_all_npc(std::cout, npc_set) << "\n";

        // Запускаем бой с дальностью 100 метров
        std::cout << "Starting battle with range 100...\n";
        start_fight(npc_set, 100);

        std::cout << "\nNPCs after battle (" << npc_set.size() << " survivors):\n";
        if (npc_set.empty()) {
            std::cout << "All NPCs are dead!\n";
        } else {
            print_all_npc(std::cout, npc_set) << "\n";
        }

        // Дополнительный тест с меньшей дистанцией
        std::cout << "\n================ Test with Smaller Range ================\n\n";
        
        // Создаем NPC близко друг к другу
        std::set<std::shared_ptr<Npc>> close_npcs;
        close_npcs.insert(FactoryNpc::create_npc(TypeNpc::orc, "Orc1", 0, 0));
        close_npcs.insert(FactoryNpc::create_npc(TypeNpc::druid, "Druid1", 1, 1));
        close_npcs.insert(FactoryNpc::create_npc(TypeNpc::squirrel, "Squirrel1", 2, 2));
        
        for (auto &npc : close_npcs) {
            npc->attach(stdin_observer);
        }
        
        std::cout << "Close NPCs before battle:\n";
        print_all_npc(std::cout, close_npcs) << "\n";
        
        std::cout << "Battle with range 5 (should fight):\n";
        start_fight(close_npcs, 5);
        
        std::cout << "Survivors: " << close_npcs.size() << "\n";
        print_all_npc(std::cout, close_npcs);
        
        std::cout << "\n================ Program Finished ================\n";
        
    } catch (const std::exception &e) {
        std::cerr << "Error: " << e.what() << '\n';
        return 1;
    }
    
    return 0;
}