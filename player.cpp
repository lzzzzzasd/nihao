#include "player.h"
#include <iostream>
#include <algorithm>

CPlayer::CPlayer(const std::string& name)
    : CRole(name, 100.0f) {
    RestoreToInitialState();
}

void CPlayer::RestoreToInitialState() {
    m_level = 1;
    m_exp = 0;
    m_gold = 0;
    m_hp = 100.0f;
    m_max_hp = 100.0f;
    m_base_attack = 10.0f;
    m_base_defense = 1.0f;
    m_base_agility = 1.0f;
    for (int i = 0; i < 4; ++i) m_skillUsed[i] = false;
    m_gun.reset();
    m_knife.reset();
    m_armor.reset();
}

float CPlayer::GetAttack() const {
    float total = m_base_attack;
    if (m_gun) total += m_gun->GetAttackBonus();
    if (m_knife) total += m_knife->GetAttackBonus();
    return std::min(total, 100.0f);
}
float CPlayer::GetDefense() const {
    float total = m_base_defense;
    if (m_armor) total += m_armor->GetDefenseBonus();
    return std::min(total, 10.0f);
}
float CPlayer::GetAgility() const {
    float total = m_base_agility;
    if (m_knife) total += m_knife->GetAgilityBonus();
    return std::min(total, 50.0f);
}
void CPlayer::AddExp(int exp) { m_exp += exp; }
void CPlayer::AddGold(int gold) { m_gold += gold; }
void CPlayer::LevelUp() {
    while (m_level < 10 && m_exp >= (1 << m_level)) {
        m_exp -= (1 << m_level);
        m_level++;
        m_base_attack += 1.0f;
        m_base_defense += 0.1f;
        m_base_agility += 0.5f;
        std::cout << "升级到" << m_level << "级！\n";
        ResetSkills();
    }
}
void CPlayer::EquipGun(std::unique_ptr<Weapon> gun) { m_gun = std::move(gun); }
void CPlayer::EquipKnife(std::unique_ptr<Weapon> knife) { m_knife = std::move(knife); }
void CPlayer::EquipArmor(std::unique_ptr<Armor> armor) { m_armor = std::move(armor); }
void CPlayer::ShowStatus() const {
    std::cout << "玩家[" << m_name << "] Lv." << m_level << " HP:" << m_hp << "/" << m_max_hp << "\n"
        << "攻击:" << GetAttack() << " 防御:" << GetDefense() << " 敏捷:" << GetAgility() << "\n"
        << "金币:" << m_gold << " 经验:" << m_exp << std::endl;
}
void CPlayer::ResetSkills() { for (int i = 0; i < 4; ++i) m_skillUsed[i] = false; }
bool CPlayer::CanUseSkill(int idx) const {
    if (idx == 0) return true;
    if (m_skillUsed[idx]) return false;
    if (idx == 1) return m_level >= 2;
    if (idx == 2) return m_level >= 6;
    if (idx == 3) return m_level >= 10;
    return false;
}
void CPlayer::UseSkill(int idx) { if (idx >= 0 && idx < 4) m_skillUsed[idx] = true; }
void CPlayer::SetHP(float hp) { m_hp = std::max(0.0f, std::min(hp, m_max_hp)); }