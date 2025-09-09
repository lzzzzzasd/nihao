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

struct SavedItem {
    std::string name;
    std::string description;
    ItemType type;
    std::map<AttributeType, int> attributes;
};

class GameManager {
private:
    GameState m_currentState;
    std::unique_ptr<CPlayer> m_player;
    std::map<std::string, std::unique_ptr<Map>> m_maps;
    Map* m_currentMap;
    std::unique_ptr<Storage> m_inventory;
    bool m_running;
    std::string m_currentMapName;
    bool m_hasReceivedSupplies;
    std::map<std::string, bool> m_bossDefeated;

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
    bool SaveGame(const std::string& filename); //�浵
    bool LoadGame(const std::string& filename); //����
    void VisitVillageSupply(); // ���ʴ��񲹸�վ
    bool CheckSuppliesReceived(); // ����Ƿ�����ȡ����
    void MarkSuppliesReceived(); // �������ȡ����

public:
    GameManager();
    void Run();
};

#endif