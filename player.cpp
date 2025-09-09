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
    m_maxhp = 100.0f;
    m_baseAttack = 10.0f;
    m_baseDefense = 1.0f;
    m_baseAgility = 1.0f;
    for (int i = 0; i < 4; ++i) m_skillUsed[i] = false;
    m_rangedWeapon.reset();
    m_meleeWeapon.reset();
    m_armor.reset();
    m_defenseReduction = 0.0f;
}

float CPlayer::GetAttack() const {
    float total = m_baseAttack;
    if (m_rangedWeapon) total += m_rangedWeapon->GetAttackBonus();
    if (m_meleeWeapon) total += m_meleeWeapon->GetAttackBonus();
    return std::min(total, 100.0f);
}
float CPlayer::GetDefense() const {
    float total = m_baseDefense;
    if (m_armor) total += m_armor->GetDefenseBonus();
    return std::min(total, 10.0f);
}
float CPlayer::GetAgility() const {
    float total = m_baseAgility;
    if (m_meleeWeapon) total += m_meleeWeapon->GetAgilityBonus();
    return std::min(total, 50.0f);
}
void CPlayer::AddExp(int exp) { m_exp += exp; }
void CPlayer::AddGold(int gold) { m_gold += gold; }
void CPlayer::LevelUp() {
    while (m_level < 10 && m_exp >= (1 << m_level)) {
        m_exp -= (1 << m_level);
        m_level++;
        m_baseAttack += 1.0f;
        m_baseDefense += 0.1f;
        m_baseAgility += 0.5f;
        std::cout << "升级到" << m_level << "级！\n";
        ResetSkills();
    }
}
void CPlayer::EquipRangedWeapon(std::unique_ptr<Weapon> rangedWeapon) {
    // 如果已有远程武器，先处理旧武器
    m_rangedWeapon = std::move(rangedWeapon);
}

void CPlayer::EquipMeleeWeapon(std::unique_ptr<Weapon> meleeWeapon) {
    m_meleeWeapon = std::move(meleeWeapon);
}

void CPlayer::EquipArmor(std::unique_ptr<Armor> armor) {
    m_armor = std::move(armor);
}
void CPlayer::ShowStatus() const {
    std::cout << "玩家[" << m_name << "] Lv." << m_level << " HP:" << m_hp << "/" << m_maxhp << "\n"
        << "攻击:" << GetAttack() << " 防御:" << GetDefense() << " 敏捷:" << GetAgility() << "\n"
        << "金币:" << m_gold << " 经验:" << m_exp << std::endl;
        std::cout << "=== 装备信息 ===" << std::endl;

    if (m_rangedWeapon) {
        std::cout << "远程武器: " << m_rangedWeapon->GetName()
            << " (攻击+" << m_rangedWeapon->GetAttackBonus();
        if (m_rangedWeapon->GetAgilityBonus() != 0) {
            std::cout << " 敏捷" << (m_rangedWeapon->GetAgilityBonus() > 0 ? "+" : "")
                << m_rangedWeapon->GetAgilityBonus();
        }
        std::cout << ")" << std::endl;
    }
    else {
        std::cout << "远程武器: 无" << std::endl;
    }

    if (m_meleeWeapon) {
        std::cout << "近战武器: " << m_meleeWeapon->GetName()
            << " (攻击+" << m_meleeWeapon->GetAttackBonus();
        if (m_meleeWeapon->GetAgilityBonus() != 0) {
            std::cout << " 敏捷" << (m_meleeWeapon->GetAgilityBonus() > 0 ? "+" : "")
                << m_meleeWeapon->GetAgilityBonus();
        }
        std::cout << ")" << std::endl;
    }
    else {
        std::cout << "近战武器: 无" << std::endl;
    }

    if (m_armor) {
        std::cout << "护甲: " << m_armor->GetName()
            << " (防御+" << m_armor->GetDefenseBonus() << ")" << std::endl;
    }
    else {
        std::cout << "护甲: 无" << std::endl;
    }

    if (m_defenseReduction > 0) {
        std::cout << "防御削减: -" << m_defenseReduction << std::endl;
    }
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
void CPlayer::SetHP(float hp) { m_hp = std::max(0.0f, std::min(hp, m_maxhp)); }

void CPlayer::ApplyDefenseReduction(float reduction) {
    m_defenseReduction += reduction; // 累积削减
    std::cout << "你的防御力被永久削减了 " << reduction << " 点！\n";
}

float CPlayer::GetEffectiveDefense() const {
    float baseDefense = GetDefense();
    return std::max(0.1f, baseDefense - m_defenseReduction); // 确保防御不低于0.1
}