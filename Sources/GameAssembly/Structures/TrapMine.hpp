#pragma once
#include <Termina/Scripting/API/ScriptingAPI.hpp>
#include <algorithm>
#include "GameAssembly/Tiers.hpp"
#include "Trap.hpp"
using namespace TerminaScript;

// Mine — ATK High (Physique)
// Usage unique, explose au contact
class TrapMine : public Trap, public TerminaScript::ScriptableComponent {
public:
    TrapMine() = default;
    TrapMine(Termina::Actor* owner) : TerminaScript::ScriptableComponent(owner) {}

    void Start()  override;
    void Update(float deltaTime) override;
    void Inspect() override;
    void Serialize(nlohmann::json& out) const override;
    void Deserialize(const nlohmann::json& in) override;

    int        getATK()        const { return atk; }
    DamageType getDamageType() const { return damage_type; }
    bool       isTriggered()   const { return triggered; }

    // Retourne les dégâts à appliquer, 0 si déjà déclenchée
    int trigger();

private:
    int        atk         = static_cast<int>(ATKTier::ATK_HIGH); // 25
    DamageType damage_type = DamageType::PHYSIQUE;
    bool       triggered   = false;
};
