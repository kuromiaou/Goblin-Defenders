#pragma once
#include <Termina/Scripting/API/ScriptingAPI.hpp>
#include <algorithm>
#include "GameAssembly/Tiers.hpp"
using namespace TerminaScript;

// ATK       : Null
// ATK_Range : Mid (3)
// ATK_SPD   : Low
// Capacité  : Slow (-1 tier SPD) ou Shred (-1 tier RES)
class TowerCC : public TerminaScript::ScriptableComponent {
public:
    TowerCC() = default;
    TowerCC(Termina::Actor* owner) : TerminaScript::ScriptableComponent(owner) {}

    void Start()  override;
    void Update(float deltaTime) override;
    void Inspect() override;
    void Serialize(nlohmann::json& out) const override;
    void Deserialize(const nlohmann::json& in) override;

    int        getATKRange()   const { return static_cast<int>(atk_range); }
    float      getATKSPD()     const { return static_cast<int>(atk_spd) * 0.1f; }
    CCType     getCCType()     const { return cc_type; }
    DamageType getShredType()  const { return shred_type; }
    AggroMode  getAggroMode()  const { return aggro; }
    bool       isStunned()     const { return stun_timer > 0.0f; }
    int        getLevel()      const { return level; }

    void setAggroMode(AggroMode m)   { aggro      = m; }
    void setCCType(CCType t)         { cc_type    = t; }
    void setShredType(DamageType t)  { shred_type = t; }
    void applyStun(float duration)   { stun_timer = duration; }
    void upgrade();

private:
    TowerRangeTier atk_range  = TowerRangeTier::TOWER_MID;
    ATKSPDTier     atk_spd    = ATKSPDTier::ATKSPD_LOW;
    CCType         cc_type    = CCType::SLOW;
    DamageType     shred_type = DamageType::PHYSIQUE;
    AggroMode      aggro      = AggroMode::FIRST;
    float          stun_timer = 0.0f;
    int            level      = 1;
};
