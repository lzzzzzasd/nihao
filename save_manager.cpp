#include "save_manager.h"
#include <iostream>
#include <cstring>
#include <stdexcept>
#include <algorithm>
#include <sstream>

namespace fs = std::filesystem;

// �ַ���д��
void SaveManager::WriteString(std::ofstream& file, const std::string& str) {
    size_t len = str.length();
    WriteBinary(file, len);
    if (len > 0) {
        file.write(str.c_str(), len);
    }
}

// �ַ�����ȡ
bool SaveManager::ReadString(std::ifstream& file, std::string& str) {
    size_t len;
    if (!ReadBinary(file, len)) return false;

    if (len > MAX_ITEM_NAME_LENGTH) {
        std::cerr << "�ַ��������쳣: " << len << std::endl;
        return false;
    }

    if (len > 0) {
        str.resize(len);
        file.read(&str[0], len);
        return file.good();
    }

    str.clear();
    return true;
}

// ������Ʒ
bool SaveManager::SaveItem(std::ofstream& file, const std::shared_ptr<Item>& item) {
    if (!file.is_open() || !item) return false;

    try {
        // ������Ʒ����
        WriteString(file, item->getName());

        // ������Ʒ����
        ItemType type = item->getType();
        WriteBinary(file, type);

        // ������������������
        auto attributes = item->getAllAttributes();
        size_t attrCount = attributes.size();
        WriteBinary(file, attrCount);

        for (const auto& pair : attributes) {
            WriteBinary(file, pair.first);
            WriteBinary(file, pair.second);
        }

        // �����װ��������װ������
        if (type == ItemType::WEAPON) {
            auto weapon = std::dynamic_pointer_cast<Weapon>(item);
            if (weapon) {
                EquipmentType equipType = weapon->getEquipmentType();
                WriteBinary(file, equipType);
            }
        }

        return true;
    }
    catch (...) {
        std::cerr << "������Ʒʧ��: " << item->getName() << std::endl;
        return false;
    }
}

// ������Ʒ
std::shared_ptr<Item> SaveManager::LoadItem(std::ifstream& file) {
    if (!file.is_open()) return nullptr;

    try {
        // ��ȡ��Ʒ����
        std::string name;
        if (!ReadString(file, name)) return nullptr;

        // ��ȡ��Ʒ����
        ItemType type;
        if (!ReadBinary(file, type)) return nullptr;

        // ��֤����
        if (type < ItemType::POTION || type > ItemType::ARMOR) {
            std::cerr << "��Ч����Ʒ����: " << static_cast<int>(type) << std::endl;
            return nullptr;
        }

        // ��ȡ��������
        size_t attrCount;
        if (!ReadBinary(file, attrCount)) return nullptr;

        if (attrCount > MAX_ATTRIBUTES) {
            std::cerr << "���������쳣: " << attrCount << std::endl;
            return nullptr;
        }

        // ��ȡ����
        std::map<AttributeType, int> attributes;
        for (size_t i = 0; i < attrCount; ++i) {
            AttributeType attrType;
            int value;

            if (!ReadBinary(file, attrType)) return nullptr;
            if (!ReadBinary(file, value)) return nullptr;

            // ��֤��������
            if (attrType < AttributeType::HP || attrType > AttributeType::AGILITY) {
                std::cerr << "��Ч����������: " << static_cast<int>(attrType) << std::endl;
                return nullptr;
            }

            attributes[attrType] = value;
        }

        // �������ʹ�����Ʒ
        switch (type) {
        case ItemType::POTION: {
            int hpValue = attributes.count(AttributeType::HP) ? attributes[AttributeType::HP] : 0;
            return std::make_shared<Potion>(name, hpValue);
        }
        case ItemType::WEAPON: {
            EquipmentType equipType;
            if (!ReadBinary(file, equipType)) return nullptr;

            int attack = attributes.count(AttributeType::ATTACK) ? attributes[AttributeType::ATTACK] : 0;
            int agility = attributes.count(AttributeType::AGILITY) ? attributes[AttributeType::AGILITY] : 0;

            return std::make_shared<Weapon>(name, equipType, attack, agility);
        }
        case ItemType::ARMOR: {
            int defense = attributes.count(AttributeType::DEFENSE) ? attributes[AttributeType::DEFENSE] : 0;
            return std::make_shared<Armor>(name, defense);
        }
        default:
            return nullptr;
        }
    }
    catch (const std::exception& e) {
        std::cerr << "������Ʒʧ��: " << e.what() << std::endl;
        return nullptr;
    }
}

// ת����ƷΪ�浵�ṹ
SavedItem SaveManager::ConvertToSavedItem(const std::shared_ptr<Item>& item) {
    SavedItem savedItem;
    if (!item) return savedItem;

    savedItem.name = item->getName();
    savedItem.type = item->getType();
    savedItem.attributes = item->getAllAttributes();

    if (savedItem.type == ItemType::WEAPON) {
        auto weapon = std::dynamic_pointer_cast<Weapon>(item);
        if (weapon) {
            savedItem.equipType = weapon->getEquipmentType();
        }
    }

    return savedItem;
}

// �Ӵ浵�ṹ�ָ���Ʒ
std::shared_ptr<Item> SaveManager::ConvertFromSavedItem(const SavedItem& savedItem) {
    switch (savedItem.type) {
    case ItemType::POTION: {
        int hpValue = savedItem.attributes.count(AttributeType::HP) ?
            savedItem.attributes.at(AttributeType::HP) : 0;
        return std::make_shared<Potion>(savedItem.name, hpValue);
    }
    case ItemType::WEAPON: {
        int attack = savedItem.attributes.count(AttributeType::ATTACK) ?
            savedItem.attributes.at(AttributeType::ATTACK) : 0;
        int agility = savedItem.attributes.count(AttributeType::AGILITY) ?
            savedItem.attributes.at(AttributeType::AGILITY) : 0;
        return std::make_shared<Weapon>(savedItem.name, savedItem.equipType, attack, agility);
    }
    case ItemType::ARMOR: {
        int defense = savedItem.attributes.count(AttributeType::DEFENSE) ?
            savedItem.attributes.at(AttributeType::DEFENSE) : 0;
        return std::make_shared<Armor>(savedItem.name, defense);
    }
    default:
        return nullptr;
    }
}

// ��ȡ�浵��Ϣ
bool SaveManager::GetSaveInfo(const std::string& filename, SaveInfo& info) {
    std::ifstream file(filename, std::ios::binary);
    if (!file.is_open()) {
        std::cerr << "�޷��򿪴浵�ļ�: " << filename << std::endl;
        return false;
    }

    try {
        // ��ȡ�汾��Ϣ
        uint32_t version;
        if (!ReadBinary(file, version)) return false;
        info.version = version;

        // ���汾������
        if (!CheckVersionCompatibility(version)) {
            std::cerr << "�浵�汾������: " << version << std::