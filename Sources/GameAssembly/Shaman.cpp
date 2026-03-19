#include "Shaman.hpp"
#include <ImGui/imgui.h>

void Shaman::Start()
{
    initForWave(1);
}

void Shaman::Update(float deltaTime)
{
    // L'application de l'aura aux alliés sera gérée par le WaveManager
    // qui détecte les ennemis dans le rayon aura_size
}

void Shaman::initForWave(int current_wave)
{
    wave   = current_wave;
    float scaling = 1.0f + (wave - 1) * 0.1f;
    hp_max = static_cast<int>(static_cast<int>(HPTier::HP_MID) * scaling);
    hp     = hp_max;
    // Aura choisie aléatoirement
    aura_type = static_cast<AuraType>(rand() % 3);
    computeGoldValue();
}

void Shaman::takeDamage(int dmg, DamageType type)
{
    int res = (type == DamageType::PHYSIQUE)
        ? static_cast<int>(res_physique)
        : static_cast<int>(res_magique);
    float mult = 100.0f / (100.0f + static_cast<float>(res));
    hp = std::max(0, hp - static_cast<int>(dmg * mult));
}

void Shaman::Inspect()
{
    ImGui::DragInt  ("HP",         &hp,        1, 0, hp_max);
    ImGui::DragInt  ("HP Max",     &hp_max,    1, 0, 9999);
    ImGui::DragInt  ("Wave",       &wave,      1, 1, 999);
    ImGui::DragFloat("Aura Size",  &aura_size, 0.1f, 0.5f, 10.0f);

    const char* aura_names[] = { "ATK Boost", "SPD Boost", "RES Boost" };
    int aura_idx = static_cast<int>(aura_type);
    if (ImGui::Combo("Aura Type", &aura_idx, aura_names, 3))
        aura_type = static_cast<AuraType>(aura_idx);
}

void Shaman::Serialize(nlohmann::json& out) const
{
    out["hp"]         = hp;
    out["hp_max"]     = hp_max;
    out["wave"]       = wave;
    out["aura_size"]  = aura_size;
    out["aura_type"]  = static_cast<int>(aura_type);
}

void Shaman::Deserialize(const nlohmann::json& in)
{
    if (in.contains("hp"))        hp        = in["hp"];
    if (in.contains("hp_max"))    hp_max    = in["hp_max"];
    if (in.contains("wave"))      wave      = in["wave"];
    if (in.contains("aura_size")) aura_size = in["aura_size"];
    if (in.contains("aura_type")) aura_type = static_cast<AuraType>(static_cast<int>(in["aura_type"]));
}
