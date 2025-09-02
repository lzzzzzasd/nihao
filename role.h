#ifndef ROLE_H
#define ROLE_H

#include <string>
#include "global.h"

class CRole {
protected:
    std::string m_name;
    float m_hp;
    float m_max_hp;

public:
    CRole(const std::string& name, float max_hp)
        : m_name(name), m_max_hp(max_hp), m_hp(max_hp) {}

    virtual ~CRole() = default;

    std::string GetName() const { return m_name; }
    float GetHP() const { return m_hp; }
    float GetMaxHP() const { return m_max_hp; }

    void SetHP(float hp) {
        m_hp = hp < 0 ? 0 : (hp > m_max_hp ? m_max_hp : hp);
    }

    bool IsAlive() const { return m_hp > 0; }

    void TakeDamage(float damage) {
        if (damage > 0) {
            m_hp -= damage;
            if (m_hp < 0) m_hp = 0;
        }
    }

    virtual float GetAttack() const = 0;
    virtual float GetDefense() const = 0;
    virtual float GetAgility() const = 0;
    virtual void ShowStatus() const = 0;
};

#endif
