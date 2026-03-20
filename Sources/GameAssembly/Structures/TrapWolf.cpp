#include "TrapWolf.hpp"
#include <ImGui/imgui.h>

void TrapWolf::Start()
{
    triggered = false;
}

void TrapWolf::Update(float deltaTime)
{
}

float TrapWolf::getStunDuration() const
{
    float t = static_cast<float>(rand()) / RAND_MAX;
    return stun_min + t * (stun_max - stun_min);
}

float TrapWolf::trigger()
{
    if (triggered) return 0.0f;
    triggered = true;
    return getStunDuration();
}

void TrapWolf::Inspect()
{
    ImGui::Checkbox ("Triggered", &triggered);
    ImGui::DragFloat("Stun Min",  &stun_min,  0.1f, 0.0f, stun_max);
    ImGui::DragFloat("Stun Max",  &stun_max,  0.1f, stun_min, 5.0f);
}

void TrapWolf::Serialize(nlohmann::json& out) const
{
    out["triggered"] = triggered;
    out["stun_min"]  = stun_min;
    out["stun_max"]  = stun_max;
}

void TrapWolf::Deserialize(const nlohmann::json& in)
{
    if (in.contains("triggered")) triggered = in["triggered"];
    if (in.contains("stun_min"))  stun_min  = in["stun_min"];
    if (in.contains("stun_max"))  stun_max  = in["stun_max"];
}
