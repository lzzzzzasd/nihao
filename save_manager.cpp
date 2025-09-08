#include "save_manager.h"
#include <iostream>
#include <cstring>
#include <stdexcept>
#include <algorithm>
#include <sstream>

namespace fs = std::filesystem;

// 字符串写入
void SaveManager::WriteString(std::ofstream& file, const std::string& str) {
    size_t len = str.length();
    WriteBinary(file, len);
    if (len > 0) {
        file.write(str.c_str(), len);
    }
}

// 字符串读取
bool SaveManager::ReadString(std::ifstream& file, std::string& str) {
    size_t len;
    if (!ReadBinary(file, len)) return false;

    if (len > MAX_ITEM_NAME_LENGTH) {
        std::cerr << "字符串长度异常: " << len << std::endl;
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

// 保存物品
bool SaveManager::SaveItem(std::ofstream& file, const std::shared_ptr<Item>& item) {
    if (!file.is_open() || !item) return false;

    try {
        // 保存物品名称
        WriteString(file, item->getName());

        // 保存物品类型
        ItemType type = item->getType();
        WriteBinary(file, type);

        // 保存属性数量和属性
        auto attributes = item->getAllAttributes();
        size_t attrCount = attributes.size();
        WriteBinary(file, attrCount);

        for (const auto& pair : attributes) {
            WriteBinary(file, pair.first);
            WriteBinary(file, pair.second);
        }

        // 如果是装备，保存装备类型
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
        std::cerr << "保存物品失败: " << item->getName() << std::endl;
        return false;
    }
}

// 加载物品
std::shared_ptr<Item> SaveManager::LoadItem(std::ifstream& file) {
    if (!file.is_open()) return nullptr;

    try {
        // 读取物品名称
        std::string name;
        if (!ReadString(file, name)) return nullptr;

        // 读取物品类型
        ItemType type;
        if (!ReadBinary(file, type)) return nullptr;

        // 验证类型
        if (type < ItemType::POTION || type > ItemType::ARMOR) {
            std::cerr << "无效的物品类型: " << static_cast<int>(type) << std::endl;
            return nullptr;
        }

        // 读取属性数量
        size_t attrCount;
        if (!ReadBinary(file, attrCount)) return nullptr;

        if (attrCount > MAX_ATTRIBUTES) {
            std::cerr << "属性数量异常: " << attrCount << std::endl;
            return nullptr;
        }

        // 读取属性
        std::map<AttributeType, int> attributes;
        for (size_t i = 0; i < attrCount; ++i) {
            AttributeType attrType;
            int value;

            if (!ReadBinary(file, attrType)) return nullptr;
            if (!ReadBinary(file, value)) return nullptr;

            // 验证属性类型
            if (attrType < AttributeType::HP || attrType > AttributeType::AGILITY) {
                std::cerr << "无效的属性类型: " << static_cast<int>(attrType) << std::endl;
                return nullptr;
            }

            attributes[attrType] = value;
        }

        // 根据类型创建物品
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
        std::cerr << "加载物品失败: " << e.what() << std::endl;
        return nullptr;
    }
}

// 转换物品为存档结构
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

// 从存档结构恢复物品
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

// 获取存档信息
bool SaveManager::GetSaveInfo(const std::string& filename, SaveInfo& info) {
    std::ifstream file(filename, std::ios::binary);
    if (!file.is_open()) {
        std::cerr << "无法打开存档文件: " << filename << std::endl;
        return false;
    }

    try {
        // 读取版本信息
        uint32_t version;
        if (!ReadBinary(file, version)) return false;
        info.version = version;

        // 检查版本兼容性
        if (!CheckVersionCompatibility(version)) {
            std::cerr << "存档版本不兼容: " << version << std::