#pragma once
#include <Termina/Scripting/API/ScriptingAPI.hpp>
#include <algorithm>
#include "GameAssembly/Tiers.hpp"
using namespace TerminaScript;

// HP  : Mid — Range
// ATK : Mid (Magique)
// SPD : Mid
// RES : Low / High
// Capacité : STUN les tours 0.5->1s après sa mort
class Magicien : public Enemy, public TerminaScript::ScriptableComponent {
public:
    Magicien() = default;
    Magicien(Termina::Actor* owner) : TerminaScript::ScriptableComponent(owner) {}

    void Start()  override;
    void Update(float deltaTime) override;
    void Inspect() override;
    void Serialize(nlohmann::json& out) const override;
    void Deserialize(const nlohmann::json& in) override;

    int   getHP()        const { return hp; }
    int   getHPMax()     const { return hp_max; }
    int   getATK()       const { return static_cast<int>(atk); }
    float getSPD()       const { return static_cast<int>(spd) * 0.1f; }
    int   getGoldValue() const { return gold_value; }
    bool  isDead()       const { return hp <= 0; }

    // Retourne la durée de stun à appliquer aux tours à la mort
    float getDeathStunDuration() const;

    void initForWave(int current_wave);
    void takeDamage(int dmg, DamageType type);

private:
    void computeGoldValue() { gold_value = static_cast<int>(hp_max * 0.5f); }

    int     hp           = 25;
    int     hp_max       = 25;
    int     wave         = 1;
    int     gold_value   = 12;
    float   stun_min     = 0.5f;
    float   stun_max     = 1.0f;
    ATKTier atk          = ATKTier::ATK_MID;
    SPDTier spd          = SPDTier::SPD_MID;
    ResTier res_physique = ResTier::RES_LOW;
    ResTier res_magique  = ResTier::RES_HIGH;
};
