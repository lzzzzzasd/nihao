#include "game_manager.h"
#include <iostream>
#include <string>
#include <cstdlib>
#include <ctime>
#include <stdexcept>
#include <fstream>
#include <sstream>

// 定义默认存档文件名常量
const std::string DEFAULT_SAVE_FILE = "default_save";

GameManager::GameManager()
    : m_currentState(GameState::MAIN_MENU), m_currentMap(nullptr), m_running(true) {
    m_inventory = std::make_unique<Storage>();
    InitializeMaps();
    std::srand(static_cast<unsigned int>(std::time(nullptr)));
}

void GameManager::InitializeMaps() {
    m_maps["森林"] = std::make_unique<Map>("森林");
    m_maps["山洞"] = std::make_unique<Map>("山洞");
    m_maps["山顶"] = std::make_unique<Map>("山顶", true);
    m_maps["商店"] = std::make_unique<Map>("商店");

    m_maps["森林"]->AddConnectedMap("山洞");
    m_maps["森林"]->AddConnectedMap("商店");

    m_maps["山洞"]->AddConnectedMap("森林");
    m_maps["山洞"]->AddConnectedMap("山顶");

    m_maps["山顶"]->AddConnectedMap("山洞");

    m_maps["商店"]->AddConnectedMap("森林");
}

void GameManager::Run() {
    while (m_running) {
        switch (m_currentState) {
        case GameState::MAIN_MENU: ShowMainMenu(); break;
        case GameState::EXPLORING: ExploreArea(); break;
        case GameState::INVENTORY: ShowInventory(); break;
        case GameState::STORE: VisitStore(); m_currentState = GameState::EXPLORING; break;
        case GameState::GAME_OVER: GameOver(); break;
        case GameState::SAVE_MENU: ShowSaveMenu(); break;
        default: m_currentState = GameState::MAIN_MENU;
        }
    }
}

void GameManager::ShowMainMenu() {
    system("cls");
    std::cout << "===== 欢迎来到MUD游戏 =====" << std::endl;
    std::cout << "1. 开始新游戏\n2. 继续上次游戏\n3. 退出游戏\n请选择: ";
    std::string choice;
    std::cin >> choice;

    if (choice == "1") {
        std::string name;
        std::cout << "请输入你的名字: ";
        std::cin >> name;
        m_player = std::make_unique<CPlayer>(name);
        m_currentMap = m_maps["森林"].get();
        m_player->SetHP(m_player->GetMaxHP());
        m_currentState = GameState::EXPLORING;
    }
    else if (choice == "2") { // 加载默认存档
        if (LoadGame(DEFAULT_SAVE_FILE)) {
            std::cout << "加载成功!" << std::endl;
            system("pause");
            m_currentState = GameState::EXPLORING;
        }
        else {
            std::cout << "加载失败! 可能没有存档" << std::endl;
            system("pause");
        }
    }
    else if (choice == "3") {
        m_running = false;
    }
    else {
        std::cout << "无效选择" << std::endl;
        system("pause");
    }
}

void GameManager::ExploreArea() {
    system("cls");
    std::cout << "===== " << m_currentMap->GetName() << " =====" << std::endl;
    std::cout << "当前位置: " << m_currentMap->GetName() << std::endl;

    std::string choice;
    if (m_currentMap->GetName() == "森林") {
        std::cout << "这是一片茂密的森林，你可以去山洞或者商店。" << std::endl;
        std::cout << "1. 前往山洞" << std::endl;
        std::cout << "2. 前往商店" << std::endl;
        std::cout << "3. 查看状态" << std::endl;
        std::cout << "4. 查看背包" << std::endl;
        std::cout << "5. 存档" << std::endl;  // 存档选项
        std::cout << "6. 退出游戏" << std::endl;

        std::cin >> choice;
        if (choice == "1") { MoveToMap(); }
        else if (choice == "2") { m_currentState = GameState::STORE; }
        else if (choice == "3") { m_player->ShowStatus(); system("pause"); }
        else if (choice == "4") { m_currentState = GameState::INVENTORY; }
        else if (choice == "5") { m_currentState = GameState::SAVE_MENU; } // 进入存档处理
        else if (choice == "6") { m_running = false; }
        else { std::cout << "无效选择" << std::endl; system("pause"); }
        return;
    }

    if (m_currentMap->GetName() == "商店") {
        std::cout << "这里是商店，可以购买物品和休息。" << std::endl;
        std::cout << "1. 购买物品" << std::endl;
        std::cout << "2. 前往森林" << std::endl;
        std::cout << "3. 查看状态" << std::endl;
        std::cout << "4. 查看背包" << std::endl;
        std::cout << "5. 存档" << std::endl;  // 存档选项
        std::cout << "6. 退出游戏" << std::endl;
        std::cin >> choice;
        if (choice == "1") { m_currentState = GameState::STORE; }
        else if (choice == "2") { MoveToMap(); }
        else if (choice == "3") { m_player->ShowStatus(); system("pause"); }
        else if (choice == "4") { m_currentState = GameState::INVENTORY; }
        else if (choice == "5") { m_currentState = GameState::SAVE_MENU; } // 进入存档处理
        else if (choice == "6") { m_running = false; }
        else { std::cout << "无效选择" << std::endl; system("pause"); }
        return;
    }

    std::cout << "请选择探索方式" << std::endl;
    std::cout << "1. 小怪战斗" << std::endl;
    std::cout << "2. BOSS战斗" << std::endl;
    std::cout << "3. 查看状态" << std::endl;
    std::cout << "4. 查看背包" << std::endl;
    std::cout << "5. 前往其他地图" << std::endl;
    std::cout << "6. 保存并退出" << std::endl;  // 存档选项
    std::cout << "7. 直接退出游戏" << std::endl;

    std::cin >> choice;
    if (choice == "1") { EncounterEnemy(false); }
    else if (choice == "2") {
        if (m_currentMap->HasBoss()) EncounterEnemy(true);
        else { std::cout << "当前地图没有BOSS" << std::endl; system("pause"); }
    }
    else if (choice == "3") { m_player->ShowStatus(); system("pause"); }
    else if (choice == "4") { m_currentState = GameState::INVENTORY; }
    else if (choice == "5") { MoveToMap(); }
    else if (choice == "6") { m_currentState = GameState::SAVE_MENU; } // 进入存档处理
    else if (choice == "7") { m_running = false; }
    else { std::cout << "无效选择" << std::endl; system("pause"); }
}

void GameManager::EncounterEnemy(bool isBoss) {
    system("cls");
    if (isBoss) {
        std::cout << "你遇到了BOSS!" << std::endl;
        auto enemy = std::make_unique<CEnemy>("暗影魔王", 200.0f, 30.0f, 10.0f, 10.0f, "暗影冲击", 2.0f, "造成巨大伤害");
        enemy->ShowStatus();
        std::cout << "准备战斗..." << std::endl;
        system("pause");
        CFightSystem fightSystem(std::move(m_player), std::move(enemy));
        fightSystem.StartFight();
        m_player = fightSystem.releasePlayer();
        if (!m_player->IsAlive()) m_currentState = GameState::GAME_OVER;
    }
    else {
        std::cout << "你遇到了一只小怪!" << std::endl;
        auto enemy = std::make_unique<CEnemy>("狼人", 20.0f, 5.0f, 2.0f, 1.0f);
        enemy->ShowStatus();
        std::cout << "准备战斗..." << std::endl;
        system("pause");
        CFightSystem fightSystem(std::move(m_player), std::move(enemy));
        fightSystem.StartFight();
        m_player = fightSystem.releasePlayer();
        if (!m_player->IsAlive()) m_currentState = GameState::GAME_OVER;
    }
}

void GameManager::ShowInventory() {
    system("cls");
    std::cout << "===== 背包 =====" << std::endl;
    m_inventory->listAllItems();
    std::cout << "1. 使用物品\n2. 返回探索\n请选择: ";
    std::string choice;
    std::cin >> choice;

    if (choice == "1") {
        std::string itemName;
        std::cout << "请输入要使用的物品名称: ";
        std::cin >> itemName;
        auto item = m_inventory->takeItem(itemName);
        if (item) {
            UseItem(item);
        }
        else {
            std::cout << "没有该物品" << std::endl;
            system("pause");
        }
    }
    m_currentState = GameState::EXPLORING;
}

void GameManager::UseItem(std::shared_ptr<Item> item) {
    if (!item) return;
    if (item->getType() == ItemType::POTION) {
        int heal = item->getAttribute(AttributeType::HP);
        float newHp = m_player->GetHP() + heal;
        m_player->SetHP(newHp);
        std::cout << "使用" << item->getName() << "恢复了" << heal << "点生命值!" << std::endl;
    }
    else if (item->getType() == ItemType::WEAPON) {
        int attack = item->getAttribute(AttributeType::ATTACK);
        int agility = item->getAttribute(AttributeType::AGILITY);
        if (agility > 0) {
            auto weapon = std::make_unique<Weapon>(item->getName(), EquipmentType::KNIFE, attack, agility);
            m_player->EquipKnife(std::move(weapon));
        }
        else {
            auto weapon = std::make_unique<Weapon>(item->getName(), EquipmentType::GUN, attack);
            m_player->EquipGun(std::move(weapon));
        }
        std::cout << "装备了" << item->getName() << std::endl;
    }
    else if (item->getType() == ItemType::ARMOR) {
        int defense = item->getAttribute(AttributeType::DEFENSE);
        auto armor = std::make_unique<Armor>(item->getName(), defense);
        m_player->EquipArmor(std::move(armor));
        std::cout << "装备了" << item->getName() << std::endl;
    }
    system("pause");
}

void GameManager::VisitStore() {
    Store store;
    store.ShowMenu(*m_player, *m_inventory);
}

void GameManager::MoveToMap() {
    system("cls");
    std::cout << "===== 切换地图 =====" << std::endl;
    std::cout << "当前位置: " << m_currentMap->GetName() << std::endl;
    m_currentMap->ShowConnectedMaps();
    std::cout << "请选择要前往的地图 (输入0返回): ";
    std::string choice;
    std::cin >> choice;

    if (choice != "0") {
        try {
            int mapIndex = std::stoi(choice);
            if (mapIndex > 0 && mapIndex <= static_cast<int>(m_currentMap->GetConnectedMaps().size())) {
                std::string mapName = m_currentMap->GetConnectedMaps()[mapIndex - 1];
                m_currentMap = m_maps[mapName].get();
                std::cout << "你现在在 " << mapName << std::endl;
            }
            else {
                std::cout << "无效的地图选择" << std::endl;
            }
        }
        catch (...) {
            std::cout << "请输入有效的数字" << std::endl;
        }
    }
    else {
        std::cout << "取消。" << std::endl;
    }
    system("pause");
}

void GameManager::GameOver() {
    system("cls");
    std::cout << "===== 游戏结束 =====" << std::endl;
    std::cout << "你已经阵亡" << std::endl;
    std::cout << "等级: " << m_player->GetLevel() << std::endl;
    std::cout << "1. 重新开始\n2. 退出游戏\n请选择: ";
    std::string choice;
    std::cin >> choice;

    if (choice == "1") {
        std::string name = m_player->GetName();
        m_player = std::make_unique<CPlayer>(name);
        m_currentMap = m_maps["森林"].get();
        m_currentState = GameState::EXPLORING;
        // 清空背包
        m_inventory = std::make_unique<Storage>();
        std::cout << "重新开始成功，回到森林!" << std::endl;
        system("pause");
    }
    else if (choice == "2") {
        m_running = false;
    }
    else {
        std::cout << "无效选择" << std::endl;
        system("pause");
    }
}

// 存档处理（自动使用默认文件名）
void GameManager::ShowSaveMenu() {
    system("cls");
    std::cout << "===== 存档管理 =====" << std::endl;

    // 直接使用默认文件名存档，无需用户输入
    if (SaveGame(DEFAULT_SAVE_FILE)) {
        std::cout << "游戏保存成功！" << std::endl;
    }
    else {
        std::cout << "游戏保存失败！" << std::endl;
    }

    system("pause");
    m_currentState = GameState::EXPLORING;
}

// 保存游戏（使用默认文件名）
bool GameManager::SaveGame(const std::string& filename) {
    try {
        std::ofstream file(filename + ".sav");
        if (!file.is_open()) return false;

        // 保存玩家信息
        file << "PLAYER\n";
        file << m_player->GetName() << "\n";
        file << m_player->GetLevel() << "\n";
        file << m_player->GetExp() << "\n";
        file << m_player->GetGold() << "\n";
        file << m_player->GetHP() << "\n";
        file << m_player->GetRawAttack() << "\n";
        file << m_player->GetRawDefense() << "\n";
        file << m_player->GetRawAgility() << "\n";

        // 保存当前地图
        file << "MAP\n";
        file << m_currentMap->GetName() << "\n";

        // 保存背包物品数量
        auto& items = m_inventory->getAllItems();
        file << "INVENTORY\n";
        file << items.size() << "\n";
        for (const auto& entry : items) {
            for (const auto& item : entry.second) {
                file << item->getName() << "\n";
                file << item->getDescription() << "\n";
                file << static_cast<int>(item->getType()) << "\n";
                auto& attrs = item->getAllAttributes();
                file << attrs.size() << "\n";
                for (const auto& attr : attrs) {
                    file << static_cast<int>(attr.first) << "\n";
                    file << attr.second << "\n";
                }
            }
        }

        file.close();
        return true;
    }
    catch (...) {
        return false;
    }
}

// 加载游戏（使用默认文件名）
bool GameManager::LoadGame(const std::string& filename) {
    try {
        std::ifstream file(filename + ".sav");
        if (!file.is_open()) return false;

        std::string line;
        // 加载玩家信息
        std::getline(file, line); // 读取"PLAYER"标记
        if (line != "PLAYER") return false;

        std::string name;
        int level, exp, gold;
        float hp, attack, defense, agility;

        std::getline(file, name);
        file >> level >> exp >> gold >> hp >> attack >> defense >> agility;
        file.ignore(); // 忽略换行符

        m_player = std::make_unique<CPlayer>(name);
        m_player->RestoreToInitialState(); // 重置后再赋值
        m_player->AddExp(exp);
        m_player->AddGold(gold);
        m_player->SetHP(hp);
        // 通过LevelUp间接设置等级
        while (m_player->GetLevel() < level) {
            m_player->LevelUp();
        }

        // 加载当前地图
        std::getline(file, line); // 读取"MAP"标记
        if (line != "MAP") return false;

        std::string mapName;
        std::getline(file, mapName);
        m_currentMap = m_maps[mapName].get();

        // 加载背包物品
        std::getline(file, line); // 读取"INVENTORY"标记
        if (line != "INVENTORY") return false;

        m_inventory = std::make_unique<Storage>();
        int itemCount;
        file >> itemCount;
        file.ignore();

        for (int i = 0; i < itemCount; ++i) {
            std::string itemName, itemDesc;
            int type;
            std::getline(file, itemName);
            std::getline(file, itemDesc);
            file >> type;

            auto item = std::make_shared<Item>(itemName, itemDesc, static_cast<ItemType>(type));
            int attrCount;
            file >> attrCount;
            file.ignore();

            for (int j = 0; j < attrCount; ++j) {
                int attrType, attrValue;
                file >> attrType >> attrValue;
                file.ignore();
                item->setAttribute(static_cast<AttributeType>(attrType), attrValue);
            }
            m_inventory->storeItem(item);
        }

        file.close();
        return true;
    }
    catch (...) {
        return false;
    }
}