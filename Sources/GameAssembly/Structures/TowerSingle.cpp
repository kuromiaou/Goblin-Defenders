#include "TowerSingle.hpp"
#include <ImGui/imgui.h>
#include <glm/geometric.hpp>
#include "GameAssembly/Enemy/EnemyQuery.hpp"
#include "GameAssembly/Enemy/EnemyMovement.hpp"

void TowerSingle::Start()
{
}

void TowerSingle::Update(float deltaTime)
{
    if (attack_indicator_timer > 0.0f)
        attack_indicator_timer = std::max(0.0f, attack_indicator_timer - deltaTime);

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

    // Select target based on aggro mode.
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

    tEnemy->takeDamage(getATK(), damage_type);
    attack_indicator_timer = 0.15f;
    TN_INFO("[TowerSingle] Hit enemy for %d (%s) dmg",
        getATK(), damage_type == DamageType::PHYSIQUE ? "Phys" : "Mag");

    float atkPerSec    = getATKSPD();
    attack_cooldown    = (atkPerSec > 0.0f) ? (1.0f / atkPerSec) : 1.0f;
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

    ImGui::TextColored(
        isAttackIndicatorActive() ? ImVec4(0.1f, 1.0f, 0.1f, 1.0f) : ImVec4(1.0f, 1.0f, 1.0f, 1.0f),
        "Attack Indicator: %s",
        isAttackIndicatorActive() ? "ON" : "OFF"
    );
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
