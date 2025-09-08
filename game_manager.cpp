#include "game_manager.h"
#include <iostream>
#include <string>
#include <cstdlib>
#include <ctime>
#include <stdexcept>

// ��ƽ̨��������
void ClearScreen() {
#ifdef _WIN32
    system("cls");
#else
    system("clear");
#endif
}

// ��ƽ̨��ͣ����
void Pause() {
#ifdef _WIN32
    system("pause");
#else
    std::cout << "��Enter������...";
    std::cin.ignore();
    std::cin.get();
#endif
}

GameManager::GameManager()
    : m_currentState(GameState::MAIN_MENU), m_currentMap(nullptr), m_running(true) {
    m_inventory = std::make_unique<Storage>();
    InitializeMaps();
    std::srand(static_cast<unsigned int>(std::time(nullptr)));
}

void GameManager::InitializeMaps() {
    m_maps["��ׯ"] = std::make_unique<Map>("��ׯ");
    m_maps["ɭ��"] = std::make_unique<Map>("ɭ��");
    m_maps["ɽ��"] = std::make_unique<Map>("ɽ��", true);
    m_maps["�̵�"] = std::make_unique<Map>("�̵�");

    m_maps["��ׯ"]->AddConnectedMap("ɭ��");
    m_maps["��ׯ"]->AddConnectedMap("�̵�");

    m_maps["ɭ��"]->AddConnectedMap("��ׯ");
    m_maps["ɭ��"]->AddConnectedMap("ɽ��");

    m_maps["ɽ��"]->AddConnectedMap("ɭ��");

    m_maps["�̵�"]->AddConnectedMap("��ׯ");
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
    ClearScreen();
    std::cout << "===== ��ӭ����MUDð������ =====" << std::endl;
    std::cout << "1. ��ʼ��Ϸ\n2. ��ȡ�浵\n3. �˳���Ϸ\n��ѡ��: ";
    int choice;
    std::cin >> choice;
    if (std::cin.fail()) {
        std::cin.clear();
        std::cin.ignore(1024, '\n');
        std::cout << "��Чѡ��" << std::endl;
        Pause();
        return;
    }
    if (choice == 1) {
        std::string name;
        std::cout << "�������������: ";
        std::cin >> name;
        m_player = std::make_unique<CPlayer>(name);
        m_currentMap = m_maps["��ׯ"].get();
        m_player->SetHP(m_player->GetMaxHP());
        m_currentState = GameState::EXPLORING;
    }
    else if (choice == 2) {
        if (!m_player) {
            m_player = std::make_unique<CPlayer>("��ʱ���");
        }
        LoadGame();
    }
    else if (choice == 3) {
        m_running = false;
    }
    else {
        std::cout << "��Чѡ��" << std::endl;
        Pause();
    }
}

// �����Ʒ���ֹ��ܵ�ʵ��
void GameManager::FindItem() {
    ClearScreen();
    // ������ɼ��ֿ��ܵ���Ʒ
    int randItem = rand() % 3;
    std::shared_ptr<Item> item;

    switch (randItem) {
    case 0:
        item = std::make_shared<Potion>("С�ͻָ�ҩˮ", 30);
        break;
    case 1:
        item = std::make_shared<Weapon>("�����С��", EquipmentType::KNIFE, 5, 2);
        break;
    case 2:
        item = std::make_shared<Armor>("Ƥ��", 3);
        break;
    }

    m_inventory->addItem(item);
    std::cout << "���ҵ��� " << item->getName() << "���ѷ��뱳����" << std::endl;
    Pause();
}

void GameManager::ExploreArea() {
    ClearScreen();
    std::cout << "===== " << m_currentMap->GetName() << " =====" << std::endl;
    std::cout << "��ǰλ��: " << m_currentMap->GetName() << std::endl;

    int choice = 0;
    if (m_currentMap->GetName() == "��ׯ") {
        std::cout << "����һ�������Ĵ�ׯ����ֻ��ȥ����������̵ꡣ" << std::endl;
        std::cout << "1. ǰ����������" << std::endl;
        std::cout << "2. �����̵�" << std::endl;
        std::cout << "3. �鿴״̬" << std::endl;
        std::cout << "4. �鿴����" << std::endl;
        std::cout << "5. ������Ϸ" << std::endl;
        std::cout << "6. �˳���Ϸ" << std::endl;

        std::cin >> choice;
        if (std::cin.fail()) {
            std::cin.clear();
            std::cin.ignore(1024, '\n');
            std::cout << "��Чѡ��" << std::endl;
            Pause();
            return;
        }
        switch (choice) {
        case 1: MoveToMap(); break;
        case 2: m_currentState = GameState::STORE; break;
        case 3: m_player->ShowStatus(); Pause(); break;
        case 4: m_currentState = GameState::INVENTORY; break;
        case 5: SaveGame(); break;
        case 6: m_running = false; break;
        default: std::cout << "��Чѡ��" << std::endl; Pause();
        }
        return;
    }

    if (m_currentMap->GetName() == "�̵�") {
        std::cout << "�����̵꣬���Թ�����Ʒ��" << std::endl;
        std::cout << "1. ������Ʒ" << std::endl;
        std::cout << "2. ǰ����������" << std::endl;
        std::cout << "3. �鿴״̬" << std::endl;
        std::cout << "4. �鿴����" << std::endl;
        std::cout << "5. ������Ϸ" << std::endl;
        std::cout << "6. �˳���Ϸ" << std::endl;
        std::cin >> choice;
        if (std::cin.fail()) {
            std::cin.clear();
            std::cin.ignore(1024, '\n');
            std::cout << "��Чѡ��" << std::endl;
            Pause();
            return;
        }
        switch (choice) {
        case 1: m_currentState = GameState::STORE; break;
        case 2: MoveToMap(); break;
        case 3: m_player->ShowStatus(); Pause(); break;
        case 4: m_currentState = GameState::INVENTORY; break;
        case 5: SaveGame(); break;
        case 6: m_running = false; break;
        default: std::cout << "��Чѡ��" << std::endl; Pause();
        }
        return;
    }

    // ����¼����м��ʷ�����Ʒ
    int randEvent = rand() % 10;
    if (randEvent < 2) { // 20%���ʷ�����Ʒ
        FindItem();
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
        Pause();
        return;
    }
    switch (choice) {
    case 1: EncounterEnemy(false); break;
    case 2:
        if (m_currentMap->HasBoss()) EncounterEnemy(true);
        else { std::cout << "��ǰ��ͼû��BOSS" << std::endl; Pause(); }
        break;
    case 3: m_player->ShowStatus(); Pause(); break;
    case 4: m_currentState = GameState::INVENTORY; break;
    case 5: MoveToMap(); break;
    case 6: SaveGame(); break;
    case 7: m_running = false; break;
    default: std::cout << "��Чѡ��" << std::endl; Pause();
    }
}

void GameManager::EncounterEnemy(bool isBoss) {
    ClearScreen();
    if (isBoss) {
        std::cout << "��������BOSS��" << std::endl;
        auto enemy = std::make_unique<CEnemy>("��Ѩħ��", 200.0f, 30.0f, 10.0f, 10.0f, "ħ�����", 2.0f, "��ɾ޴��˺�");
        enemy->ShowStatus();
        std::cout << "׼��ս��..." << std::endl;
        Pause();
        CFightSystem fightSystem(std::move(m_player), std::move(enemy));
        fightSystem.StartFight();
        m_player = fightSystem.releasePlayer();
        if (!m_player->IsAlive()) m_currentState = GameState::GAME_OVER;
    }
    else {
        std::cout << "��������һֻС�֣�" << std::endl;
        auto enemy = std::make_unique<CEnemy>("Ұ��", 20.0f, 5.0f, 2.0f, 1.0f);
        enemy->ShowStatus();
        std::cout << "׼��ս��..." << std::endl;
        Pause();
        CFightSystem fightSystem(std::move(m_player), std::move(enemy));
        fightSystem.StartFight();
        m_player = fightSystem.releasePlayer();
        if (!m_player->IsAlive()) m_currentState = GameState::GAME_OVER;
    }
}

void GameManager::ShowInventory() {
    ClearScreen();
    std::cout << "===== ���� =====" << std::endl;
    m_inventory->listAllItems();
    std::cout << "1. ʹ����Ʒ\n2. ����̽��\n��ѡ��: ";
    int choice;
    std::cin >> choice;
    if (std::cin.fail()) {
        std::cin.clear();
        std::cin.ignore(1024, '\n');
        std::cout << "��Чѡ��" << std::endl;
        Pause();
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
            Pause();
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

        auto weapon = std::dynamic_pointer_cast<Weapon>(item);
        if (weapon) {
            if (weapon->getEquipmentType() == EquipmentType::KNIFE) {
                m_player->EquipKnife(std::make_unique<Weapon>(*weapon));
            }
            else {
                m_player->EquipGun(std::make_unique<Weapon>(*weapon));
            }
        }
        std::cout << "װ����" << item->getName() << std::endl;
    }
    else if (item->getType() == ItemType::ARMOR) {
        int defense = item->getAttribute(AttributeType::DEFENSE);
        auto armor = std::dynamic_pointer_cast<Armor>(item);
        if (armor) {
            m_player->EquipArmor(std::make_unique<Armor>(*armor));
        }
        std::cout << "װ����" << item->getName() << std::endl;
    }
    Pause();
}

void GameManager::VisitStore() {
    Store store;
    store.ShowMenu(*m_player, *m_inventory);
}

void GameManager::MoveToMap() {
    ClearScreen();
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
    Pause();
}

void GameManager::GameOver() {
    ClearScreen();
    std::cout << "===== ��Ϸ���� =====" << std::endl;
    std::cout << "���Ѿ�������" << std::endl;
    std::cout << "�ȼ�: " << m_player->GetLevel() << std::endl;
    std::cout << "1. �����ֻ�\n2. �����¼���˳�\n3. ֱ���˳���Ϸ\n��ѡ��: ";
    int choice;
    std::cin >> choice;
    if (choice == 1) {
        std::string name = m_player->GetName();
        m_player = std::make_unique<CPlayer>(name);
        m_currentMap = m_maps["��ׯ"].get();
        m_currentState = GameState::EXPLORING;
        // ��ձ���
        m_inventory = std::make_unique<Storage>();
        std::cout << "�����ֻ���������������գ��ص���ׯ��" << std::endl;
        Pause();
    }
    else if (choice == 2) {
        SaveGame();
        m_running = false;
    }
    else {
        m_running = false;
    }
}

// �浵����ʵ��
void GameManager::SaveGame() {
    if (!m_player) {
        std::cout << "û�пɱ������Ϸ���ȣ�" << std::endl;
        Pause();
        return;
    }

    std::string filename;
    std::cout << "������浵�ļ���: ";
    std::cin >> filename;

    if (SaveManager::SaveGame(filename, *m_player, m_currentMap, *m_inventory)) {
        std::cout << "�浵�ɹ�!" << std::endl;
    }
    else {
        std::cout << "�浵ʧ��!" << std::endl;
    }
    Pause();
}

// ��������ʵ��
void GameManager::LoadGame() {
    std::string filename;
    std::cout << "����������ļ���: ";
    std::cin >> filename;

    if (SaveManager::LoadGame(filename, *m_player, m_currentMap, *m_inventory, m_maps)) {
        std::cout << "�����ɹ�!" << std::endl;
        m_currentState = GameState::EXPLORING;
    }
    else {
        std::cout << "����ʧ��!" << std::endl;
    }
    Pause();
}
