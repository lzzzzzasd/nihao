#include "game_manager.h"
#include <iostream>
#include <string>
#include <cstdlib>
#include <ctime>
#include <stdexcept>
#include <fstream>
#include <sstream>

// ����Ĭ�ϴ浵�ļ�������
const std::string DEFAULT_SAVE_FILE = "default_save";

GameManager::GameManager()
    : m_currentState(GameState::MAIN_MENU), m_currentMap(nullptr), m_running(true) {
    m_inventory = std::make_unique<Storage>();
    InitializeMaps();
    std::srand(static_cast<unsigned int>(std::time(nullptr)));
}

void GameManager::InitializeMaps() {
    m_maps["ɭ��"] = std::make_unique<Map>("ɭ��");
    m_maps["ɽ��"] = std::make_unique<Map>("ɽ��");
    m_maps["ɽ��"] = std::make_unique<Map>("ɽ��", true);
    m_maps["�̵�"] = std::make_unique<Map>("�̵�");

    m_maps["ɭ��"]->AddConnectedMap("ɽ��");
    m_maps["ɭ��"]->AddConnectedMap("�̵�");

    m_maps["ɽ��"]->AddConnectedMap("ɭ��");
    m_maps["ɽ��"]->AddConnectedMap("ɽ��");

    m_maps["ɽ��"]->AddConnectedMap("ɽ��");

    m_maps["�̵�"]->AddConnectedMap("ɭ��");
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
    std::cout << "===== ��ӭ����MUD��Ϸ =====" << std::endl;
    std::cout << "1. ��ʼ����Ϸ\n2. �����ϴ���Ϸ\n3. �˳���Ϸ\n��ѡ��: ";
    std::string choice;
    std::cin >> choice;

    if (choice == "1") {
        std::string name;
        std::cout << "�������������: ";
        std::cin >> name;
        m_player = std::make_unique<CPlayer>(name);
        m_currentMap = m_maps["ɭ��"].get();
        m_player->SetHP(m_player->GetMaxHP());
        m_currentState = GameState::EXPLORING;
    }
    else if (choice == "2") { // ����Ĭ�ϴ浵
        if (LoadGame(DEFAULT_SAVE_FILE)) {
            std::cout << "���سɹ�!" << std::endl;
            system("pause");
            m_currentState = GameState::EXPLORING;
        }
        else {
            std::cout << "����ʧ��! ����û�д浵" << std::endl;
            system("pause");
        }
    }
    else if (choice == "3") {
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

    std::string choice;
    if (m_currentMap->GetName() == "ɭ��") {
        std::cout << "����һƬï�ܵ�ɭ�֣������ȥɽ�������̵ꡣ" << std::endl;
        std::cout << "1. ǰ��ɽ��" << std::endl;
        std::cout << "2. ǰ���̵�" << std::endl;
        std::cout << "3. �鿴״̬" << std::endl;
        std::cout << "4. �鿴����" << std::endl;
        std::cout << "5. �浵" << std::endl;  // �浵ѡ��
        std::cout << "6. �˳���Ϸ" << std::endl;

        std::cin >> choice;
        if (choice == "1") { MoveToMap(); }
        else if (choice == "2") { m_currentState = GameState::STORE; }
        else if (choice == "3") { m_player->ShowStatus(); system("pause"); }
        else if (choice == "4") { m_currentState = GameState::INVENTORY; }
        else if (choice == "5") { m_currentState = GameState::SAVE_MENU; } // ����浵����
        else if (choice == "6") { m_running = false; }
        else { std::cout << "��Чѡ��" << std::endl; system("pause"); }
        return;
    }

    if (m_currentMap->GetName() == "�̵�") {
        std::cout << "�������̵꣬���Թ�����Ʒ����Ϣ��" << std::endl;
        std::cout << "1. ������Ʒ" << std::endl;
        std::cout << "2. ǰ��ɭ��" << std::endl;
        std::cout << "3. �鿴״̬" << std::endl;
        std::cout << "4. �鿴����" << std::endl;
        std::cout << "5. �浵" << std::endl;  // �浵ѡ��
        std::cout << "6. �˳���Ϸ" << std::endl;
        std::cin >> choice;
        if (choice == "1") { m_currentState = GameState::STORE; }
        else if (choice == "2") { MoveToMap(); }
        else if (choice == "3") { m_player->ShowStatus(); system("pause"); }
        else if (choice == "4") { m_currentState = GameState::INVENTORY; }
        else if (choice == "5") { m_currentState = GameState::SAVE_MENU; } // ����浵����
        else if (choice == "6") { m_running = false; }
        else { std::cout << "��Чѡ��" << std::endl; system("pause"); }
        return;
    }

    std::cout << "��ѡ��̽����ʽ" << std::endl;
    std::cout << "1. С��ս��" << std::endl;
    std::cout << "2. BOSSս��" << std::endl;
    std::cout << "3. �鿴״̬" << std::endl;
    std::cout << "4. �鿴����" << std::endl;
    std::cout << "5. ǰ��������ͼ" << std::endl;
    std::cout << "6. ���沢�˳�" << std::endl;  // �浵ѡ��
    std::cout << "7. ֱ���˳���Ϸ" << std::endl;

    std::cin >> choice;
    if (choice == "1") { EncounterEnemy(false); }
    else if (choice == "2") {
        if (m_currentMap->HasBoss()) EncounterEnemy(true);
        else { std::cout << "��ǰ��ͼû��BOSS" << std::endl; system("pause"); }
    }
    else if (choice == "3") { m_player->ShowStatus(); system("pause"); }
    else if (choice == "4") { m_currentState = GameState::INVENTORY; }
    else if (choice == "5") { MoveToMap(); }
    else if (choice == "6") { m_currentState = GameState::SAVE_MENU; } // ����浵����
    else if (choice == "7") { m_running = false; }
    else { std::cout << "��Чѡ��" << std::endl; system("pause"); }
}

void GameManager::EncounterEnemy(bool isBoss) {
    system("cls");
    if (isBoss) {
        std::cout << "��������BOSS!" << std::endl;
        auto enemy = std::make_unique<CEnemy>("��Ӱħ��", 200.0f, 30.0f, 10.0f, 10.0f, "��Ӱ���", 2.0f, "��ɾ޴��˺�");
        enemy->ShowStatus();
        std::cout << "׼��ս��..." << std::endl;
        system("pause");
        CFightSystem fightSystem(std::move(m_player), std::move(enemy));
        fightSystem.StartFight();
        m_player = fightSystem.releasePlayer();
        if (!m_player->IsAlive()) m_currentState = GameState::GAME_OVER;
    }
    else {
        std::cout << "��������һֻС��!" << std::endl;
        auto enemy = std::make_unique<CEnemy>("����", 20.0f, 5.0f, 2.0f, 1.0f);
        enemy->ShowStatus();
        std::cout << "׼��ս��..." << std::endl;
        system("pause");
        CFightSystem fightSystem(std::move(m_player), std::move(enemy));
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
    std::string choice;
    std::cin >> choice;

    if (choice == "1") {
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
        std::cout << "ʹ��" << item->getName() << "�ָ���" << heal << "������ֵ!" << std::endl;
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
        std::cout << "װ����" << item->getName() << std::endl;
    }
    else if (item->getType() == ItemType::ARMOR) {
        int defense = item->getAttribute(AttributeType::DEFENSE);
        auto armor = std::make_unique<Armor>(item->getName(), defense);
        m_player->EquipArmor(std::move(armor));
        std::cout << "װ����" << item->getName() << std::endl;
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
    std::cout << "��ѡ��Ҫǰ���ĵ�ͼ (����0����): ";
    std::string choice;
    std::cin >> choice;

    if (choice != "0") {
        try {
            int mapIndex = std::stoi(choice);
            if (mapIndex > 0 && mapIndex <= static_cast<int>(m_currentMap->GetConnectedMaps().size())) {
                std::string mapName = m_currentMap->GetConnectedMaps()[mapIndex - 1];
                m_currentMap = m_maps[mapName].get();
                std::cout << "�������� " << mapName << std::endl;
            }
            else {
                std::cout << "��Ч�ĵ�ͼѡ��" << std::endl;
            }
        }
        catch (...) {
            std::cout << "��������Ч������" << std::endl;
        }
    }
    else {
        std::cout << "ȡ����" << std::endl;
    }
    system("pause");
}

void GameManager::GameOver() {
    system("cls");
    std::cout << "===== ��Ϸ���� =====" << std::endl;
    std::cout << "���Ѿ�����" << std::endl;
    std::cout << "�ȼ�: " << m_player->GetLevel() << std::endl;
    std::cout << "1. ���¿�ʼ\n2. �˳���Ϸ\n��ѡ��: ";
    std::string choice;
    std::cin >> choice;

    if (choice == "1") {
        std::string name = m_player->GetName();
        m_player = std::make_unique<CPlayer>(name);
        m_currentMap = m_maps["ɭ��"].get();
        m_currentState = GameState::EXPLORING;
        // ��ձ���
        m_inventory = std::make_unique<Storage>();
        std::cout << "���¿�ʼ�ɹ����ص�ɭ��!" << std::endl;
        system("pause");
    }
    else if (choice == "2") {
        m_running = false;
    }
    else {
        std::cout << "��Чѡ��" << std::endl;
        system("pause");
    }
}

// �浵�����Զ�ʹ��Ĭ���ļ�����
void GameManager::ShowSaveMenu() {
    system("cls");
    std::cout << "===== �浵���� =====" << std::endl;

    // ֱ��ʹ��Ĭ���ļ����浵�������û�����
    if (SaveGame(DEFAULT_SAVE_FILE)) {
        std::cout << "��Ϸ����ɹ���" << std::endl;
    }
    else {
        std::cout << "��Ϸ����ʧ�ܣ�" << std::endl;
    }

    system("pause");
    m_currentState = GameState::EXPLORING;
}

// ������Ϸ��ʹ��Ĭ���ļ�����
bool GameManager::SaveGame(const std::string& filename) {
    try {
        std::ofstream file(filename + ".sav");
        if (!file.is_open()) return false;

        // ���������Ϣ
        file << "PLAYER\n";
        file << m_player->GetName() << "\n";
        file << m_player->GetLevel() << "\n";
        file << m_player->GetExp() << "\n";
        file << m_player->GetGold() << "\n";
        file << m_player->GetHP() << "\n";
        file << m_player->GetRawAttack() << "\n";
        file << m_player->GetRawDefense() << "\n";
        file << m_player->GetRawAgility() << "\n";

        // ���浱ǰ��ͼ
        file << "MAP\n";
        file << m_currentMap->GetName() << "\n";

        // ���汳����Ʒ����
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

// ������Ϸ��ʹ��Ĭ���ļ�����
bool GameManager::LoadGame(const std::string& filename) {
    try {
        std::ifstream file(filename + ".sav");
        if (!file.is_open()) return false;

        std::string line;
        // ���������Ϣ
        std::getline(file, line); // ��ȡ"PLAYER"���
        if (line != "PLAYER") return false;

        std::string name;
        int level, exp, gold;
        float hp, attack, defense, agility;

        std::getline(file, name);
        file >> level >> exp >> gold >> hp >> attack >> defense >> agility;
        file.ignore(); // ���Ի��з�

        m_player = std::make_unique<CPlayer>(name);
        m_player->RestoreToInitialState(); // ���ú��ٸ�ֵ
        m_player->AddExp(exp);
        m_player->AddGold(gold);
        m_player->SetHP(hp);
        // ͨ��LevelUp������õȼ�
        while (m_player->GetLevel() < level) {
            m_player->LevelUp();
        }

        // ���ص�ǰ��ͼ
        std::getline(file, line); // ��ȡ"MAP"���
        if (line != "MAP") return false;

        std::string mapName;
        std::getline(file, mapName);
        m_currentMap = m_maps[mapName].get();

        // ���ر�����Ʒ
        std::getline(file, line); // ��ȡ"INVENTORY"���
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