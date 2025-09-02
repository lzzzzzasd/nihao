#include "game_manager.h"
#include <iostream>
#include <string>
#include <cstdlib>
#include <ctime>
#include <stdexcept>

GameManager::GameManager()
    : m_currentState(GameState::MAIN_MENU), m_currentMap(nullptr), m_running(true) {
    m_inventory = std::make_unique<Storage>();
    InitializeMaps();
    std::srand(static_cast<unsigned int>(std::time(nullptr)));
}

void GameManager::InitializeMaps() {
    m_maps["村庄"] = std::make_unique<Map>("村庄");
    m_maps["森林"] = std::make_unique<Map>("森林");
    m_maps["山洞"] = std::make_unique<Map>("山洞", true);
    m_maps["商店"] = std::make_unique<Map>("商店");

    m_maps["村庄"]->AddConnectedMap("森林");
    m_maps["村庄"]->AddConnectedMap("商店");

    m_maps["森林"]->AddConnectedMap("村庄");
    m_maps["森林"]->AddConnectedMap("山洞");

    m_maps["山洞"]->AddConnectedMap("森林");

    m_maps["商店"]->AddConnectedMap("村庄");
}

void GameManager::Run() {
    while (m_running) {
        switch (m_currentState) {
        case GameState::MAIN_MENU: ShowMainMenu(); break;
        case GameState::EXPLORING: ExploreArea(); break;
        case GameState::INVENTORY: ShowInventory(); break;
        case GameState::STORE: VisitStore(); m_currentState = GameState::EXPLORING; break;
        case GameState::GAME_OVER: GameOver(); break;
        default: m_currentState = GameState::MAIN_MENU;
        }
    }
}

void GameManager::ShowMainMenu() {
    system("cls");
    std::cout << "===== 欢迎来到MUD冒险世界 =====" << std::endl;
    std::cout << "1. 开始游戏\n2. 退出游戏\n请选择: ";
    int choice;
    std::cin >> choice;
    if (std::cin.fail()) {
        std::cin.clear();
        std::cin.ignore(1024, '\n');
        std::cout << "无效选项" << std::endl;
        system("pause");
        return; // 返回重新显示主菜单
    }
    if (choice == 1) {
        std::string name;
        std::cout << "请输入你的名字: ";
        std::cin >> name;
        m_player = std::make_unique<CPlayer>(name);
        m_currentMap = m_maps["村庄"].get();
        m_player->SetHP(m_player->GetMaxHP());
        m_currentState = GameState::EXPLORING;
    }
    else if (choice == 2) {
        m_running = false;
    }
    else {
        std::cout << "无效选项" << std::endl;
        system("pause");
    }
}


void GameManager::ExploreArea() {
    system("cls");
    std::cout << "===== " << m_currentMap->GetName() << " =====" << std::endl;
    std::cout << "当前位置: " << m_currentMap->GetName() << std::endl;

    int choice = 0;
    if (m_currentMap->GetName() == "村庄") {
        std::cout << "这是一个安静的村庄，你只能去其他区域或商店。" << std::endl;
        std::cout << "1. 前往其他区域" << std::endl;
        std::cout << "2. 进入商店" << std::endl;
        std::cout << "3. 查看状态" << std::endl;
        std::cout << "4. 查看背包" << std::endl;
        std::cout << "5. 退出游戏" << std::endl;

        std::cin >> choice;
        if (std::cin.fail()) {
            std::cin.clear();
            std::cin.ignore(1024, '\n');
            std::cout << "无效选项" << std::endl;
            system("pause");
            return;
        }
        switch (choice) {
        case 1: MoveToMap(); break;
        case 2: m_currentState = GameState::STORE; break;
        case 3: m_player->ShowStatus(); system("pause"); break;
        case 4: m_currentState = GameState::INVENTORY; break;
        case 5: m_running = false; break;
        default: std::cout << "无效选项" << std::endl; system("pause");
        }
        return;
    }

    if (m_currentMap->GetName() == "商店") {
        std::cout << "你在商店，可以探索或遇到BOSS。" << std::endl;
        std::cout << "1. 购买商品" << std::endl;
        std::cout << "2. 前往其他区域" << std::endl;
        std::cout << "3. 查看状态" << std::endl;
        std::cout << "4. 查看背包" << std::endl;
        std::cout << "5. 退出游戏" << std::endl;
        std::cin >> choice;
        if (std::cin.fail()) {
            std::cin.clear();
            std::cin.ignore(1024, '\n');
            std::cout << "无效选项" << std::endl;
            system("pause");
            return;
        }
        switch (choice) {
        case 1: m_currentState = GameState::STORE; break;
        case 2: MoveToMap(); break;
        case 3: m_player->ShowStatus(); system("pause"); break;
        case 4: m_currentState = GameState::INVENTORY; break;
        case 5: m_running = false; break;
        default: std::cout << "无效选项" << std::endl; system("pause");
        }
        return;
    }

    std::cout << "请选择探索方式" << std::endl;
    std::cout << "1. 小怪战斗" << std::endl;
    std::cout << "2. BOSS战斗" << std::endl;
    std::cout << "3. 查看状态" << std::endl;
    std::cout << "4. 查看背包" << std::endl;
    std::cout << "5. 前往其他区域" << std::endl;
    std::cout << "6. 退出游戏" << std::endl;

    std::cin >> choice;
    if (std::cin.fail()) {
        std::cin.clear();
        std::cin.ignore(1024, '\n');
        std::cout << "无效选项" << std::endl;
        system("pause");
        return;
    }
    switch (choice) {
    case 1: EncounterEnemy(false); break;
    case 2:
        if (m_currentMap->HasBoss()) EncounterEnemy(true);
        else { std::cout << "当前地图没有BOSS" << std::endl; system("pause"); }
        break;
    case 3: m_player->ShowStatus(); system("pause"); break;
    case 4: m_currentState = GameState::INVENTORY; break;
    case 5: MoveToMap(); break;
    case 6: m_running = false; break;
    default: std::cout << "无效选项" << std::endl; system("pause");
    }
}

void GameManager::EncounterEnemy(bool isBoss) {
    system("cls");
    if (isBoss) {
        std::cout << "你遇到了BOSS！" << std::endl;
        auto enemy = std::make_unique<CEnemy>("洞穴魔王", 200.0f, 30.0f, 10.0f, 10.0f, "魔王冲击", 2.0f, "造成巨大伤害");
        enemy->ShowStatus();
        std::cout << "准备战斗..." << std::endl;
        system("pause");
        CFightSystem fightSystem(std::move(m_player), std::move(enemy));
        fightSystem.StartFight();
        m_player = fightSystem.releasePlayer();
        if (!m_player->IsAlive()) m_currentState = GameState::GAME_OVER;
    }
    else {
        std::cout << "你遇到了一只小怪！" << std::endl;
        auto enemy = std::make_unique<CEnemy>("野兽", 20.0f, 5.0f, 2.0f, 1.0f);
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
    int choice;
    std::cin >> choice;
    if (std::cin.fail()) {
        std::cin.clear();
        std::cin.ignore(1024, '\n');
        std::cout << "无效选项" << std::endl;
        system("pause");
        m_currentState = GameState::EXPLORING;
        return;
    }
    if (choice == 1) {
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
        std::cout << "使用" << item->getName() << "，恢复" << heal << "生命值。" << std::endl;
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
    std::cout << "请选择要前往的地图 (0返回): ";
    int choice;
    std::cin >> choice;
    if (choice > 0 && choice <= static_cast<int>(m_currentMap->GetConnectedMaps().size())) {
        std::string mapName = m_currentMap->GetConnectedMaps()[choice - 1];
        m_currentMap = m_maps[mapName].get();
        std::cout << "已前往 " << mapName << std::endl;
    }
    else {
        std::cout << "返回。" << std::endl;
    }
    system("pause");
}

void GameManager::GameOver() {
    system("cls");
    std::cout << "===== 游戏结束 =====" << std::endl;
    std::cout << "你已经死亡。" << std::endl;
    std::cout << "等级: " << m_player->GetLevel() << std::endl;
    std::cout << "1. 重新轮回\n2. 退出游戏\n请选择: ";
    int choice;
    std::cin >> choice;
    if (choice == 1) {
        std::string name = m_player->GetName();
        m_player = std::make_unique<CPlayer>(name);
        m_currentMap = m_maps["村庄"].get();
        m_currentState = GameState::EXPLORING;
        // 清空背包
        m_inventory = std::make_unique<Storage>();
        std::cout << "你已轮回重生，背包已清空，回到村庄。" << std::endl;
        system("pause");
    }
    else {
        m_running = false;
    }
}