#include "game_manager.h"
#include <iostream>
#include <string>
#include <cstdlib>
#include <ctime>
#include <stdexcept>
#include <fstream>
#include <sstream>

GameManager::GameManager()
    : m_currentState(GameState::MAIN_MENU), m_currentMap(nullptr), m_running(true), m_hasReceivedSupplies(false) {
    m_inventory = std::make_unique<Storage>();
    InitializeMaps();
    std::srand(static_cast<unsigned int>(std::time(nullptr)));
    // 初始化BOSS状态
    m_bossDefeated["洞穴魔王"] = false;
    m_bossDefeated["沙漠暴君"] = false;
}

void GameManager::InitializeMaps() {
    m_maps["村庄"] = std::make_unique<Map>("村庄");
    m_maps["森林"] = std::make_unique<Map>("森林");
    m_maps["山洞"] = std::make_unique<Map>("山洞", true);
    m_maps["商店"] = std::make_unique<Map>("商店");
    m_maps["沙漠"] = std::make_unique<Map>("沙漠", true);

    m_maps["村庄"]->AddConnectedMap("森林");

    m_maps["森林"]->AddConnectedMap("村庄");
    m_maps["森林"]->AddConnectedMap("山洞");

    m_maps["山洞"]->AddConnectedMap("森林");
    m_maps["山洞"]->AddConnectedMap("沙漠");

    m_maps["商店"]->AddConnectedMap("村庄");

    m_maps["沙漠"]->AddConnectedMap("山洞");
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
    std::cout << "==================================================" << std::endl;
    std::cout << "======= 欢迎来到村庄守护者：暴君讨伐传 =========" << std::endl;
    std::cout << "==================================================" << std::endl;
    std::cout << std::endl;

    std::cout << "【故事背景】" << std::endl;
    std::cout << "你生活在一个被黄沙环绕的古老村落——绿洲村。" << std::endl;
    std::cout << "这里曾经水草丰美，村民安居乐业，但这一切都被沙漠暴君改变了。" << std::endl;
    std::cout << std::endl;

    std::cout << "三年前，沙漠暴君苏醒，它召唤的沙尘暴席卷了整个地区。" << std::endl;
    std::cout << "庄稼枯萎，水源干涸，村民们的生活陷入了前所未有的困境。" << std::endl;
    std::cout << "每年都有勇敢的村民前去讨伐，但无人归还..." << std::endl;
    std::cout << std::endl;

    std::cout << "经过层层选拔，你——" << std::endl;
    std::cout << "村中最强的战士，被选为讨伐沙漠暴君的最后希望。" << std::endl;
    std::cout << "背负着全村人的期望，你踏上了这条充满艰险的征途。" << std::endl;
    std::cout << std::endl;

    std::cout << "你的使命：穿越危险的区域，击败沙漠暴君，为村庄带来和平！" << std::endl;
    std::cout << "==================================================" << std::endl;
    std::cout << std::endl;
    std::cout << "1. 开始游戏\n2. 加载存档\n3. 退出游戏\n请选择: ";
    int choice;
    std::cin >> choice;
    if (std::cin.fail()) {
        std::cin.clear();
        std::cin.ignore(1024, '\n');
        std::cout << "无效选项" << std::endl;
        system("pause");
        return;
    }
    if (choice == 1) {
        std::string name;
        std::cout << "请输入你的名字: ";
        std::cin >> name;
        m_player = std::make_unique<CPlayer>(name);
        m_currentMap = m_maps["村庄"].get();
        m_currentMapName = "村庄";
        m_player->SetHP(m_player->GetMaxHP());
        std::cout << std::endl;
        std::cout << "==================================================" << std::endl;
        std::cout << "勇敢的" << name << "，你的传奇就此开始！" << std::endl;
        std::cout << "村民们为你准备了基础的装备和补给。" << std::endl;
        std::cout << "首先在村庄中熟悉环境，然后踏上讨伐之路吧！" << std::endl;
        std::cout << "==================================================" << std::endl;
        system("pause");
        m_currentState = GameState::EXPLORING;
    }
    else if (choice == 2) {
        if (LoadGame("savegame.dat")) {
            m_currentMap = m_maps[m_currentMapName].get();
            m_currentState = GameState::EXPLORING;
            std::cout << "存档加载成功！" << std::endl;
            system("pause");
        }
        else {
            std::cout << "加载存档失败！" << std::endl;
            system("pause");
        }
    }
    else if (choice == 3) {
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
        std::cout << "3. 村民补给站" << std::endl;
        std::cout << "4. 查看状态" << std::endl;
        std::cout << "5. 查看背包" << std::endl;
        std::cout << "6. 保存游戏" << std::endl;
        std::cout << "7. 退出游戏" << std::endl;

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
        case 2:
            // 直接切换到商店地图，而不是通过 MoveToMap()
            m_currentMap = m_maps["商店"].get();
            std::cout << "已进入商店" << std::endl;
            system("pause");
            break;
        case 3: VisitVillageSupply(); break;
        case 4: m_player->ShowStatus(); system("pause"); break;
        case 5: m_currentState = GameState::INVENTORY; break;
        case 6:
            if (SaveGame("savegame.dat")) {
                std::cout << "游戏保存成功！" << std::endl;
            }
            else {
                std::cout << "保存游戏失败！" << std::endl;
            }
            system("pause");
            break;
        case 7: m_running = false; break;
        default: std::cout << "无效选项" << std::endl; system("pause");
        }
        return;
    }

    if (m_currentMap->GetName() == "商店") {
        std::cout << "1. 购买商品" << std::endl;
        std::cout << "2. 返回村庄" << std::endl;
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
        case 2: m_currentMap = m_maps["村庄"].get();
            std::cout << "已返回村庄" << std::endl;
            system("pause");
            break;
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
    std::cout << "6. 保存游戏" << std::endl;
    std::cout << "7. 退出游戏" << std::endl;

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
    case 6:
        if (SaveGame("savegame.dat")) {
            std::cout << "游戏保存成功！" << std::endl;
        }
        else {
            std::cout << "保存游戏失败！" << std::endl;
        }
        system("pause");
        break;
    case 7: m_running = false; break;
    default: std::cout << "无效选项" << std::endl; system("pause");
    }
}

void GameManager::VisitVillageSupply() {
    system("cls");
    std::cout << "===== 村民补给站 =====" << std::endl;

    if (CheckSuppliesReceived()) {
        std::cout << "老村长: \"你已经领取过补给品了，好好利用它们讨伐沙漠暴君吧！\"" << std::endl;
        system("pause");
        return;
    }

    std::cout << "老村长: \"欢迎你，勇敢的战士！村民们为你准备了一些补给品。\"" << std::endl;
    std::cout << "老村长: \"这是村民们凑出来的装备和药水，希望能帮到你：\"" << std::endl;
    std::cout << "- 粗糙的铁剑 x1" << std::endl;
    std::cout << "- 皮质护甲 x1" << std::endl;
    std::cout << "- 强效药水 x2" << std::endl;
    std::cout << std::endl;
    std::cout << "是否领取补给？(1.领取 2.暂时不要): ";

    int choice;
    std::cin >> choice;

    if (choice == 1) {
        // 创建并添加铁剑
        auto sword = std::make_shared<Item>("粗糙的铁剑", "攻击+3", ItemType::WEAPON, EquipmentType::MELEE_WEAPON);
        sword->setAttribute(AttributeType::ATTACK, 3);
        m_inventory->storeItem(sword);

        // 创建并添加皮质护甲
        auto armor = std::make_shared<Item>("皮质护甲", "防御+2", ItemType::ARMOR, EquipmentType::ARMOR);
        armor->setAttribute(AttributeType::DEFENSE, 2);
        m_inventory->storeItem(armor);

        // 创建并添加2个药水
        auto potion1 = std::make_shared<Item>("强效药水", "恢复50点生命值", ItemType::POTION);
        potion1->setAttribute(AttributeType::HP, 50);
        m_inventory->storeItem(potion1);

        auto potion2 = std::make_shared<Item>("强效药水", "恢复50点生命值", ItemType::POTION);
        potion2->setAttribute(AttributeType::HP, 50);
        m_inventory->storeItem(potion2);

        MarkSuppliesReceived();

        std::cout << std::endl;
        std::cout << "老村长: \"愿沙漠之神保佑你，勇敢的战士！\"" << std::endl;
        std::cout << "你获得了补给品，已放入背包中。" << std::endl;
    }
    else {
        std::cout << "老村长: \"如果需要补给，随时可以来找我。\"" << std::endl;
    }

    system("pause");
}

bool GameManager::CheckSuppliesReceived() {
    return m_hasReceivedSupplies;
}

void GameManager::MarkSuppliesReceived() {
    m_hasReceivedSupplies = true;
}

void GameManager::EncounterEnemy(bool isBoss) {
    system("cls");
    if (isBoss) {
        std::string bossName;
        if (m_currentMap->GetName() == "山洞") {
            bossName = "洞穴魔王";
        }
        else if (m_currentMap->GetName() == "沙漠") {
            bossName = "沙漠暴君";
        }
        // 检查BOSS是否已被击败
        if (m_bossDefeated[bossName]) {
            std::cout << bossName << " 已经被击败，不会再次出现！" << std::endl;
            system("pause");
            return;
        }
        std::cout << "你遇到了BOSS！" << std::endl;
        std::unique_ptr<CEnemy> enemy;
        if (m_currentMap->GetName() == "山洞") {
            enemy = std::make_unique<CEnemy>("洞穴魔王", 1.0f, 30.0f, 10.0f, 10.0f,
                "魔王冲击", 2.0f, "造成巨大伤害");
        }
        else if (m_currentMap->GetName() == "沙漠") {
            enemy = std::make_unique<CEnemy>("沙漠暴君", 250.0f, 35.0f, 12.0f, 8.0f,
                "沙尘暴", 2.5f, "召唤沙尘暴造成伤害并削减防御力", 2.0f);
        }
        enemy->ShowStatus();
        std::cout << "准备战斗..." << std::endl;
        system("pause");
        CFightSystem fightSystem(std::move(m_player), std::move(enemy), *m_inventory);
        fightSystem.StartFight();
        m_player = fightSystem.releasePlayer();
        if (!m_player->IsAlive()) {
            m_currentState = GameState::GAME_OVER;
        }
        else {
            // 玩家存活，说明击败了BOSS
            m_bossDefeated[bossName] = true;
            // 处理BOSS掉落
            std::string dropItem = fightSystem.GetDropItem();
            if (!dropItem.empty() && dropItem == "魔王战斧") {
                // 创建魔王战斧并添加到背包
                auto axe = std::make_shared<Item>("魔王战斧", "攻击+20 防御+5 敏捷-3", ItemType::WEAPON, EquipmentType::MELEE_WEAPON);
                axe->setAttribute(AttributeType::ATTACK, 20);
                axe->setAttribute(AttributeType::DEFENSE, 5);
                axe->setAttribute(AttributeType::AGILITY, -3);
                m_inventory->storeItem(axe);
                std::cout << "获得了魔王战斧！\n";
                system("pause");
            }
        }
        if (!m_player->IsAlive()) m_currentState = GameState::GAME_OVER;
    }
    else {
        std::cout << "你遇到了一只小怪！" << std::endl;
        std::unique_ptr<CEnemy> enemy;
        if (m_currentMap->GetName() == "森林") {
            enemy = std::make_unique<CEnemy>("野猪", 20.0f, 5.0f, 2.0f, 1.0f);
        }
        else if (m_currentMap->GetName() == "山洞") {
            enemy = std::make_unique<CEnemy>("巨型蝙蝠", 25.0f, 6.0f, 1.0f, 3.0f); // 修改为巨型蝙蝠
        }
        else if (m_currentMap->GetName() == "沙漠") {
            enemy = std::make_unique<CEnemy>("剧毒沙蝎", 25.0f, 7.0f, 3.0f, 2.0f);
        }

        enemy->ShowStatus();
        std::cout << "准备战斗..." << std::endl;
        system("pause");
        CFightSystem fightSystem(std::move(m_player), std::move(enemy), *m_inventory);
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

        // 使用明确的装备类型判断
        if (item->getEquipmentType() == EquipmentType::RANGED_WEAPON) {
            // 远程武器
            if (m_player->HasRangedWeapon()) {
                std::cout << "已装备远程武器 [" << m_player->GetRangedWeapon()->GetName()
                    << "]，是否替换？(1.是 2.否): ";
                int choice;
                std::cin >> choice;
                if (choice != 1) {
                    m_inventory->storeItem(item);
                    std::cout << "取消装备。" << std::endl;
                    system("pause");
                    return;
                }

                // 将原来的远程武器放回背包
                auto oldWeapon = std::make_shared<Item>(
                    m_player->GetRangedWeapon()->GetName(),
                    "已装备的远程武器",
                    ItemType::WEAPON,
                    EquipmentType::RANGED_WEAPON
                );
                oldWeapon->setAttribute(AttributeType::ATTACK, m_player->GetRangedWeapon()->GetAttackBonus());
                oldWeapon->setAttribute(AttributeType::AGILITY, m_player->GetRangedWeapon()->GetAgilityBonus());
                m_inventory->storeItem(oldWeapon);
            }

            auto weapon = std::make_unique<Weapon>(item->getName(), EquipmentType::RANGED_WEAPON, attack, agility);
            m_player->EquipRangedWeapon(std::move(weapon));
            std::cout << "装备了远程武器: " << item->getName() << std::endl;
        }
        else {
            // 近战武器
            if (m_player->HasMeleeWeapon()) {
                std::cout << "已装备近战武器 [" << m_player->GetMeleeWeapon()->GetName()
                    << "]，是否替换？(1.是 2.否): ";
                int choice;
                std::cin >> choice;
                if (choice != 1) {
                    m_inventory->storeItem(item);
                    std::cout << "取消装备。" << std::endl;
                    system("pause");
                    return;
                }

                // 将原来的近战武器放回背包
                auto oldWeapon = std::make_shared<Item>(
                    m_player->GetMeleeWeapon()->GetName(),
                    "已装备的近战武器",
                    ItemType::WEAPON,
                    EquipmentType::MELEE_WEAPON
                );
                oldWeapon->setAttribute(AttributeType::ATTACK, m_player->GetMeleeWeapon()->GetAttackBonus());
                oldWeapon->setAttribute(AttributeType::AGILITY, m_player->GetMeleeWeapon()->GetAgilityBonus());
                m_inventory->storeItem(oldWeapon);
            }

            auto weapon = std::make_unique<Weapon>(item->getName(), EquipmentType::MELEE_WEAPON, attack, agility);
            m_player->EquipMeleeWeapon(std::move(weapon));
            std::cout << "装备了近战武器: " << item->getName() << std::endl;
        }
    }
    else if (item->getType() == ItemType::ARMOR) {
        int defense = item->getAttribute(AttributeType::DEFENSE);

        if (m_player->HasArmor()) {
            std::cout << "已装备护甲 [" << m_player->GetArmor()->GetName()
                << "]，是否替换？(1.是 2.否): ";
            int choice;
            std::cin >> choice;
            if (choice != 1) {
                m_inventory->storeItem(item);
                std::cout << "取消装备。" << std::endl;
                system("pause");
                return;
            }

            // 将原来的护甲放回背包
            auto oldArmor = std::make_shared<Item>(
                m_player->GetArmor()->GetName(),
                "已装备的护甲",
                ItemType::ARMOR
            );
            oldArmor->setAttribute(AttributeType::DEFENSE, m_player->GetArmor()->GetDefenseBonus());
            m_inventory->storeItem(oldArmor);
        }

        auto armor = std::make_unique<Armor>(item->getName(), defense);
        m_player->EquipArmor(std::move(armor));
        std::cout << "装备了护甲: " << item->getName() << std::endl;
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
        m_hasReceivedSupplies = false;
        // 重置BOSS状态
        m_bossDefeated["洞穴魔王"] = false;
        m_bossDefeated["沙漠暴君"] = false;
        std::cout << "你已轮回重生，背包已清空，回到村庄。" << std::endl;
        system("pause");
    }
    else {
        m_running = false;
    }
}

bool GameManager::SaveGame(const std::string& filename) {
    std::ofstream file(filename);
    if (!file.is_open()) return false;

    try {
        // 玩家数据
        file << m_player->GetName() << "\n";
        file << m_player->GetLevel() << "\n";
        file << m_player->GetExp() << "\n";
        file << m_player->GetGold() << "\n";
        file << m_player->GetHP() << "\n";
        file << m_player->GetMaxHP() << "\n";
        file << m_player->GetRawAttack() << "\n";
        file << m_player->GetRawDefense() << "\n";
        file << m_player->GetRawAgility() << "\n";

        // 当前地图
        file << m_currentMap->GetName() << "\n";

        file << (m_hasReceivedSupplies ? "1" : "0") << "\n";

        // 保存BOSS击败状态
        file << m_bossDefeated["洞穴魔王"] << "\n";
        file << m_bossDefeated["沙漠暴君"] << "\n";

        // 保存背包物品
        int totalItemCount = 0;
        for (const auto& entry : m_inventory->GetAllItems()) {
            totalItemCount += entry.second.size();
        }
        file << totalItemCount << "\n";

        // 保存每个物品的详细信息
        for (const auto& entry : m_inventory->GetAllItems()) {
            for (const auto& item : entry.second) {
                file << item->getName() << "\n";
                file << item->getDescription() << "\n";
                file << static_cast<int>(item->getType()) << "\n";
                file << static_cast<int>(item->getEquipmentType()) << "\n"; // 新增：保存装备类型

                // 保存属性
                auto attributes = item->getAllAttributes();
                file << attributes.size() << "\n";
                for (const auto& attr : attributes) {
                    file << static_cast<int>(attr.first) << "\n";
                    file << attr.second << "\n";
                }
            }
        }

    }
    catch (const std::exception& e) {
        std::cerr << "保存存档错误: " << e.what() << std::endl;
        file.close();
        return false;
    }
    catch (...) {
        file.close();
        return false;
    }

    file.close();
    return true;
}

bool GameManager::LoadGame(const std::string& filename) {
    std::ifstream file(filename);
    if (!file.is_open()) return false;

    try {
        std::string name;
        int level, exp, gold;
        float hp, maxhp, attack, defense, agility;
        std::string suppliesFlag;
        bool caveBossDefeated, desertBossDefeated;

        std::getline(file, name);
        file >> level >> exp >> gold >> hp >> maxhp >> attack >> defense >> agility;
        file.ignore(); // 消耗换行符

        std::getline(file, m_currentMapName);
        std::getline(file, suppliesFlag);

        // 加载BOSS击败状态
        file >> caveBossDefeated;
        file.ignore();
        file >> desertBossDefeated;
        file.ignore();

        m_bossDefeated["洞穴魔王"] = caveBossDefeated;
        m_bossDefeated["沙漠暴君"] = desertBossDefeated;


        // 加载背包物品数量
        int totalItemCount;
        file >> totalItemCount;
        file.ignore();

        // 清空当前背包
        m_inventory = std::make_unique<Storage>();

        // 加载每个物品
        for (int i = 0; i < totalItemCount; i++) {
            std::string itemName, itemDesc;
            int itemType, attrCount;

            std::getline(file, itemName);
            std::getline(file, itemDesc);
            file >> itemType;
            file.ignore();
            int equipmentType;
            file >> equipmentType;
            file.ignore();

            // 修复：移除重复的 item 创建
            auto item = std::make_shared<Item>(itemName, itemDesc, static_cast<ItemType>(itemType));

            file >> attrCount;
            file.ignore();

            for (int j = 0; j < attrCount; j++) {
                int attrType, attrValue;
                file >> attrType;
                file.ignore();
                file >> attrValue;
                file.ignore();

                item->setAttribute(static_cast<AttributeType>(attrType), attrValue);
            }

            m_inventory->storeItem(item);
        }

        m_player = std::make_unique<CPlayer>(name);
        m_player->SetLevel(level);
        m_player->SetExp(exp);
        m_player->SetGold(gold);
        m_player->SetMaxHP(maxhp);
        m_player->SetHP(hp);
        m_player->SetBaseStats(attack, defense, agility);

        m_currentMap = m_maps[m_currentMapName].get();
        m_hasReceivedSupplies = (suppliesFlag == "1");

    }
    catch (const std::exception& e) {
        std::cerr << "加载存档错误: " << e.what() << std::endl;
        file.close();
        return false;
    }
    catch (...) {
        std::cerr << "未知加载错误" << std::endl;
        file.close();
        return false;
    }

    file.close();
    return true;
}