#ifndef GLOBAL_H
#define GLOBAL_H

#include <string>
#include <iostream>

// ս�����
enum class FightResult {
    CONTINUE,
    PLAYER_WIN,
    ENEMY_WIN
};

// ��������
enum class SkillType {
    NORMAL_ATTACK,
    SKILL_1,
    SKILL_2,
    SKILL_3,
    BOSS_SKILL
};

// װ������
enum class EquipmentType {
    GUN,
    KNIFE,
    ARMOR
};

// ��������
enum class EnemyType {
    MONSTER,
    BOSS
};

// ��Ʒ����
enum class ItemType {
    WEAPON,
    ARMOR,
    POTION,
    MONEY,
    OTHER
};

// ��������
enum class AttributeType {
    ATTACK,
    DEFENSE,
    AGILITY,
    HP,
    OTHER
};

// ��Ϸ״̬
enum class GameState {
    MAIN_MENU,
    EXPLORING,
    INVENTORY,
    STORE,
    GAME_OVER,
    SAVE_MENU       // �����浵�˵�״̬
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
