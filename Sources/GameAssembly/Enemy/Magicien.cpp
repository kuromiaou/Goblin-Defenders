#include "Magicien.hpp"
#include <ImGui/imgui.h>
#include "GameAssembly/Gameplay/GoldPickup.hpp"
#include "GameAssembly/Structures/TowerAOE.hpp"
#include "GameAssembly/Structures/TowerCC.hpp"
#include "GameAssembly/Structures/TowerSingle.hpp"
#include "GameAssembly/Utils/ActorUtils.hpp"

void Magicien::Start()
{
    initForWave(1);
}

void Magicien::Update(float deltaTime)
{
    tickEffects(deltaTime);
    if (isDead()) {
        handleDeath();
        return;
    }
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
    res = applyResistanceEffects(res);
    float mult = 100.0f / (100.0f + static_cast<float>(res));
    hp = std::max(0, hp - static_cast<int>(dmg * mult));
}

void Magicien::stunAllTowers(float duration) const
{
    if (!m_Owner) return;
    Termina::World* world = m_Owner->GetParentWorld();
    if (!world) return;

    for (const auto& actor : world->GetActors())
    {
        if (!actor || !actor->IsActive()) continue;
        if (actor->HasComponent<TowerSingle>()) actor->GetComponent<TowerSingle>().applyStun(duration);
        if (actor->HasComponent<TowerAOE>())    actor->GetComponent<TowerAOE>().applyStun(duration);
        if (actor->HasComponent<TowerCC>())     actor->GetComponent<TowerCC>().applyStun(duration);
    }
}

void Magicien::handleDeath()
{
    if (death_handled || !m_Owner) return;
    death_handled = true;

    stunAllTowers(getDeathStunDuration());

    static const TerminaScript::Prefab goldPrefab("Assets/Prefabs/gold pile.trp");
    if (goldPrefab.IsValid() && m_Transform) {
        if (Termina::Actor* drop = Instantiate(goldPrefab)) {
            drop->GetComponent<Termina::Transform>().SetPosition(m_Transform->GetPosition());
            //drop->AddComponent<GoldPickup>().SetGoldAmount(getGoldValue());
        }
    }

    DestroyActorHierarchy(m_Owner);
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
