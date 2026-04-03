#include "Nexus.hpp"
#include <ImGui/imgui.h>

void Nexus::Start()
{
    hp = hp_max;
}

void Nexus::Update(float deltaTime)
{
    if (isGameOver())
    {
        TN_INFO("GAME OVER — Le Nexus est détruit !");
    }
}

void Nexus::Inspect()
{
    ImGui::DragInt("HP",     &hp,     1, 0, hp_max);
    ImGui::DragInt("HP Max", &hp_max, 1, 0, 9999);
}

void Nexus::Serialize(nlohmann::json& out) const
{
    out["hp"]     = hp;
    out["hp_max"] = hp_max;
}

void Nexus::Deserialize(const nlohmann::json& in)
{
    if (in.contains("hp"))     hp     = in["hp"];
    if (in.contains("hp_max")) hp_max = in["hp_max"];
}
