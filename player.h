#ifndef PLAYER_H
#define PLAYER_H

#include <string>
#include <memory>
#include "role.h"
#include "equipment.h"

class CPlayer : public CRole {
private:
    int m_level;
    int m_exp;
    int m_gold;
    float m_baseAttack;
    float m_baseDefense;
    float m_baseAgility;
    bool m_skillUsed[4]; // 普攻+3技能
    std::unique_ptr<Weapon> m_rangedWeapon;
    std::unique_ptr<Weapon> m_meleeWeapon;
    std::unique_ptr<Armor> m_armor;
    float m_defenseReduction;

public:
    CPlayer(const std::string& name);

    void RestoreToInitialState();
    float GetAttack() const override;
    float GetDefense() const override;
    float GetAgility() const override;
    float GetRawAttack() const { return m_baseAttack; }
    float GetRawDefense() const { return m_baseDefense; }
    float GetRawAgility() const { return m_baseAgility; }
    int GetLevel() const { return m_level; }
    int GetExp() const { return m_exp; }
    int GetGold() const { return m_gold; }

    void AddExp(int exp);
    void AddGold(int gold);
    void LevelUp();

    void EquipRangedWeapon(std::unique_ptr<Weapon> rangedWeapon);
    void EquipMeleeWeapon(std::unique_ptr<Weapon> meleeWeapon);
    void EquipArmor(std::unique_ptr<Armor> armor);

    void ShowStatus() const override;

    void ResetSkills();
    bool CanUseSkill(int idx) const;
    void UseSkill(int idx);
    void SetHP(float hp);
    void SetLevel(int level) { m_level = level; }
    void SetExp(int exp) { m_exp = exp; }
    void SetGold(int gold) { m_gold = gold; }
    void SetMaxHP(float maxhp) { m_maxhp = maxhp; }
    void SetBaseStats(float attack, float defense, float agility) {
        m_baseAttack = attack;
        m_baseDefense = defense;
        m_baseAgility = agility;
    }

    //boss技能削减防御
    void ApplyDefenseReduction(float reduction);
    float GetEffectiveDefense() const;

    // 获取装备信息
    const Weapon* GetRangedWeapon() const { return m_rangedWeapon.get(); }
    const Weapon* GetMeleeWeapon() const { return m_meleeWeapon.get(); }
    const Armor* GetArmor() const { return m_armor.get(); }

    // 装备检查
    bool HasRangedWeapon() const { return m_rangedWeapon != nullptr; }
    bool HasMeleeWeapon() const { return m_meleeWeapon != nullptr; }
    bool HasArmor() const { return m_armor != nullptr; }
};

#endif
