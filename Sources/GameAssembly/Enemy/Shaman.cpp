#include "Shaman.hpp"
#include <ImGui/imgui.h>
#include <glm/geometric.hpp>
#include "GameAssembly/Enemy/EnemyMovement.hpp"
#include "GameAssembly/Enemy/EnemyQuery.hpp"
#include "GameAssembly/Gameplay/GoldPickup.hpp"
#include "GameAssembly/Utils/ActorUtils.hpp"

void Shaman::Start()
{
    initForWave(1);
}

void Shaman::Update(float deltaTime)
{
    tickEffects(deltaTime);
    if (isDead()) {
        handleDeath();
        return;
    }

    aura_tick_accumulator += deltaTime;
    if (aura_tick_accumulator >= 0.1f) {
        aura_tick_accumulator = 0.0f;
        applyAura();
    }
}

void Shaman::initForWave(int current_wave)
{
    wave   = current_wave;
    float scaling = 1.0f + (wave - 1) * 0.1f;
    hp_max = static_cast<int>(static_cast<int>(HPTier::HP_MID) * scaling);
    hp     = hp_max;
    computeGoldValue();
}

void Shaman::takeDamage(int dmg, DamageType type)
{
    int res = (type == DamageType::PHYSIQUE)
        ? static_cast<int>(res_physique)
        : static_cast<int>(res_magique);
    res = applyResistanceEffects(res);
    float mult = 100.0f / (100.0f + static_cast<float>(res));
    hp = std::max(0, hp - static_cast<int>(dmg * mult));
}

void Shaman::applyAura()
{
    if (!m_Owner || !m_Transform) return;

    auto enemies = GetAllLiveEnemies(m_Owner->GetParentWorld());
    const glm::vec3 center = m_Transform->GetPosition();

    for (auto& [actor, enemy] : enemies)
    {
        if (!actor || actor == m_Owner || !actor->HasComponent<Termina::Transform>()) continue;

        const glm::vec3 pos = actor->GetComponent<Termina::Transform>().GetPosition();
        if (glm::distance(center, pos) > aura_size) continue;

        switch (aura_type)
        {
        case AuraType::ATK_BOOST:
            enemy->applyAttackBoost(0.2f, 1.25f);
            break;
        case AuraType::SPD_BOOST:
            if (actor->HasComponent<EnemyMovement>())
                actor->GetComponent<EnemyMovement>().ApplySpeedBoost(0.2f, 1.35f);
            break;
        case AuraType::RES_BOOST:
            enemy->applyResBoost(0.2f, static_cast<int>(ResTier::RES_MID));
            break;
        }
    }
}

void Shaman::handleDeath()
{
    if (death_handled || !m_Owner) return;
    death_handled = true;

    static const TerminaScript::Prefab goldPrefab("Assets/Prefabs/gold pile.trp");
    if (goldPrefab.IsValid() && m_Transform) {
        if (Termina::Actor* drop = Instantiate(goldPrefab)) {
            drop->GetComponent<Termina::Transform>().SetPosition(m_Transform->GetPosition());
            drop->AddComponent<GoldPickup>().SetGoldAmount(getGoldValue());
        }
    }

    DestroyActorHierarchy(m_Owner);
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
