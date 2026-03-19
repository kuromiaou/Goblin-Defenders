#include "TowerSingle.hpp"
#include <ImGui/imgui.h>

void TowerSingle::Start()
{
}

void TowerSingle::Update(float deltaTime)
{
    if (stun_timer > 0.0f)
        stun_timer = std::max(0.0f, stun_timer - deltaTime);
}

void TowerSingle::upgrade()
{
    level++;
    // À étendre selon le game design final
}

void TowerSingle::Inspect()
{
    ImGui::DragInt("Level", &level, 1, 1, 99);

    const char* dmg_names[] = { "Physique", "Magique" };
    int dmg_idx = static_cast<int>(damage_type);
    if (ImGui::Combo("Damage Type", &dmg_idx, dmg_names, 2))
        damage_type = static_cast<DamageType>(dmg_idx);

    const char* aggro_names[] = { "First", "Last", "Max HP", "Current HP" };
    int aggro_idx = static_cast<int>(aggro);
    if (ImGui::Combo("Aggro", &aggro_idx, aggro_names, 4))
        aggro = static_cast<AggroMode>(aggro_idx);

    ImGui::DragFloat("Stun Timer", &stun_timer, 0.1f, 0.0f, 10.0f);
}

void TowerSingle::Serialize(nlohmann::json& out) const
{
    out["level"]       = level;
    out["damage_type"] = static_cast<int>(damage_type);
    out["aggro"]       = static_cast<int>(aggro);
}

void TowerSingle::Deserialize(const nlohmann::json& in)
{
    if (in.contains("level"))       level       = in["level"];
    if (in.contains("damage_type")) damage_type = static_cast<DamageType>(static_cast<int>(in["damage_type"]));
    if (in.contains("aggro"))       aggro       = static_cast<AggroMode>(static_cast<int>(in["aggro"]));
}
