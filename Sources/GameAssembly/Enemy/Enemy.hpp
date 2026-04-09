#pragma once

#include "GameAssembly/Tiers.hpp"
#include <algorithm>

// Base class for all enemy types.
// Provides a virtual interface for towers/traps and shared timed-effect helpers.
class Enemy
{
public:
    Enemy() = default;
    virtual ~Enemy() = default;

    virtual bool isDead()       const { return false; }
    virtual int  getHP()        const { return 0; }
    virtual int  getHPMax()     const { return 0; }
    virtual int  getGoldValue() const { return 0; }
    virtual void takeDamage(int dmg, DamageType type) {}

    void  applyAttackBoost(float duration, float multiplier)
    {
        if (duration <= 0.0f || multiplier <= 1.0f) return;
        attack_boost_timer = std::max(attack_boost_timer, duration);
        attack_boost_mult  = std::max(attack_boost_mult, multiplier);
    }
    float getAttackMultiplier() const { return attack_boost_timer > 0.0f ? attack_boost_mult : 1.0f; }

    void applyResBoost(float duration, int bonus)
    {
        if (duration <= 0.0f || bonus <= 0) return;
        res_boost_timer = std::max(res_boost_timer, duration);
        res_boost_bonus = std::max(res_boost_bonus, bonus);
    }

    // Shred: reduces enemy resistance by one tier for `duration` seconds.
    void  applyShred(float duration)  { shred_timer = std::max(shred_timer, duration); }
    bool  isShredded() const          { return shred_timer > 0.0f; }

protected:
    // Call from the concrete ScriptableComponent::Update to tick timed effects.
    void tickEffects(float deltaTime)
    {
        if (shred_timer > 0.0f)
            shred_timer = std::max(0.0f, shred_timer - deltaTime);
        if (attack_boost_timer > 0.0f)
            attack_boost_timer = std::max(0.0f, attack_boost_timer - deltaTime);
        if (res_boost_timer > 0.0f)
            res_boost_timer = std::max(0.0f, res_boost_timer - deltaTime);
    }

    // Returns the resistance lowered by one tier (used when shredded).
    static int shredResistance(int res)
    {
        if (res >= static_cast<int>(ResTier::RES_HIGH)) return static_cast<int>(ResTier::RES_MID);
        if (res >= static_cast<int>(ResTier::RES_MID))  return static_cast<int>(ResTier::RES_LOW);
        return static_cast<int>(ResTier::RES_LOW);
    }

    int applyResistanceEffects(int res) const
    {
        if (isShredded()) res = shredResistance(res);
        if (res_boost_timer > 0.0f) res += res_boost_bonus;
        return std::max(0, res);
    }

    int applyAttackEffects(int baseAtk) const
    {
        return static_cast<int>(static_cast<float>(baseAtk) * getAttackMultiplier());
    }

    float shred_timer = 0.0f;
    float attack_boost_timer = 0.0f;
    float attack_boost_mult  = 1.0f;
    float res_boost_timer = 0.0f;
    int   res_boost_bonus = 0;
};
