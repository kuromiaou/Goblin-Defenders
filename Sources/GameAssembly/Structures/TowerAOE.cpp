#include "TowerAOE.hpp"
#include <ImGui/imgui.h>
#include <glm/geometric.hpp>
#include "GameAssembly/Enemy/EnemyQuery.hpp"
#include "GameAssembly/Enemy/EnemyMovement.hpp"

void TowerAOE::Start()
{
}

void TowerAOE::Update(float deltaTime)
{
    // Tick stun — tower cannot fire while stunned.
    if (stun_timer > 0.0f) {
        stun_timer = std::max(0.0f, stun_timer - deltaTime);
        return;
    }

    // Tick attack cooldown.
    if (attack_cooldown > 0.0f) {
        attack_cooldown -= deltaTime;
        return;
    }

    auto enemies = GetAllLiveEnemies(m_Owner->GetParentWorld());
    if (enemies.empty()) return;

    glm::vec3 towerPos = m_Transform->GetPosition();
    float     range    = static_cast<float>(static_cast<int>(atk_range));

    // Find the primary target (center of AOE explosion).
    Termina::Actor* primary = nullptr;
    float           best    = -1e9f;

    for (auto& [actor, enemy] : enemies)
    {
        glm::vec3 ePos = actor->GetComponent<Termina::Transform>().GetPosition();
        if (glm::distance(towerPos, ePos) > range) continue;

        float val = 0.0f;
        switch (aggro)
        {
        case AggroMode::FIRST:
            val = actor->HasComponent<EnemyMovement>()
                ? static_cast<float>(actor->GetComponent<EnemyMovement>().GetCheckpointIndex())
                : 0.0f;
            break;
        case AggroMode::LAST:
            val = actor->HasComponent<EnemyMovement>()
                ? -static_cast<float>(actor->GetComponent<EnemyMovement>().GetCheckpointIndex())
                : 0.0f;
            break;
        case AggroMode::MAX_HP:
            val = static_cast<float>(enemy->getHPMax());
            break;
        case AggroMode::CURRENT_HP:
            val = static_cast<float>(enemy->getHP());
            break;
        }

        if (!primary || val > best) {
            primary = actor;
            best    = val;
        }
    }

    if (!primary) return;

    // Apply AOE damage to all enemies within aoe_radius of the primary target.
    glm::vec3 aoeCenter = primary->GetComponent<Termina::Transform>().GetPosition();
    int hitCount = 0;

    for (auto& [actor, enemy] : enemies)
    {
        glm::vec3 ePos = actor->GetComponent<Termina::Transform>().GetPosition();
        if (glm::distance(aoeCenter, ePos) <= aoe_radius) {
            enemy->takeDamage(getATK(), damage_type);
            ++hitCount;
        }
    }

    TN_INFO("[TowerAOE] Hit %d enemy(ies) for %d (%s) dmg",
        hitCount, getATK(), damage_type == DamageType::PHYSIQUE ? "Phys" : "Mag");

    float atkPerSec = getATKSPD();
    attack_cooldown = (atkPerSec > 0.0f) ? (1.0f / atkPerSec) : 1.0f;
}

void TowerAOE::upgrade()
{
    level++;
}

void TowerAOE::Inspect()
{
    ImGui::DragInt  ("Level",      &level,      1, 1, 99);
    ImGui::DragFloat("AOE Radius", &aoe_radius, 0.1f, 0.5f, 10.0f);

    const char* dmg_names[] = { "Physique", "Magique" };
    int dmg_idx = static_cast<int>(damage_type);
    if (ImGui::Combo("Damage Type", &dmg_idx, dmg_names, 2))
        damage_type = static_cast<DamageType>(dmg_idx);

    const char* aggro_names[] = { "First", "Last", "Max HP", "Current HP" };
    int aggro_idx = static_cast<int>(aggro);
    if (ImGui::Combo("Aggro", &aggro_idx, aggro_names, 4))
        aggro = static_cast<AggroMode>(aggro_idx);
}

void TowerAOE::Serialize(nlohmann::json& out) const
{
    out["level"]       = level;
    out["damage_type"] = static_cast<int>(damage_type);
    out["aggro"]       = static_cast<int>(aggro);
    out["aoe_radius"]  = aoe_radius;
}

void TowerAOE::Deserialize(const nlohmann::json& in)
{
    if (in.contains("level"))       level       = in["level"];
    if (in.contains("damage_type")) damage_type = static_cast<DamageType>(static_cast<int>(in["damage_type"]));
    if (in.contains("aggro"))       aggro       = static_cast<AggroMode>(static_cast<int>(in["aggro"]));
    if (in.contains("aoe_radius"))  aoe_radius  = in["aoe_radius"];
}
