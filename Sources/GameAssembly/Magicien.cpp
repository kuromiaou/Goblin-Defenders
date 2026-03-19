#include "Magicien.hpp"
#include <ImGui/imgui.h>

void Magicien::Start()
{
    initForWave(1);
}

void Magicien::Update(float deltaTime)
{
}

void Magicien::initForWave(int current_wave)
{
    wave   = current_wave;
    float scaling = 1.0f + (wave - 1) * 0.1f;
    hp_max = static_cast<int>(static_cast<int>(HPTier::HP_MID) * scaling);
    hp     = hp_max;
    computeGoldValue();
}

void Magicien::takeDamage(int dmg, DamageType type)
{
    int res = (type == DamageType::PHYSIQUE)
        ? static_cast<int>(res_physique)
        : static_cast<int>(res_magique);
    float mult = 100.0f / (100.0f + static_cast<float>(res));
    hp = std::max(0, hp - static_cast<int>(dmg * mult));
}

float Magicien::getDeathStunDuration() const
{
    float t = static_cast<float>(rand()) / RAND_MAX;
    return stun_min + t * (stun_max - stun_min);
}

void Magicien::Inspect()
{
    ImGui::DragInt  ("HP",       &hp,       1, 0, hp_max);
    ImGui::DragInt  ("HP Max",   &hp_max,   1, 0, 9999);
    ImGui::DragInt  ("Wave",     &wave,     1, 1, 999);
    ImGui::DragFloat("Stun Min", &stun_min, 0.1f, 0.0f, stun_max);
    ImGui::DragFloat("Stun Max", &stun_max, 0.1f, stun_min, 5.0f);
}

void Magicien::Serialize(nlohmann::json& out) const
{
    out["hp"]       = hp;
    out["hp_max"]   = hp_max;
    out["wave"]     = wave;
    out["stun_min"] = stun_min;
    out["stun_max"] = stun_max;
}

void Magicien::Deserialize(const nlohmann::json& in)
{
    if (in.contains("hp"))       hp       = in["hp"];
    if (in.contains("hp_max"))   hp_max   = in["hp_max"];
    if (in.contains("wave"))     wave     = in["wave"];
    if (in.contains("stun_min")) stun_min = in["stun_min"];
    if (in.contains("stun_max")) stun_max = in["stun_max"];
}
