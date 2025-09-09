#include "fight_system.h"
#include <iostream>
#include <algorithm>

CFightSystem::CFightSystem(std::unique_ptr<CPlayer> player, std::unique_ptr<CEnemy> enemy, Storage& inventory)
    : m_player(std::move(player)), m_enemy(std::move(enemy)), m_inventory(inventory),
    m_isPlayerTurn(true), m_dropItem("") {
}

void CFightSystem::ShowFightPanel() const {
    std::cout << "------战斗面板------\n";
    // 只显示名字和生命值（不显示攻击、防御、敏捷）
    std::cout << "玩家: " << m_player->GetName()
        << "  HP: " << m_player->GetHP() << "/" << m_player->GetMaxHP() << "\n";
    std::cout << (m_enemy->IsBoss() ? "BOSS: " : "怪物: ") << m_enemy->GetName()
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
    return (attackerAtk / std::max(defenderDef, 0.1f)) * multiplier;
}

bool CFightSystem::UsePotionInBattle() {
    if (m_inventory.hasItem("强效药水")) {
        auto potion = m_inventory.takeItem("强效药水");
        if (potion) {
            int heal = potion->getAttribute(AttributeType::HP);
            float newHp = m_player->GetHP() + heal;
            m_player->SetHP(std::min(newHp, m_player->GetMaxHP()));
            std::cout << "战斗中使用了强效药水，恢复" << heal << "点生命值！\n";
            return true;
        }
    }
    std::cout << "没有强效药水可以使用！\n";
    return false;
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
            std::cout << "选择技能：0.普通 1.技能1(1.5x) 2.技能2(回血2x等级) 3.技能3(2x) 4. 使用强效药水\n";
            std::cin >> skillIdx;
            if (skillIdx == 4) {
                if (UsePotionInBattle()) {
                    m_isPlayerTurn = !m_isPlayerTurn;
                    continue;
                }
                else {
                    continue;
                }
            }
            if (!m_player->CanUseSkill(skillIdx)) {
                std::cout << "技能未解锁或已用过，默认使用普通攻击\n";
                skillIdx = 0;
            }
            m_player->UseSkill(skillIdx);

            float multiplier = 1.0f;
            if (skillIdx == 1) multiplier = 1.5f;
            else if (skillIdx == 3) multiplier = 2.0f;

            if (skillIdx == 2) {
                float heal = 2 * m_player->GetLevel();
                m_player->SetHP(m_player->GetHP() + heal);
                std::cout << "本轮回血" << heal << "生命值\n";
            }
            else {
                if (IsHit(m_enemy->GetAgility())) {
                    float damage = CalculateDamage(m_player->GetAttack(), m_enemy->GetDefense(), multiplier);
                    m_enemy->TakeDamage(damage);
                    std::cout << "造成了" << damage << "伤害\n";
                }
                else {
                    std::cout << "你的攻击未命中\n";
                }
            }
            if (!m_enemy->IsAlive()) {
                std::cout << "敌人已被击败！\n";
                int exp = m_enemy->GetExpReward();
                int gold = m_enemy->GetGoldReward();
                std::cout << "你获得了 " << exp << " 经验，" << gold << " 金币！\n";
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
                    float damage = CalculateDamage(m_enemy->GetAttack(), m_player->GetEffectiveDefense(), m_enemy->GetBossSkill()->damageMultiplier);
                    m_player->TakeDamage(damage);
                    std::cout << "BOSS造成了" << damage << "伤害\n";

                    if (m_enemy->GetName() == "沙漠暴君" && m_enemy->GetDefenseReduction() > 0) {
                        m_player->ApplyDefenseReduction(m_enemy->GetDefenseReduction());
                    }
                }
                else {
                    std::cout << "BOSS攻击未命中\n";
                }
            }
            else {
                if (IsHit(m_player->GetAgility())) {
                    float damage = CalculateDamage(m_enemy->GetAttack(), m_player->GetEffectiveDefense());
                    m_player->TakeDamage(damage);
                    std::cout << "敌人造成了" << damage << "伤害\n";
                }
                else {
                    std::cout << "敌人攻击未命中\n";
                }
            }
            if (!m_player->IsAlive()) {
                std::cout << "你已阵亡...\n";
                break;
            }
        }
        m_isPlayerTurn = !m_isPlayerTurn;
    }
    if (!m_enemy->IsAlive() && m_enemy->IsBoss()) {
        // 获取BOSS掉落物品
        m_dropItem = m_enemy->GetBossDropItem();
        if (!m_dropItem.empty()) {
            std::cout << "BOSS掉落了一件物品: " << m_dropItem << "！\n";
        }
    }
}