#pragma once
#include <Termina/Scripting/API/ScriptingAPI.hpp>
#include <algorithm>
#include "GameAssembly/Tiers.hpp"
using namespace TerminaScript;

// Aura Gold — Si un ennemi meurt dans la zone, bonus Gold au joueur
// Bonus : entre 50% et 100% des HP_MAX de l'ennemi
// Reste actif toute la vague (non consumé)
class TrapAuraGold : public TerminaScript::ScriptableComponent {
public:
    TrapAuraGold() = default;
    TrapAuraGold(Termina::Actor* owner) : TerminaScript::ScriptableComponent(owner) {}

    void Start()  override;
    void Update(float deltaTime) override;
    void Inspect() override;
    void Serialize(nlohmann::json& out) const override;
    void Deserialize(const nlohmann::json& in) override;

    float getAuraRadius()        const { return aura_radius; }
    float getGoldMultiplierMin() const { return gold_mult_min; }
    float getGoldMultiplierMax() const { return gold_mult_max; }

    void setAuraRadius(float r) { aura_radius = r; }

    // Calcule le bonus Gold pour un ennemi mort dans la zone
    int computeGoldBonus(int enemy_hp_max) const;

private:
    float aura_radius  = 3.0f;
    float gold_mult_min = 0.5f;  // +50% HP_MAX
    float gold_mult_max = 1.0f;  // +100% HP_MAX
};
