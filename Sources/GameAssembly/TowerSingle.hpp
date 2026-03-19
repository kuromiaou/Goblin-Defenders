#pragma once
#include <Termina/Scripting/API/ScriptingAPI.hpp>
#include <algorithm>
#include "Tiers.hpp"
using namespace TerminaScript;

// ATK       : High
// ATK_Range : High (5)
// ATK_SPD   : High
// Type      : Physique ou Magique
class TowerSingle : public TerminaScript::ScriptableComponent {
public:
    TowerSingle() = default;
    TowerSingle(Termina::Actor* owner) : TerminaScript::ScriptableComponent(owner) {}

    void Start()  override;
    void Update(float deltaTime) override;
    void Inspect() override;
    void Serialize(nlohmann::json& out) const override;
    void Deserialize(const nlohmann::json& in) override;

    int        getATK()        const { return static_cast<int>(atk); }
    int        getATKRange()   const { return static_cast<int>(atk_range); }
    float      getATKSPD()     const { return static_cast<int>(atk_spd) * 0.1f; }
    DamageType getDamageType() const { return damage_type; }
    AggroMode  getAggroMode()  const { return aggro; }
    bool       isStunned()     const { return stun_timer > 0.0f; }
    int        getLevel()      const { return level; }

    void setAggroMode(AggroMode m) { aggro = m; }
    void setDamageType(DamageType t) { damage_type = t; }
    void applyStun(float duration)   { stun_timer = duration; }
    void upgrade();

private:
    ATKTier       atk         = ATKTier::ATK_HIGH;
    TowerRangeTier atk_range  = TowerRangeTier::TOWER_HIGH;
    ATKSPDTier    atk_spd     = ATKSPDTier::ATKSPD_HIGH;
    DamageType    damage_type = DamageType::PHYSIQUE;
    AggroMode     aggro       = AggroMode::FIRST;
    float         stun_timer  = 0.0f;
    int           level       = 1;
};
