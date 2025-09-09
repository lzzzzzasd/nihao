#ifndef EQUIPMENT_H
#define EQUIPMENT_H

#include <string>
#include "global.h"

class Weapon {
private:
    std::string m_name;
    EquipmentType m_type;
    float m_attack_bonus;
    float m_agility_bonus;

public:
    Weapon(const std::string& name, EquipmentType type, float attack, float agility = 0.0f)
        : m_name(name), m_type(type), m_attack_bonus(attack), m_agility_bonus(agility) {
    }

    std::string GetName() const { return m_name; }
    EquipmentType GetType() const { return m_type; }
    float GetAttackBonus() const { return m_attack_bonus; }
    float GetAgilityBonus() const { return m_agility_bonus; }
};

class Armor {
private:
    std::string m_name;
    float m_defense_bonus;

public:
    Armor(const std::string& name, float defense)
        : m_name(name), m_defense_bonus(defense) {
    }

    std::string GetName() const { return m_name; }
    float GetDefenseBonus() const { return m_defense_bonus; }
};

#endif