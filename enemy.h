#pragma once
#ifndef ENEMY_H
#define ENEMY_H

#include <string>
#include "role.h"
#include "global.h"

class CEnemy : public CRole {
private:
    EnemyType m_type;
    float m_attack;
    float m_defense;
    float m_agility;
    int m_expReward;
    int m_goldReward;
    BossSkill m_bossSkill;
    bool m_bossSkillUsed;

public:
    // С�ֹ��죺ֱ��ָ������ֵ
    CEnemy(const std::string& name, float hp, float attack, float defense, float agility)
        : CRole(name, hp),
        m_type(EnemyType::MONSTER),
        m_attack(attack), m_defense(defense), m_agility(agility),
        m_expReward(1), m_goldReward(2), m_bossSkillUsed(false) {
    }

    // BOSS���죺ֱ��ָ������ֵ
    CEnemy(const std::string& name, float hp, float attack, float defense, float agility,
        const std::string& skillName, float skillMultiplier, const std::string& skillDesc)
        : CRole(name, hp),
        m_type(EnemyType::BOSS),
        m_attack(attack), m_defense(defense), m_agility(agility),
        m_expReward(4), m_goldReward(10), m_bossSkillUsed(false) {
        m_bossSkill = { skillName, skillDesc, skillMultiplier };
    }

    EnemyType GetType() const { return m_type; }
    int GetExpReward() const { return m_expReward; }
    int GetGoldReward() const { return m_goldReward; }
    bool IsBoss() const { return m_type == EnemyType::BOSS; }
    const BossSkill* GetBossSkill() const { return IsBoss() ? &m_bossSkill : nullptr; }
    bool CanUseBossSkill() const { return IsBoss() && !m_bossSkillUsed; }
    void UseBossSkill() { m_bossSkillUsed = true; }
    float GetAttack() const override { return m_attack; }
    float GetDefense() const override { return m_defense; }
    float GetAgility() const override { return m_agility; }
    void ShowStatus() const override {
        std::cout << (IsBoss() ? "BOSS: " : "����: ") << m_name << std::endl;
        std::cout << "����ֵ: " << m_hp << "/" << m_max_hp << std::endl;
        std::cout << "����: " << m_attack << " ����: " << m_defense << " ����: " << m_agility << std::endl;
        if (IsBoss()) {
            std::cout << "����: " << m_bossSkill.name << " - " << m_bossSkill.description << std::endl;
        }
        std::cout << "����: " << m_expReward << "����, " << m_goldReward << "���" << std::endl;
    }
};

#endif