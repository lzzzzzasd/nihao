#ifndef GAME_MANAGER_H
#define GAME_MANAGER_H

#include <memory>
#include <map>
#include "global.h"
#include "player.h"
#include "enemy.h"
#include "map.h"
#include "storage.h"
#include "store.h"
#include "fight_system.h"
#include "save_manager.h"

class GameManager {
private:
    GameState m_currentState;
    std::unique_ptr<CPlayer> m_player;
    std::map<std::string, std::unique_ptr<Map>> m_maps;
    Map* m_currentMap;
    std::unique_ptr<Storage> m_inventory;
    bool m_running;

    void InitializeMaps();
    void ExploreArea();
    void EncounterEnemy(bool isBoss);
    void FindItem();
    void ShowInventory();
    void UseItem(std::shared_ptr<Item> item);
    void VisitStore();
    void MoveToMap();
    void ShowMainMenu();
    void GameOver();

    // 存档相关方法
    void SaveGame();
    void LoadGame();

public:
    GameManager();
    void Run();
};

#endif
