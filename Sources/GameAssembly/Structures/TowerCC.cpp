#include "TowerCC.hpp"
#include <ImGui/imgui.h>
#include <glm/geometric.hpp>
#include "GameAssembly/Enemy/EnemyQuery.hpp"
#include "GameAssembly/Enemy/EnemyMovement.hpp"

void TowerCC::Start()
{
}

void TowerCC::Update(float deltaTime)
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

    // Find the primary target.
    Termina::Actor* target = nullptr;
    Enemy*          tEnemy = nullptr;
    float           best   = -1e9f;

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

        if (!target || val > best) {
            target = actor;
            tEnemy = enemy;
            best   = val;
        }
    }

    if (!target) return;

    // Apply CC effect.
    switch (cc_type)
    {
    case CCType::SLOW:
        if (target->HasComponent<EnemyMovement>()) {
            target->GetComponent<EnemyMovement>().ApplySlow(cc_duration, 0.5f);
            TN_INFO("[TowerCC] Applied SLOW (x0.5 spd, %.1fs) to enemy", cc_duration);
        }
        break;
    case CCType::SHRED:
        tEnemy->applyShred(cc_duration);
        TN_INFO("[TowerCC] Applied SHRED (-1 tier RES, %.1fs) to enemy", cc_duration);
        break;
    }

    float atkPerSec = getATKSPD();
    attack_cooldown = (atkPerSec > 0.0f) ? (1.0f / atkPerSec) : 1.0f;
}

void TowerCC::upgrade()
{
    level++;
}

void TowerCC::Inspect()
{
    ImGui::DragInt  ("Level",       &level,       1, 1, 99);
    ImGui::DragFloat("CC Duration", &cc_duration, 0.1f, 0.5f, 10.0f);

    const char* cc_names[] = { "Slow", "Shred" };
    int cc_idx = static_cast<int>(cc_type);
    if (ImGui::Combo("CC Type", &cc_idx, cc_names, 2))
        cc_type = static_cast<CCType>(cc_idx);

    const char* aggro_names[] = { "First", "Last", "Max HP", "Current HP" };
    int aggro_idx = static_cast<int>(aggro);
    if (ImGui::Combo("Aggro", &aggro_idx, aggro_names, 4))
        aggro = static_cast<AggroMode>(aggro_idx);
}

void TowerCC::Serialize(nlohmann::json& out) const
{
    out["level"]       = level;
    out["cc_type"]     = static_cast<int>(cc_type);
    out["aggro"]       = static_cast<int>(aggro);
    out["cc_duration"] = cc_duration;
}

void TowerCC::Deserialize(const nlohmann::json& in)
{
    if (in.contains("level"))       level       = in["level"];
    if (in.contains("cc_type"))     cc_type     = static_cast<CCType>(static_cast<int>(in["cc_type"]));
    if (in.contains("aggro"))       aggro       = static_cast<AggroMode>(static_cast<int>(in["aggro"]));
    if (in.contains("cc_duration")) cc_duration = in["cc_duration"];
}
