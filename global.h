#ifndef GLOBAL_H
#define GLOBAL_H

#include <string>
#include <iostream>

// 战斗结果
enum class FightResult {
    CONTINUE,
    PLAYER_WIN,
    ENEMY_WIN
};

// 技能类型
enum class SkillType {
    NORMAL_ATTACK,
    SKILL_1,
    SKILL_2,
    SKILL_3,
    BOSS_SKILL
};

// 装备类型
enum class EquipmentType {
    RANGED_WEAPON,  
    MELEE_WEAPON,   
    ARMOR
};

// 敌人类型
enum class EnemyType {
    MONSTER,
    BOSS
};

// 物品类型
enum class ItemType {
    WEAPON,
    ARMOR,
    POTION,
    MONEY,
};

// 属性类型
enum class AttributeType {
    ATTACK,
    DEFENSE,
    AGILITY,
    HP,
};

// 游戏状态
enum class GameState {
    MAIN_MENU,
    EXPLORING,
    INVENTORY,
    STORE,
    GAME_OVER
};

struct SkillInfo {
    std::string name;
    std::string description;
    float damageMultiplier;
    int unlockLevel;
    SkillType type;
};

struct BossSkill {
    std::string name;
    std::string description;
    float damageMultiplier;
};

struct SaveFlags {
    bool hasReceivedSupplies; // 是否已领取补给
};

// 属性类型转字符串
inline std::string AttributeTypeToString(AttributeType type) {
    switch (type) {
    case AttributeType::ATTACK: return "攻击力";
    case AttributeType::DEFENSE: return "防御力";
    case AttributeType::AGILITY: return "敏捷度";
    case AttributeType::HP: return "生命值";
    default: return "未知属性";
    }
}

// 物品类型转字符串
inline std::string ItemTypeToString(ItemType type) {
    switch (type) {
    case ItemType::WEAPON: return "武器";
    case ItemType::ARMOR: return "护甲";
    case ItemType::POTION: return "药水";
    case ItemType::MONEY: return "金钱";
    default: return "未知类型";
    }
}

#endif