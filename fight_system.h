#pragma once
#ifndef FIGHT_SYSTEM_H
#define FIGHT_SYSTEM_H

#include <memory>
#include "player.h"
#include "enemy.h"
#include "global.h"
#include "storage.h"

class CFightSystem {
private:
    std::unique_ptr<CPlayer> m_player;
    std::unique_ptr<CEnemy> m_enemy;
    Storage& m_inventory;
    bool m_isPlayerTurn;
    std::string m_dropItem;

public:
    CFightSystem(std::unique_ptr<CPlayer> player, std::unique_ptr<CEnemy> enemy, Storage& inventory);
    ~CFightSystem(){}

    std::unique_ptr<CPlayer> releasePlayer() { return std::move(m_player); }
    void StartFight();
    void ShowFightPanel() const;
    void DecideFirstAttacker();
    bool IsHit(float defenderAgility);
    float CalculateDamage(float attackerAtk, float defenderDef, float multiplier = 1.0f);
    bool UsePotionInBattle();
    std::string GetDropItem() const { return m_dropItem; }
};

#endif