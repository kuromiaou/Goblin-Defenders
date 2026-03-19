#pragma once
#include <Termina/Scripting/API/ScriptingAPI.hpp>
#include <algorithm>
#include "Tiers.hpp"
using namespace TerminaScript;

// Piège à Loup — STUN 0.5s -> 1.5s
// Usage unique, placé librement sur la map
class TrapWolf : public TerminaScript::ScriptableComponent {
public:
    TrapWolf() = default;
    TrapWolf(Termina::Actor* owner) : TerminaScript::ScriptableComponent(owner) {}

    void Start()  override;
    void Update(float deltaTime) override;
    void Inspect() override;
    void Serialize(nlohmann::json& out) const override;
    void Deserialize(const nlohmann::json& in) override;

    bool  isTriggered()       const { return triggered; }
    float getStunDuration()   const;
    float getStunMin()        const { return stun_min; }
    float getStunMax()        const { return stun_max; }

    // Appelé quand un ennemi marche dessus
    // Retourne la durée du stun à appliquer, 0 si déjà déclenché
    float trigger();

private:
    bool  triggered = false;
    float stun_min  = 0.5f;
    float stun_max  = 1.5f;
};
