#pragma once

#include <Termina/Scripting/API/ScriptingAPI.hpp>
#include <algorithm>

class GoldPickup : public TerminaScript::ScriptableComponent
{
public:
    GoldPickup() = default;
    GoldPickup(Termina::Actor* owner) : TerminaScript::ScriptableComponent(owner) {}

    void Update(float deltaTime) override;
    void Inspect() override;
    void Serialize(nlohmann::json& out) const override;
    void Deserialize(const nlohmann::json& in) override;

    void SetGoldAmount(int amount) { m_GoldAmount = std::max(0, amount); }
    int  GetGoldAmount() const { return m_GoldAmount; }

private:
    int   m_GoldAmount    = 1;
    float m_PickupRadius  = 1.0f;
    bool  m_Consumed      = false;
};
