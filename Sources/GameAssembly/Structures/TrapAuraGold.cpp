#include "TrapAuraGold.hpp"
#include <ImGui/imgui.h>

void TrapAuraGold::Start()
{
}

void TrapAuraGold::Update(float deltaTime)
{
}

int TrapAuraGold::computeGoldBonus(int enemy_hp_max) const
{
    float t = static_cast<float>(rand()) / RAND_MAX;
    float mult = gold_mult_min + t * (gold_mult_max - gold_mult_min);
    return static_cast<int>(enemy_hp_max * mult);
}

void TrapAuraGold::Inspect()
{
    ImGui::DragFloat("Aura Radius",   &aura_radius,   0.1f, 0.5f, 10.0f);
    ImGui::DragFloat("Gold Mult Min", &gold_mult_min, 0.05f, 0.0f, gold_mult_max);
    ImGui::DragFloat("Gold Mult Max", &gold_mult_max, 0.05f, gold_mult_min, 3.0f);
}

void TrapAuraGold::Serialize(nlohmann::json& out) const
{
    out["aura_radius"]   = aura_radius;
    out["gold_mult_min"] = gold_mult_min;
    out["gold_mult_max"] = gold_mult_max;
}

void TrapAuraGold::Deserialize(const nlohmann::json& in)
{
    if (in.contains("aura_radius"))   aura_radius   = in["aura_radius"];
    if (in.contains("gold_mult_min")) gold_mult_min = in["gold_mult_min"];
    if (in.contains("gold_mult_max")) gold_mult_max = in["gold_mult_max"];
}
