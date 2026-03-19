#include "TowerCC.hpp"
#include <ImGui/imgui.h>

void TowerCC::Start()
{
}

void TowerCC::Update(float deltaTime)
{
    if (stun_timer > 0.0f)
        stun_timer = std::max(0.0f, stun_timer - deltaTime);
}

void TowerCC::upgrade()
{
    level++;
}

void TowerCC::Inspect()
{
    ImGui::DragInt("Level", &level, 1, 1, 99);

    const char* cc_names[] = { "Slow", "Shred" };
    int cc_idx = static_cast<int>(cc_type);
    if (ImGui::Combo("CC Type", &cc_idx, cc_names, 2))
        cc_type = static_cast<CCType>(cc_idx);

    if (cc_type == CCType::SHRED) {
        const char* shred_names[] = { "Physique", "Magique" };
        int shred_idx = static_cast<int>(shred_type);
        if (ImGui::Combo("Shred Type", &shred_idx, shred_names, 2))
            shred_type = static_cast<DamageType>(shred_idx);
    }

    const char* aggro_names[] = { "First", "Last", "Max HP", "Current HP" };
    int aggro_idx = static_cast<int>(aggro);
    if (ImGui::Combo("Aggro", &aggro_idx, aggro_names, 4))
        aggro = static_cast<AggroMode>(aggro_idx);
}

void TowerCC::Serialize(nlohmann::json& out) const
{
    out["level"]      = level;
    out["cc_type"]    = static_cast<int>(cc_type);
    out["shred_type"] = static_cast<int>(shred_type);
    out["aggro"]      = static_cast<int>(aggro);
}

void TowerCC::Deserialize(const nlohmann::json& in)
{
    if (in.contains("level"))      level      = in["level"];
    if (in.contains("cc_type"))    cc_type    = static_cast<CCType>(static_cast<int>(in["cc_type"]));
    if (in.contains("shred_type")) shred_type = static_cast<DamageType>(static_cast<int>(in["shred_type"]));
    if (in.contains("aggro"))      aggro      = static_cast<AggroMode>(static_cast<int>(in["aggro"]));
}
