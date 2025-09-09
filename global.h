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
    GUN,
    KNIFE,
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
    OTHER
};

// 属性类型
enum class AttributeType {
    ATTACK,
    DEFENSE,
    AGILITY,
    HP,
    OTHER
};

// 游戏状态
enum class GameState {
    MAIN_MENU,
    EXPLORING,
    INVENTORY,
    STORE,
    GAME_OVER,
    SAVE_MENU       // 新增存档菜单状态
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

#endif
