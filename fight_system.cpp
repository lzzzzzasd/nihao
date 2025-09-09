#include "fight_system.h"
#include <iostream>
#include <algorithm>

CFightSystem::CFightSystem(std::unique_ptr<CPlayer> player, std::unique_ptr<CEnemy> enemy)
    : m_player(std::move(player)), m_enemy(std::move(enemy)), m_isPlayerTurn(true) {
}

void CFightSystem::ShowFightPanel() const {
    std::cout << "------ս�����------\n";
    std::cout << "���: " << m_player->GetName()
        << "  HP: " << m_player->GetHP() << "/" << m_player->GetMaxHP() << "\n";
    std::cout << (m_enemy->IsBoss() ? "BOSS: " : "����: ") << m_enemy->GetName()
        << "  HP: " << m_enemy->GetHP() << "/" << m_enemy->GetMaxHP() << "\n";
    std::cout << "--------------------\n";
}

void CFightSystem::DecideFirstAttacker() {
    m_isPlayerTurn = m_player->GetAgility() >= m_enemy->GetAgility();
}

bool CFightSystem::IsHit(float defenderAgility) {
    float missChance = defenderAgility / 100.0f;
    float randValue = static_cast<float>(rand()) / RAND_MAX;
    return randValue > missChance;
}

float CFightSystem::CalculateDamage(float attackerAtk, float defenderDef, float multiplier) {
    return (attackerAtk / std::max(defenderDef, 1.0f)) * multiplier;
}

void CFightSystem::StartFight() {
    DecideFirstAttacker();
    m_player->ResetSkills();
    if (m_enemy->IsBoss()) m_enemy->UseBossSkill();

    bool bossSkillUsed = false;
    while (m_player->IsAlive() && m_enemy->IsAlive()) {
        ShowFightPanel();
        if (m_isPlayerTurn) {
            int skillIdx = 0;
            std::cout << "ѡ���ܣ�0.��ͨ 1.����1(1.5x) 2.����2(��Ѫ2x�ȼ�) 3.����3(2x)\n";
            std::cin >> skillIdx;
            if (!m_player->CanUseSkill(skillIdx)) {
                std::cout << "����δ���������ù���Ĭ��ʹ����ͨ����\n";
                skillIdx = 0;
            }
            m_player->UseSkill(skillIdx);

            float multiplier = 1.0f;
            if (skillIdx == 1) multiplier = 1.5f;
            else if (skillIdx == 3) multiplier = 2.0f;

            if (skillIdx == 2) {
                float heal = 2 * m_player->GetLevel();
                m_player->SetHP(m_player->GetHP() + heal);
                std::cout << "���ֻ�Ѫ" << heal << "����ֵ\n";
            }
            else {
                if (IsHit(m_enemy->GetAgility())) {
                    float damage = CalculateDamage(m_player->GetAttack(), m_enemy->GetDefense(), multiplier);
                    m_enemy->TakeDamage(damage);
                    std::cout << "�����" << damage << "�˺�\n";
                }
                else {
                    std::cout << "��Ĺ���δ����\n";
                }
            }
            if (!m_enemy->IsAlive()) {
                std::cout << "�����ѱ����ܣ�\n";
                int exp = m_enemy->GetExpReward();
                int gold = m_enemy->GetGoldReward();
                std::cout << "������ " << exp << " ���飬" << gold << " ��ң�\n";
                m_player->AddExp(exp);
                m_player->AddGold(gold);
                m_player->LevelUp();
                system("pause");
                break;
            }
        }
        else {
            if (m_enemy->IsBoss() && !bossSkillUsed) {
                bossSkillUsed = true;
                if (IsHit(m_player->GetAgility())) {
                    float damage = CalculateDamage(m_enemy->GetAttack(), m_player->GetDefense(), m_enemy->GetBossSkill()->damageMultiplier);
                    m_player->TakeDamage(damage);
                    std::cout << "BOSS�����" << damage << "�˺�\n";
                }
                else {
                    std::cout << "BOSS����δ����\n";
                }
            }
            else {
                if (IsHit(m_player->GetAgility())) {
                    float damage = CalculateDamage(m_enemy->GetAttack(), m_player->GetDefense());
                    m_player->TakeDamage(damage);
                    std::cout << "���������" << damage << "�˺�\n";
                }
                else {
                    std::cout << "���˹���δ����\n";
                }
            }
            if (!m_player->IsAlive()) {
                std::cout << "��������...\n";
                break;
            }
        }
        m_isPlayerTurn = !m_isPlayerTurn;
    }
}