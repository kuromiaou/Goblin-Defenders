#pragma once
#include <Termina/Scripting/API/ScriptingAPI.hpp>
#include <algorithm>
#include "GameAssembly/Tiers.hpp"
#include "Tower.hpp"
using namespace TerminaScript;

// ATK       : High
// ATK_Range : High (5)
// ATK_SPD   : High
// Type      : Physique ou Magique
class TowerSingle : public Tower , public TerminaScript::ScriptableComponent {
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
    int        getLevel()      const { return level; }

    bool       isStunned()     const { return stun_timer > 0.0f; }

    void setAggroMode(AggroMode m) { aggro = m; }
    void setDamageType(DamageType t) { damage_type = t; }
    void applyStun(float duration) { stun_timer = std::max(stun_timer, duration); }
    void upgrade();

private:
    ATKTier       atk         = ATKTier::ATK_HIGH;
    TowerRangeTier atk_range  = TowerRangeTier::TOWER_HIGH;
    ATKSPDTier    atk_spd     = ATKSPDTier::ATKSPD_HIGH;
    DamageType    damage_type = DamageType::PHYSIQUE;
    AggroMode     aggro       = AggroMode::FIRST;
    int           level       = 1;
    float         stun_timer  = 0.0f;
    float         attack_cooldown = 0.0f;
};