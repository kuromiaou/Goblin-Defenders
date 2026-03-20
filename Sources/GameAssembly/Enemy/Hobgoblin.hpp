#pragma once
#include <Termina/Scripting/API/ScriptingAPI.hpp>
#include <algorithm>
#include "GameAssembly/Tiers.hpp"
using namespace TerminaScript;

// HP  : High (50) x wave scaling
// ATK : Mid — Melee
// SPD : Low
// RES : High / Mid
// Capacité : x% de chance de nullifier les dégâts
class Hobgoblin : public TerminaScript::ScriptableComponent {
public:
    Hobgoblin() = default;
    Hobgoblin(Termina::Actor* owner) : TerminaScript::ScriptableComponent(owner) {}

    void Start()  override;
    void Update(float deltaTime) override;
    void Inspect() override;
    void Serialize(nlohmann::json& out) const override;
    void Deserialize(const nlohmann::json& in) override;

    int   getHP()          const { return hp; }
    int   getHPMax()       const { return hp_max; }
    int   getATK()         const { return static_cast<int>(atk); }
    float getSPD()         const { return static_cast<int>(spd) * 0.1f; }
    int   getGoldValue()   const { return gold_value; }
    float getNullifyChance()const { return nullify_chance; }
    bool  isDead()         const { return hp <= 0; }

    void initForWave(int current_wave);
    void takeDamage(int dmg, DamageType type);

private:
    void computeGoldValue() { gold_value = static_cast<int>(hp_max * 0.5f); }
    bool rollNullify() const { return (static_cast<float>(rand()) / RAND_MAX) < nullify_chance; }

    int        hp             = 50;
    int        hp_max         = 50;
    int        wave           = 1;
    int        gold_value     = 25;
    float      nullify_chance = 0.20f;
    ATKTier    atk            = ATKTier::ATK_MID;
    SPDTier    spd            = SPDTier::SPD_LOW;
    ResTier    res_physique   = ResTier::RES_HIGH;
    ResTier    res_magique    = ResTier::RES_MID;
};
