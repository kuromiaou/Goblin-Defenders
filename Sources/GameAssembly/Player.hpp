#pragma once
#include <Termina/Scripting/API/ScriptingAPI.hpp>
#include <algorithm>
#include "Tiers.hpp"
using namespace TerminaScript;

class Player : public TerminaScript::ScriptableComponent {
public:
    Player() = default;
    Player(Termina::Actor* owner) : TerminaScript::ScriptableComponent(owner) {}

    void Start()  override;
    void Update(float deltaTime) override;
    void Inspect() override;
    void Serialize(nlohmann::json& out) const override;
    void Deserialize(const nlohmann::json& in) override;

    // --- HP ---
    int  getHP()    const { return hp; }
    int  getHPMax() const { return hp_max; }
    bool isDead()   const { return hp <= 0; }
    void setHP(int v)    { hp     = std::clamp(v, 0, hp_max); }
    void setHPMax(int v) { hp_max = std::max(0, v); }
    void heal(int v)     { hp     = std::min(hp_max, hp + v); }
    void takeDamage(int dmg, DamageType type);

    // --- ATK ---
    int        getATKPhysique()       const { return atk_physique; }
    int        getATKMagique()        const { return atk_magique; }
    int        getCurrentATK()        const;
    DamageType getCurrentDamageType() const { return current_damage_type; }
    void setATKPhysique(int v) { atk_physique = v; }
    void setATKMagique(int v)  { atk_magique  = v; }
    void switchToPhysique();
    void switchToMagique();

    // --- SPD ---
    float        getSPD()          const { return static_cast<int>(spd) * 0.1f; }
    float        getATKSPD()       const { return static_cast<int>(atk_spd) * 0.1f; }
    int          getATKRange()     const { return static_cast<int>(atk_range); }
    SPDTier      getSPDTier()      const { return spd; }
    ATKSPDTier   getATKSPDTier()   const { return atk_spd; }
    ATKRangeTier getATKRangeTier() const { return atk_range; }
    void setSPD(SPDTier t)           { spd       = t; }
    void setATKSPD(ATKSPDTier t)     { atk_spd   = t; }
    void setATKRange(ATKRangeTier t)  { atk_range = t; }

    // --- Résistances ---
    ResTier getResPhysique() const { return res_physique; }
    ResTier getResMagique()  const { return res_magique; }
    void setResPhysique(ResTier t) { res_physique = t; }
    void setResMagique(ResTier t)  { res_magique  = t; }

    // --- Économie ---
    int  getRSC()  const { return rsc; }
    int  getGold() const { return gold; }
    void setRSC(int v)  { rsc  = std::max(0, v); }
    void setGold(int v) { gold = std::max(0, v); }
    void addGold(int amount) { gold += amount; }
    void addRSC(int amount)  { rsc  += amount; }
    bool spendGold(int amount);
    bool spendRSC(int cost);

private:
    int          hp                  = 100;
    int          hp_max              = 100;
    int          atk_physique        = 10;
    int          atk_magique         = 10;
    DamageType   current_damage_type = DamageType::PHYSIQUE;
    SPDTier      spd                 = SPDTier::SPD_HIGH;
    ATKSPDTier   atk_spd             = ATKSPDTier::ATKSPD_LOW;
    ATKRangeTier atk_range           = ATKRangeTier::MELEE;
    ResTier      res_physique        = ResTier::RES_LOW;
    ResTier      res_magique         = ResTier::RES_LOW;
    int          rsc                 = 0;
    int          gold                = 0;
};
