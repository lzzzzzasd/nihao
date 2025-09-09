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
    // ��ʼ��BOSS״̬
    m_bossDefeated["��Ѩħ��"] = false;
    m_bossDefeated["ɳĮ����"] = false;
}

void GameManager::InitializeMaps() {
    m_maps["��ׯ"] = std::make_unique<Map>("��ׯ");
    m_maps["ɭ��"] = std::make_unique<Map>("ɭ��");
    m_maps["ɽ��"] = std::make_unique<Map>("ɽ��", true);
    m_maps["�̵�"] = std::make_unique<Map>("�̵�");
    m_maps["ɳĮ"] = std::make_unique<Map>("ɳĮ", true);

    m_maps["��ׯ"]->AddConnectedMap("ɭ��");

    m_maps["ɭ��"]->AddConnectedMap("��ׯ");
    m_maps["ɭ��"]->AddConnectedMap("ɽ��");

    m_maps["ɽ��"]->AddConnectedMap("ɭ��");
    m_maps["ɽ��"]->AddConnectedMap("ɳĮ");

    m_maps["�̵�"]->AddConnectedMap("��ׯ");

    m_maps["ɳĮ"]->AddConnectedMap("ɽ��");
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
    std::cout << "======= ��ӭ������ׯ�ػ��ߣ������ַ��� =========" << std::endl;
    std::cout << "==================================================" << std::endl;
    std::cout << std::endl;

    std::cout << "�����±�����" << std::endl;
    std::cout << "��������һ������ɳ���ƵĹ��ϴ��䡪�����޴塣" << std::endl;
    std::cout << "��������ˮ�ݷ��������񰲾���ҵ������һ�ж���ɳĮ�����ı��ˡ�" << std::endl;
    std::cout << std::endl;

    std::cout << "����ǰ��ɳĮ�������ѣ����ٻ���ɳ����ϯ��������������" << std::endl;
    std::cout << "ׯ�ڿ�ή��ˮԴ�ɺԣ������ǵ�����������ǰ��δ�е�������" << std::endl;
    std::cout << "ÿ�궼���¸ҵĴ���ǰȥ�ַ��������˹黹..." << std::endl;
    std::cout << std::endl;

    std::cout << "�������ѡ�Σ��㡪��" << std::endl;
    std::cout << "������ǿ��սʿ����ѡΪ�ַ�ɳĮ���������ϣ����" << std::endl;
    std::cout << "������ȫ���˵���������̤���������������յ���;��" << std::endl;
    std::cout << std::endl;

    std::cout << "���ʹ������ԽΣ�յ����򣬻���ɳĮ������Ϊ��ׯ������ƽ��" << std::endl;
    std::cout << "==================================================" << std::endl;
    std::cout << std::endl;
    std::cout << "1. ��ʼ��Ϸ\n2. ���ش浵\n3. �˳���Ϸ\n��ѡ��: ";
    int choice;
    std::cin >> choice;
    if (std::cin.fail()) {
        std::cin.clear();
        std::cin.ignore(1024, '\n');
        std::cout << "��Чѡ��" << std::endl;
        system("pause");
        return;
    }
    if (choice == 1) {
        std::string name;
        std::cout << "�������������: ";
        std::cin >> name;
        m_player = std::make_unique<CPlayer>(name);
        m_currentMap = m_maps["��ׯ"].get();
        m_currentMapName = "��ׯ";
        m_player->SetHP(m_player->GetMaxHP());
        std::cout << std::endl;
        std::cout << "==================================================" << std::endl;
        std::cout << "�¸ҵ�" << name << "����Ĵ���ʹ˿�ʼ��" << std::endl;
        std::cout << "������Ϊ��׼���˻�����װ���Ͳ�����" << std::endl;
        std::cout << "�����ڴ�ׯ����Ϥ������Ȼ��̤���ַ�֮·�ɣ�" << std::endl;
        std::cout << "==================================================" << std::endl;
        system("pause");
        m_currentState = GameState::EXPLORING;
    }
    else if (choice == 2) {
        if (LoadGame("savegame.dat")) {
            m_currentMap = m_maps[m_currentMapName].get();
            m_currentState = GameState::EXPLORING;
            std::cout << "�浵���سɹ���" << std::endl;
            system("pause");
        }
        else {
            std::cout << "���ش浵ʧ�ܣ�" << std::endl;
            system("pause");
        }
    }
    else if (choice == 3) {
        m_running = false;
    }
    else {
        std::cout << "��Чѡ��" << std::endl;
        system("pause");
    }
}



void GameManager::ExploreArea() {
    system("cls");
    std::cout << "===== " << m_currentMap->GetName() << " =====" << std::endl;
    std::cout << "��ǰλ��: " << m_currentMap->GetName() << std::endl;

    int choice = 0;
    if (m_currentMap->GetName() == "��ׯ") {
        std::cout << "����һ�������Ĵ�ׯ����ֻ��ȥ����������̵ꡣ" << std::endl;
        std::cout << "1. ǰ����������" << std::endl;
        std::cout << "2. �����̵�" << std::endl;
        std::cout << "3. ���񲹸�վ" << std::endl;
        std::cout << "4. �鿴״̬" << std::endl;
        std::cout << "5. �鿴����" << std::endl;
        std::cout << "6. ������Ϸ" << std::endl;
        std::cout << "7. �˳���Ϸ" << std::endl;

        std::cin >> choice;
        if (std::cin.fail()) {
            std::cin.clear();
            std::cin.ignore(1024, '\n');
            std::cout << "��Чѡ��" << std::endl;
            system("pause");
            return;
        }
        switch (choice) {
        case 1: MoveToMap(); break;
        case 2:
            // ֱ���л����̵��ͼ��������ͨ�� MoveToMap()
            m_currentMap = m_maps["�̵�"].get();
            std::cout << "�ѽ����̵�" << std::endl;
            system("pause");
            break;
        case 3: VisitVillageSupply(); break;
        case 4: m_player->ShowStatus(); system("pause"); break;
        case 5: m_currentState = GameState::INVENTORY; break;
        case 6:
            if (SaveGame("savegame.dat")) {
                std::cout << "��Ϸ����ɹ���" << std::endl;
            }
            else {
                std::cout << "������Ϸʧ�ܣ�" << std::endl;
            }
            system("pause");
            break;
        case 7: m_running = false; break;
        default: std::cout << "��Чѡ��" << std::endl; system("pause");
        }
        return;
    }

    if (m_currentMap->GetName() == "�̵�") {
        std::cout << "1. ������Ʒ" << std::endl;
        std::cout << "2. ���ش�ׯ" << std::endl;
        std::cout << "3. �鿴״̬" << std::endl;
        std::cout << "4. �鿴����" << std::endl;
        std::cout << "5. �˳���Ϸ" << std::endl;
        std::cin >> choice;
        if (std::cin.fail()) {
            std::cin.clear();
            std::cin.ignore(1024, '\n');
            std::cout << "��Чѡ��" << std::endl;
            system("pause");
            return;
        }
        switch (choice) {
        case 1: m_currentState = GameState::STORE; break;
        case 2: m_currentMap = m_maps["��ׯ"].get();
            std::cout << "�ѷ��ش�ׯ" << std::endl;
            system("pause");
            break;
        case 3: m_player->ShowStatus(); system("pause"); break;
        case 4: m_currentState = GameState::INVENTORY; break;
        case 5: m_running = false; break;
        default: std::cout << "��Чѡ��" << std::endl; system("pause");
        }
        return;
    }

    std::cout << "��ѡ��̽����ʽ" << std::endl;
    std::cout << "1. С��ս��" << std::endl;
    std::cout << "2. BOSSս��" << std::endl;
    std::cout << "3. �鿴״̬" << std::endl;
    std::cout << "4. �鿴����" << std::endl;
    std::cout << "5. ǰ����������" << std::endl;
    std::cout << "6. ������Ϸ" << std::endl;
    std::cout << "7. �˳���Ϸ" << std::endl;

    std::cin >> choice;
    if (std::cin.fail()) {
        std::cin.clear();
        std::cin.ignore(1024, '\n');
        std::cout << "��Чѡ��" << std::endl;
        system("pause");
        return;
    }
    switch (choice) {
    case 1: EncounterEnemy(false); break;
    case 2:
        if (m_currentMap->HasBoss()) EncounterEnemy(true);
        else { std::cout << "��ǰ��ͼû��BOSS" << std::endl; system("pause"); }
        break;
    case 3: m_player->ShowStatus(); system("pause"); break;
    case 4: m_currentState = GameState::INVENTORY; break;
    case 5: MoveToMap(); break;
    case 6:
        if (SaveGame("savegame.dat")) {
            std::cout << "��Ϸ����ɹ���" << std::endl;
        }
        else {
            std::cout << "������Ϸʧ�ܣ�" << std::endl;
        }
        system("pause");
        break;
    case 7: m_running = false; break;
    default: std::cout << "��Чѡ��" << std::endl; system("pause");
    }
}

void GameManager::VisitVillageSupply() {
    system("cls");
    std::cout << "===== ���񲹸�վ =====" << std::endl;

    if (CheckSuppliesReceived()) {
        std::cout << "�ϴ峤: \"���Ѿ���ȡ������Ʒ�ˣ��ú����������ַ�ɳĮ�����ɣ�\"" << std::endl;
        system("pause");
        return;
    }

    std::cout << "�ϴ峤: \"��ӭ�㣬�¸ҵ�սʿ��������Ϊ��׼����һЩ����Ʒ��\"" << std::endl;
    std::cout << "�ϴ峤: \"���Ǵ����Ǵճ�����װ����ҩˮ��ϣ���ܰﵽ�㣺\"" << std::endl;
    std::cout << "- �ֲڵ����� x1" << std::endl;
    std::cout << "- Ƥ�ʻ��� x1" << std::endl;
    std::cout << "- ǿЧҩˮ x2" << std::endl;
    std::cout << std::endl;
    std::cout << "�Ƿ���ȡ������(1.��ȡ 2.��ʱ��Ҫ): ";

    int choice;
    std::cin >> choice;

    if (choice == 1) {
        // �������������
        auto sword = std::make_shared<Item>("�ֲڵ�����", "����+3", ItemType::WEAPON, EquipmentType::MELEE_WEAPON);
        sword->setAttribute(AttributeType::ATTACK, 3);
        m_inventory->storeItem(sword);

        // ���������Ƥ�ʻ���
        auto armor = std::make_shared<Item>("Ƥ�ʻ���", "����+2", ItemType::ARMOR, EquipmentType::ARMOR);
        armor->setAttribute(AttributeType::DEFENSE, 2);
        m_inventory->storeItem(armor);

        // ���������2��ҩˮ
        auto potion1 = std::make_shared<Item>("ǿЧҩˮ", "�ָ�50������ֵ", ItemType::POTION);
        potion1->setAttribute(AttributeType::HP, 50);
        m_inventory->storeItem(potion1);

        auto potion2 = std::make_shared<Item>("ǿЧҩˮ", "�ָ�50������ֵ", ItemType::POTION);
        potion2->setAttribute(AttributeType::HP, 50);
        m_inventory->storeItem(potion2);

        MarkSuppliesReceived();

        std::cout << std::endl;
        std::cout << "�ϴ峤: \"ԸɳĮ֮�����㣬�¸ҵ�սʿ��\"" << std::endl;
        std::cout << "�����˲���Ʒ���ѷ��뱳���С�" << std::endl;
    }
    else {
        std::cout << "�ϴ峤: \"�����Ҫ��������ʱ���������ҡ�\"" << std::endl;
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
        if (m_currentMap->GetName() == "ɽ��") {
            bossName = "��Ѩħ��";
        }
        else if (m_currentMap->GetName() == "ɳĮ") {
            bossName = "ɳĮ����";
        }
        // ���BOSS�Ƿ��ѱ�����
        if (m_bossDefeated[bossName]) {
            std::cout << bossName << " �Ѿ������ܣ������ٴγ��֣�" << std::endl;
            system("pause");
            return;
        }
        std::cout << "��������BOSS��" << std::endl;
        std::unique_ptr<CEnemy> enemy;
        if (m_currentMap->GetName() == "ɽ��") {
            enemy = std::make_unique<CEnemy>("��Ѩħ��", 1.0f, 30.0f, 10.0f, 10.0f,
                "ħ�����", 2.0f, "��ɾ޴��˺�");
        }
        else if (m_currentMap->GetName() == "ɳĮ") {
            enemy = std::make_unique<CEnemy>("ɳĮ����", 250.0f, 35.0f, 12.0f, 8.0f,
                "ɳ����", 2.5f, "�ٻ�ɳ��������˺�������������", 2.0f);
        }
        enemy->ShowStatus();
        std::cout << "׼��ս��..." << std::endl;
        system("pause");
        CFightSystem fightSystem(std::move(m_player), std::move(enemy), *m_inventory);
        fightSystem.StartFight();
        m_player = fightSystem.releasePlayer();
        if (!m_player->IsAlive()) {
            m_currentState = GameState::GAME_OVER;
        }
        else {
            // ��Ҵ�˵��������BOSS
            m_bossDefeated[bossName] = true;
            // ����BOSS����
            std::string dropItem = fightSystem.GetDropItem();
            if (!dropItem.empty() && dropItem == "ħ��ս��") {
                // ����ħ��ս������ӵ�����
                auto axe = std::make_shared<Item>("ħ��ս��", "����+20 ����+5 ����-3", ItemType::WEAPON, EquipmentType::MELEE_WEAPON);
                axe->setAttribute(AttributeType::ATTACK, 20);
                axe->setAttribute(AttributeType::DEFENSE, 5);
                axe->setAttribute(AttributeType::AGILITY, -3);
                m_inventory->storeItem(axe);
                std::cout << "�����ħ��ս����\n";
                system("pause");
            }
        }
        if (!m_player->IsAlive()) m_currentState = GameState::GAME_OVER;
    }
    else {
        std::cout << "��������һֻС�֣�" << std::endl;
        std::unique_ptr<CEnemy> enemy;
        if (m_currentMap->GetName() == "ɭ��") {
            enemy = std::make_unique<CEnemy>("Ұ��", 20.0f, 5.0f, 2.0f, 1.0f);
        }
        else if (m_currentMap->GetName() == "ɽ��") {
            enemy = std::make_unique<CEnemy>("��������", 25.0f, 6.0f, 1.0f, 3.0f); // �޸�Ϊ��������
        }
        else if (m_currentMap->GetName() == "ɳĮ") {
            enemy = std::make_unique<CEnemy>("�綾ɳЫ", 25.0f, 7.0f, 3.0f, 2.0f);
        }

        enemy->ShowStatus();
        std::cout << "׼��ս��..." << std::endl;
        system("pause");
        CFightSystem fightSystem(std::move(m_player), std::move(enemy), *m_inventory);
        fightSystem.StartFight();
        m_player = fightSystem.releasePlayer();
        if (!m_player->IsAlive()) m_currentState = GameState::GAME_OVER;
    }
}

void GameManager::ShowInventory() {
    system("cls");
    std::cout << "===== ���� =====" << std::endl;
    m_inventory->listAllItems();
    std::cout << "1. ʹ����Ʒ\n2. ����̽��\n��ѡ��: ";
    int choice;
    std::cin >> choice;
    if (std::cin.fail()) {
        std::cin.clear();
        std::cin.ignore(1024, '\n');
        std::cout << "��Чѡ��" << std::endl;
        system("pause");
        m_currentState = GameState::EXPLORING;
        return;
    }
    if (choice == 1) {
        std::string itemName;
        std::cout << "������Ҫʹ�õ���Ʒ����: ";
        std::cin >> itemName;
        auto item = m_inventory->takeItem(itemName);
        if (item) {
            UseItem(item);
        }
        else {
            std::cout << "û�и���Ʒ" << std::endl;
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
        std::cout << "ʹ��" << item->getName() << "���ָ�" << heal << "����ֵ��" << std::endl;
    }
    else if (item->getType() == ItemType::WEAPON) {
        int attack = item->getAttribute(AttributeType::ATTACK);
        int agility = item->getAttribute(AttributeType::AGILITY);

        // ʹ����ȷ��װ�������ж�
        if (item->getEquipmentType() == EquipmentType::RANGED_WEAPON) {
            // Զ������
            if (m_player->HasRangedWeapon()) {
                std::cout << "��װ��Զ������ [" << m_player->GetRangedWeapon()->GetName()
                    << "]���Ƿ��滻��(1.�� 2.��): ";
                int choice;
                std::cin >> choice;
                if (choice != 1) {
                    m_inventory->storeItem(item);
                    std::cout << "ȡ��װ����" << std::endl;
                    system("pause");
                    return;
                }

                // ��ԭ����Զ�������Żر���
                auto oldWeapon = std::make_shared<Item>(
                    m_player->GetRangedWeapon()->GetName(),
                    "��װ����Զ������",
                    ItemType::WEAPON,
                    EquipmentType::RANGED_WEAPON
                );
                oldWeapon->setAttribute(AttributeType::ATTACK, m_player->GetRangedWeapon()->GetAttackBonus());
                oldWeapon->setAttribute(AttributeType::AGILITY, m_player->GetRangedWeapon()->GetAgilityBonus());
                m_inventory->storeItem(oldWeapon);
            }

            auto weapon = std::make_unique<Weapon>(item->getName(), EquipmentType::RANGED_WEAPON, attack, agility);
            m_player->EquipRangedWeapon(std::move(weapon));
            std::cout << "װ����Զ������: " << item->getName() << std::endl;
        }
        else {
            // ��ս����
            if (m_player->HasMeleeWeapon()) {
                std::cout << "��װ����ս���� [" << m_player->GetMeleeWeapon()->GetName()
                    << "]���Ƿ��滻��(1.�� 2.��): ";
                int choice;
                std::cin >> choice;
                if (choice != 1) {
                    m_inventory->storeItem(item);
                    std::cout << "ȡ��װ����" << std::endl;
                    system("pause");
                    return;
                }

                // ��ԭ���Ľ�ս�����Żر���
                auto oldWeapon = std::make_shared<Item>(
                    m_player->GetMeleeWeapon()->GetName(),
                    "��װ���Ľ�ս����",
                    ItemType::WEAPON,
                    EquipmentType::MELEE_WEAPON
                );
                oldWeapon->setAttribute(AttributeType::ATTACK, m_player->GetMeleeWeapon()->GetAttackBonus());
                oldWeapon->setAttribute(AttributeType::AGILITY, m_player->GetMeleeWeapon()->GetAgilityBonus());
                m_inventory->storeItem(oldWeapon);
            }

            auto weapon = std::make_unique<Weapon>(item->getName(), EquipmentType::MELEE_WEAPON, attack, agility);
            m_player->EquipMeleeWeapon(std::move(weapon));
            std::cout << "װ���˽�ս����: " << item->getName() << std::endl;
        }
    }
    else if (item->getType() == ItemType::ARMOR) {
        int defense = item->getAttribute(AttributeType::DEFENSE);

        if (m_player->HasArmor()) {
            std::cout << "��װ������ [" << m_player->GetArmor()->GetName()
                << "]���Ƿ��滻��(1.�� 2.��): ";
            int choice;
            std::cin >> choice;
            if (choice != 1) {
                m_inventory->storeItem(item);
                std::cout << "ȡ��װ����" << std::endl;
                system("pause");
                return;
            }

            // ��ԭ���Ļ��׷Żر���
            auto oldArmor = std::make_shared<Item>(
                m_player->GetArmor()->GetName(),
                "��װ���Ļ���",
                ItemType::ARMOR
            );
            oldArmor->setAttribute(AttributeType::DEFENSE, m_player->GetArmor()->GetDefenseBonus());
            m_inventory->storeItem(oldArmor);
        }

        auto armor = std::make_unique<Armor>(item->getName(), defense);
        m_player->EquipArmor(std::move(armor));
        std::cout << "װ���˻���: " << item->getName() << std::endl;
    }
    system("pause");
}

void GameManager::VisitStore() {
    Store store;
    store.ShowMenu(*m_player, *m_inventory);
}

void GameManager::MoveToMap() {
    system("cls");
    std::cout << "===== �л���ͼ =====" << std::endl;
    std::cout << "��ǰλ��: " << m_currentMap->GetName() << std::endl;
    m_currentMap->ShowConnectedMaps();
    std::cout << "��ѡ��Ҫǰ���ĵ�ͼ (0����): ";
    int choice;
    std::cin >> choice;
    if (choice > 0 && choice <= static_cast<int>(m_currentMap->GetConnectedMaps().size())) {
        std::string mapName = m_currentMap->GetConnectedMaps()[choice - 1];
        m_currentMap = m_maps[mapName].get();
        std::cout << "��ǰ�� " << mapName << std::endl;
    }
    else {
        std::cout << "���ء�" << std::endl;
    }
    system("pause");
}

void GameManager::GameOver() {
    system("cls");
    std::cout << "===== ��Ϸ���� =====" << std::endl;
    std::cout << "���Ѿ�������" << std::endl;
    std::cout << "�ȼ�: " << m_player->GetLevel() << std::endl;
    std::cout << "1. �����ֻ�\n2. �˳���Ϸ\n��ѡ��: ";
    int choice;
    std::cin >> choice;
    if (choice == 1) {
        std::string name = m_player->GetName();
        m_player = std::make_unique<CPlayer>(name);
        m_currentMap = m_maps["��ׯ"].get();
        m_currentState = GameState::EXPLORING;
        // ��ձ���
        m_inventory = std::make_unique<Storage>();
        m_hasReceivedSupplies = false;
        // ����BOSS״̬
        m_bossDefeated["��Ѩħ��"] = false;
        m_bossDefeated["ɳĮ����"] = false;
        std::cout << "�����ֻ���������������գ��ص���ׯ��" << std::endl;
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
        // �������
        file << m_player->GetName() << "\n";
        file << m_player->GetLevel() << "\n";
        file << m_player->GetExp() << "\n";
        file << m_player->GetGold() << "\n";
        file << m_player->GetHP() << "\n";
        file << m_player->GetMaxHP() << "\n";
        file << m_player->GetRawAttack() << "\n";
        file << m_player->GetRawDefense() << "\n";
        file << m_player->GetRawAgility() << "\n";

        // ��ǰ��ͼ
        file << m_currentMap->GetName() << "\n";

        file << (m_hasReceivedSupplies ? "1" : "0") << "\n";

        // ����BOSS����״̬
        file << m_bossDefeated["��Ѩħ��"] << "\n";
        file << m_bossDefeated["ɳĮ����"] << "\n";

        // ���汳����Ʒ
        int totalItemCount = 0;
        for (const auto& entry : m_inventory->GetAllItems()) {
            totalItemCount += entry.second.size();
        }
        file << totalItemCount << "\n";

        // ����ÿ����Ʒ����ϸ��Ϣ
        for (const auto& entry : m_inventory->GetAllItems()) {
            for (const auto& item : entry.second) {
                file << item->getName() << "\n";
                file << item->getDescription() << "\n";
                file << static_cast<int>(item->getType()) << "\n";
                file << static_cast<int>(item->getEquipmentType()) << "\n"; // ����������װ������

                // ��������
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
        std::cerr << "����浵����: " << e.what() << std::endl;
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
        file.ignore(); // ���Ļ��з�

        std::getline(file, m_currentMapName);
        std::getline(file, suppliesFlag);

        // ����BOSS����״̬
        file >> caveBossDefeated;
        file.ignore();
        file >> desertBossDefeated;
        file.ignore();

        m_bossDefeated["��Ѩħ��"] = caveBossDefeated;
        m_bossDefeated["ɳĮ����"] = desertBossDefeated;


        // ���ر�����Ʒ����
        int totalItemCount;
        file >> totalItemCount;
        file.ignore();

        // ��յ�ǰ����
        m_inventory = std::make_unique<Storage>();

        // ����ÿ����Ʒ
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

            // �޸����Ƴ��ظ��� item ����
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
        std::cerr << "���ش浵����: " << e.what() << std::endl;
        file.close();
        return false;
    }
    catch (...) {
        std::cerr << "δ֪���ش���" << std::endl;
        file.close();
        return false;
    }

    file.close();
    return true;
}