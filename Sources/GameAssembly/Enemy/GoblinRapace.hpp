#pragma once
#include <Termina/Scripting/API/ScriptingAPI.hpp>
#include <algorithm>
#include "GameAssembly/Tiers.hpp"
using namespace TerminaScript;

// HP  : Mid/High
// ATK : Null
// SPD : High
// RES : Mid / Mid
// Capacité : ramasse le Gold au sol, Gold perdu si atteint le Nexus
class GoblinRapace : public TerminaScript::ScriptableComponent {
public:
    GoblinRapace() = default;
    GoblinRapace(Termina::Actor* owner) : TerminaScript::ScriptableComponent(owner) {}

    void Start()  override;
    void Update(float deltaTime) override;
    void Inspect() override;
    void Serialize(nlohmann::json& out) const override;
    void Deserialize(const nlohmann::json& in) override;

    int  getHP()           const { return hp; }
    int  getHPMax()        const { return hp_max; }
    float getSPD()         const { return static_cast<int>(spd) * 0.1f; }
    int  getStolenGold()   const { return stolen_gold; }
    bool isDead()          const { return hp <= 0; }
    bool hasReachedNexus() const { return reached_nexus; }

    // Gold_Value = (50% HP_MAX + Gold volé) * 1.2
    // Si le Goblin atteint le Nexus : 0
    int getGoldValue() const;

    void stealGold(int amount) { stolen_gold += amount; }
    void onReachNexus()        { reached_nexus = true; }
    void initForWave(int current_wave);
    void takeDamage(int dmg, DamageType type);

private:
    int     hp            = 35;
    int     hp_max        = 35;
    int     wave          = 1;
    int     stolen_gold   = 0;
    bool    reached_nexus = false;
    SPDTier spd           = SPDTier::SPD_HIGH;
    ResTier res_physique  = ResTier::RES_MID;
    ResTier res_magique   = ResTier::RES_MID;
};
