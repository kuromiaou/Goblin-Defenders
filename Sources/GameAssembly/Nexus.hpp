#pragma once
#include <Termina/Scripting/API/ScriptingAPI.hpp>
#include <algorithm>
#include "Tiers.hpp"
using namespace TerminaScript;

class Nexus : public TerminaScript::ScriptableComponent {
public:
    Nexus() = default;
    Nexus(Termina::Actor* owner) : TerminaScript::ScriptableComponent(owner) {}

    void Start()  override;
    void Update(float deltaTime) override;
    void Inspect() override;
    void Serialize(nlohmann::json& out) const override;
    void Deserialize(const nlohmann::json& in) override;

    int  getHP()      const { return hp; }
    int  getHPMax()   const { return hp_max; }
    bool isDead()     const { return hp <= 0; }
    bool isGameOver() const { return isDead(); }

    void takeDamage(int dmg) { hp = std::max(0, hp - dmg); }

private:
    int hp     = 500;
    int hp_max = 500;
};
