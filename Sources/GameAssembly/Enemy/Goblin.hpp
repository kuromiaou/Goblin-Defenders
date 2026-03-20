#pragma once
#include <Termina/Scripting/API/ScriptingAPI.hpp>
#include <algorithm>
#include "GameAssembly/Tiers.hpp"
using namespace TerminaScript;

// HP  : Low (10) x wave scaling
// ATK : Low
// SPD : Mid
// RES : Low / Low
// Capacité : spawn en horde (3-5)
class Goblin : public Enemy, public TerminaScript::ScriptableComponent {
public:
    Goblin() = default;
    Goblin(Termina::Actor* owner) : TerminaScript::ScriptableComponent(owner) {}

    void Start()  override;
    void Update(float deltaTime) override;
    void Inspect() override;
    void Serialize(nlohmann::json& out) const override;
    void Deserialize(const nlohmann::json& in) override;

    // --- Stats ---
    int  getHP()         const { return hp; }
    int  getHPMax()      const { return hp_max; }
    int  getATK()        const { return static_cast<int>(atk); }
    float getSPD()       const { return static_cast<int>(spd) * 0.1f; }
    int  getGoldValue()  const { return gold_value; }
    int  getWave()       const { return wave; }
    bool isDead()        const { return hp <= 0; }

    void initForWave(int current_wave);
    void takeDamage(int dmg, DamageType type);

    static int getHordeSize() { return 3 + (rand() % 3); } // 3 à 5

private:
    void computeGoldValue() { gold_value = static_cast<int>(hp_max * 0.5f); }

    int        hp         = 10;
    int        hp_max     = 10;
    int        wave       = 1;
    int        gold_value = 5;
    ATKTier    atk        = ATKTier::ATK_LOW;
    SPDTier    spd        = SPDTier::SPD_MID;
    ResTier    res_physique = ResTier::RES_LOW;
    ResTier    res_magique  = ResTier::RES_LOW;
};
