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
    float m_base_attack;
    float m_base_defense;
    float m_base_agility;
    bool m_skillUsed[4]; // ÆÕ¹¥+3¼¼ÄÜ
    std::unique_ptr<Weapon> m_gun;
    std::unique_ptr<Weapon> m_knife;
    std::unique_ptr<Armor> m_armor;

public:
    CPlayer(const std::string& name);

    void RestoreToInitialState();
    float GetAttack() const override;
    float GetDefense() const override;
    float GetAgility() const override;
    float GetRawAttack() const { return m_base_attack; }
    float GetRawDefense() const { return m_base_defense; }
    float GetRawAgility() const { return m_base_agility; }
    int GetLevel() const { return m_level; }
    int GetExp() const { return m_exp; }
    int GetGold() const { return m_gold; }

    void AddExp(int exp);
    void AddGold(int gold);
    void LevelUp();

    void EquipGun(std::unique_ptr<Weapon> gun);
    void EquipKnife(std::unique_ptr<Weapon> knife);
    void EquipArmor(std::unique_ptr<Armor> armor);

    void ShowStatus() const override;

    void ResetSkills();
    bool CanUseSkill(int idx) const;
    void UseSkill(int idx);
    void SetHP(float hp);
};

#endif
