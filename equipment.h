#ifndef EQUIPMENT_H
#define EQUIPMENT_H

#include <string>
#include "global.h"

class Weapon {
private:
    std::string m_name;
    EquipmentType m_type;
    float m_attackBonus;
    float m_agilityBonus;

public:
    Weapon(const std::string& name, EquipmentType type, float attack, float agility = 0.0f)
        : m_name(name), m_type(type), m_attackBonus(attack), m_agilityBonus(agility) {
    }

    std::string GetName() const { return m_name; }
    EquipmentType GetType() const { return m_type; }
    float GetAttackBonus() const { return m_attackBonus; }
    float GetAgilityBonus() const { return m_agilityBonus; }
};

class Armor {
private:
    std::string m_name;
    float m_defenseBonus;

public:
    Armor(const std::string& name, float defense)
        : m_name(name), m_defenseBonus(defense) {
    }

    std::string GetName() const { return m_name; }
    float GetDefenseBonus() const { return m_defenseBonus; }
};

#endif