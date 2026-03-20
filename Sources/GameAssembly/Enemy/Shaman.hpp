#pragma once
#include <Termina/Scripting/API/ScriptingAPI.hpp>
#include <algorithm>
#include "GameAssembly/Tiers.hpp"
using namespace TerminaScript;

// HP  : Mid
// ATK : Null
// SPD : Mid
// RES : Mid / Mid
// Capacité : Aura (ATK_BOOST / SPD_BOOST / RES_BOOST) sur les alliés proches
class Shaman : public TerminaScript::ScriptableComponent {
public:
    Shaman() = default;
    Shaman(Termina::Actor* owner) : TerminaScript::ScriptableComponent(owner) {}

    void Start()  override;
    void Update(float deltaTime) override;
    void Inspect() override;
    void Serialize(nlohmann::json& out) const override;
    void Deserialize(const nlohmann::json& in) override;

    int      getHP()        const { return hp; }
    int      getHPMax()     const { return hp_max; }
    float    getSPD()       const { return static_cast<int>(spd) * 0.1f; }
    int      getGoldValue() const { return gold_value; }
    AuraType getAuraType()  const { return aura_type; }
    float    getAuraSize()  const { return aura_size; }
    bool     isDead()       const { return hp <= 0; }

    void initForWave(int current_wave);
    void takeDamage(int dmg, DamageType type);

private:
    void computeGoldValue() { gold_value = static_cast<int>(hp_max * 0.5f); }

    int      hp           = 25;
    int      hp_max       = 25;
    int      wave         = 1;
    int      gold_value   = 12;
    float    aura_size    = 3.0f;
    AuraType aura_type    = AuraType::ATK_BOOST;
    SPDTier  spd          = SPDTier::SPD_MID;
    ResTier  res_physique = ResTier::RES_MID;
    ResTier  res_magique  = ResTier::RES_MID;
};
