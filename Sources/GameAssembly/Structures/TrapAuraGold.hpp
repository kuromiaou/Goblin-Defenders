#pragma once

#include <Termina/Scripting/API/ScriptingAPI.hpp>
#include "GameAssembly/Tiers.hpp"
#include "Trap.hpp"
#include <set>
using namespace TerminaScript;

// Aura Gold — passive trap: when any enemy dies inside the aura, award a gold bonus.
class TrapAuraGold : public Trap, public TerminaScript::ScriptableComponent {
public:
    TrapAuraGold() = default;
    TrapAuraGold(Termina::Actor* owner) : TerminaScript::ScriptableComponent(owner) {}

    void Start()  override;
    void Update(float deltaTime) override;
    void Inspect() override;
    void Serialize(nlohmann::json& out) const override;
    void Deserialize(const nlohmann::json& in) override;

    int computeGoldBonus(int enemy_hp_max) const;

private:
    float aura_radius   = 3.0f;
    float gold_mult_min = 0.1f;
    float gold_mult_max = 0.3f;

    // Tracks actors already awarded to prevent double-counting.
    std::set<Termina::Actor*> m_AwardedActors;
};
