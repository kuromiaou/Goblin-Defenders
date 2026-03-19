#include "TrapMine.hpp"
#include <ImGui/imgui.h>

void TrapMine::Start()
{
    triggered = false;
}

void TrapMine::Update(float deltaTime)
{
}

int TrapMine::trigger()
{
    if (triggered) return 0;
    triggered = true;
    return atk;
}

void TrapMine::Inspect()
{
    ImGui::DragInt ("ATK",       &atk,       1, 0, 9999);
    ImGui::Checkbox("Triggered", &triggered);

    const char* dmg_names[] = { "Physique", "Magique" };
    int dmg_idx = static_cast<int>(damage_type);
    if (ImGui::Combo("Damage Type", &dmg_idx, dmg_names, 2))
        damage_type = static_cast<DamageType>(dmg_idx);
}

void TrapMine::Serialize(nlohmann::json& out) const
{
    out["atk"]         = atk;
    out["damage_type"] = static_cast<int>(damage_type);
    out["triggered"]   = triggered;
}

void TrapMine::Deserialize(const nlohmann::json& in)
{
    if (in.contains("atk"))         atk         = in["atk"];
    if (in.contains("damage_type")) damage_type = static_cast<DamageType>(static_cast<int>(in["damage_type"]));
    if (in.contains("triggered"))   triggered   = in["triggered"];
}
