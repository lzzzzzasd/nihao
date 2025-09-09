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
    RANGED_WEAPON,  
    MELEE_WEAPON,   
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
};

// ��������
enum class AttributeType {
    ATTACK,
    DEFENSE,
    AGILITY,
    HP,
};

// ��Ϸ״̬
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
    bool hasReceivedSupplies; // �Ƿ�����ȡ����
};

// ��������ת�ַ���
inline std::string AttributeTypeToString(AttributeType type) {
    switch (type) {
    case AttributeType::ATTACK: return "������";
    case AttributeType::DEFENSE: return "������";
    case AttributeType::AGILITY: return "���ݶ�";
    case AttributeType::HP: return "����ֵ";
    default: return "δ֪����";
    }
}

// ��Ʒ����ת�ַ���
inline std::string ItemTypeToString(ItemType type) {
    switch (type) {
    case ItemType::WEAPON: return "����";
    case ItemType::ARMOR: return "����";
    case ItemType::POTION: return "ҩˮ";
    case ItemType::MONEY: return "��Ǯ";
    default: return "δ֪����";
    }
}

#endif