#ifndef SAVE_MANAGER_H
#define SAVE_MANAGER_H

#include <string>
#include <fstream>
#include <vector>
#include <map>
#include <memory>
#include <ctime>
#include <filesystem>
#include "player.h"
#include "map.h"
#include "storage.h"
#include "item.h"
#include "weapon.h"
#include "armor.h"

// 物品存档结构
struct SavedItem {
    std::string name;
    ItemType type = ItemType::POTION;
    std::map<AttributeType, int> attributes;
    EquipmentType equipType = EquipmentType::NONE;
};

// 存档信息结构体
struct SaveInfo {
    std::string playerName;
    int level = 1;
    int exp = 0;
    int gold = 0;
    float hp = 100.0f;
    float maxHp = 100.0f;
    std::string mapName = "村庄";
    std::time_t saveTime = 0;
    uint32_t version = 0;

    void Display() const {
        std::cout << "玩家: " << playerName << " Lv." << level << std::endl;
        std::cout << "生命: " << hp << "/" << maxHp << std::endl;
        std::cout << "金币: " << gold << " 经验: " << exp << std::endl;
        std::cout << "位置: " << mapName << std::endl;

        // 显示保存时间
        char timeBuffer[80];
        std::strftime(timeBuffer, sizeof(timeBuffer), "%Y-%m-%d %H:%M:%S",
            std::localtime(&saveTime));
        std::cout << "保存时间: " << timeBuffer << std::endl;
    }
};

class SaveManager {
private:
    static const uint32_t SAVE_VERSION = 2;
    static const size_t MAX_ITEM_NAME_LENGTH = 256;
    static const size_t MAX_ATTRIBUTES = 20;

    // 二进制读写辅助函数
    template<typename T>
    static void WriteBinary(std::ofstream& file, const T& value) {
        file.write(reinterpret_cast<const char*>(&value), sizeof(value));
    }

    template<typename T>
    static bool ReadBinary(std::ifstream& file, T& value) {
        file.read(reinterpret_cast<char*>(&value), sizeof(value));
        return file.good();
    }

    // 字符串读写辅助函数
    static void WriteString(std::ofstream& file, const std::string& str);
    static bool ReadString(std::ifstream& file, std::string& str);

    // 辅助函数：保存和加载物品
    static bool SaveItem(std::ofstream& file, const std::shared_ptr<Item>& item);
    static std::shared_ptr<Item> LoadItem(std::ifstream& file);
    static SavedItem ConvertToSavedItem(const std::shared_ptr<Item>& item);
    static std::shared_ptr<Item> ConvertFromSavedItem(const SavedItem& savedItem);

public:
    // 版本控制
    static const uint32_t CURRENT_VERSION = SAVE_VERSION;

    // 主要存档功能
    static bool SaveGame(const std::string& filename,
        const CPlayer& player,
        const Map* currentMap,
        const Storage& inventory);

    static bool LoadGame(const std::string& filename,
        CPlayer& player,
        Map*& currentMap,
        Storage& inventory,
        const std::map<std::string, std::unique_ptr<Map>>& maps);

   
    static bool GetSaveInfo(const std::string& filename, SaveInfo& info);
    static bool ValidateSaveFile(const std::string& filename);
    static bool BackupSave(const std::string& filename);
    static bool RestoreBackup(const std::string& filename);
    static std::vector<std::string> GetSaveFiles(const std::string& directory = "saves");
    static bool DeleteSaveFile(const std::string& filename);
    static bool CheckVersionCompatibility(uint32_t fileVersion);
};

#endif