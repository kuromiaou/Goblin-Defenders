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

    // Shred: reduces enemy resistance by one tier for `duration` seconds.
    void  applyShred(float duration)  { shred_timer = std::max(shred_timer, duration); }
    bool  isShredded() const          { return shred_timer > 0.0f; }

protected:
    // Call from the concrete ScriptableComponent::Update to tick timed effects.
    void tickEffects(float deltaTime)
    {
        if (shred_timer > 0.0f)
            shred_timer = std::max(0.0f, shred_timer - deltaTime);
    }

    // Returns the resistance lowered by one tier (used when shredded).
    static int shredResistance(int res)
    {
        if (res >= static_cast<int>(ResTier::RES_HIGH)) return static_cast<int>(ResTier::RES_MID);
        if (res >= static_cast<int>(ResTier::RES_MID))  return static_cast<int>(ResTier::RES_LOW);
        return static_cast<int>(ResTier::RES_LOW);
    }

    float shred_timer = 0.0f;
};