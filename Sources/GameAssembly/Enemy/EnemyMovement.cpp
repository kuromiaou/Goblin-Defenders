#include "EnemyMovement.hpp"
#include <algorithm>
#include <glm/geometric.hpp>
#include "ImGui/imgui.h"
#include "GameAssembly/Enemy/Goblin.hpp"
#include "GameAssembly/Enemy/GoblinRapace.hpp"
#include "GameAssembly/Enemy/Hobgoblin.hpp"
#include "GameAssembly/Enemy/Magicien.hpp"
#include "GameAssembly/Enemy/Shaman.hpp"
#include "GameAssembly/Structures/Nexus.hpp"
#include "GameAssembly/Utils/ActorUtils.hpp"

namespace
{
constexpr const char* kNexusActorName = "Nexus";

int GetNexusContactDamage(Termina::Actor* enemyActor)
{
    if (!enemyActor) return 1;

    if (enemyActor->HasComponent<Goblin>()) {
        return std::max(1, enemyActor->GetComponent<Goblin>().getATK());
    }
    if (enemyActor->HasComponent<Hobgoblin>()) {
        return std::max(1, enemyActor->GetComponent<Hobgoblin>().getATK());
    }
    if (enemyActor->HasComponent<Magicien>()) {
        return std::max(1, enemyActor->GetComponent<Magicien>().getATK());
    }

    // Shaman and GoblinRapace don't expose ATK getters: apply minimum guaranteed chip damage.
    return 1;
}

void ApplyNexusContactEffects(Termina::Actor* enemyActor)
{
    if (!enemyActor) return;

    Termina::World* world = enemyActor->GetParentWorld();
    if (!world) return;

    if (enemyActor->HasComponent<GoblinRapace>()) {
        enemyActor->GetComponent<GoblinRapace>().onReachNexus();
    }

    const int damage = GetNexusContactDamage(enemyActor);
    Termina::Actor* nexusActor = nullptr;

    for (const auto& actor : world->GetActors())
    {
        if (!actor || !actor->IsActive()) continue;
        if (actor->HasComponent<Nexus>() || actor->GetName() == kNexusActorName) {
            nexusActor = actor.get();
            break;
        }
    }

    if (!nexusActor) return;
    if (!nexusActor->HasComponent<Nexus>()) {
        // NOTE: Le composant "Nexus" doit être posé dans la scène/prefab (pas d'AddComponent runtime).
        TN_ERROR("EnemyMovement: missing Nexus component on '%s'.", nexusActor->GetName().c_str());
        return;
    }
    nexusActor->GetComponent<Nexus>().takeDamage(damage);
}
}

void EnemyMovement::Start()
{
    if (!m_Transform) {
        TN_ERROR("EnemyMovement: m_Transform is nullptr! Skipping pathfinding init.");
        return;
    }

    auto& checkpoints = Checkpoint::GetAllCheckpoints();
    std::sort(checkpoints.begin(), checkpoints.end(),
        [](Checkpoint* a, Checkpoint* b) {
            return a->getOrder() < b->getOrder();
        });

    if (!checkpoints.empty()) {
        m_CurrentTarget = checkpoints[0];
        m_CurrentCheckpointIndex = 0;
        m_PathfindingInitialized = true;
        TN_INFO("Enemy movement started. First checkpoint: %d", m_CurrentTarget->getOrder());
    }
    else {
        TN_WARN("No checkpoints found for enemy movement!");
    }
}

void EnemyMovement::Update(float deltaTime)
{
    if (!m_PathfindingInitialized || !m_CurrentTarget || !m_Transform)
        return;

    // Tick stun timer — enemy cannot move while stunned.
    if (m_StunTimer > 0.0f) {
        m_StunTimer = std::max(0.0f, m_StunTimer - deltaTime);
        return;
    }

    // Tick slow timer.
    if (m_SlowTimer > 0.0f)
        m_SlowTimer = std::max(0.0f, m_SlowTimer - deltaTime);
    if (m_SpeedBoostTimer > 0.0f)
        m_SpeedBoostTimer = std::max(0.0f, m_SpeedBoostTimer - deltaTime);

    glm::vec3 currentPos = m_Transform->GetLocalPosition();
    glm::vec3 targetPos  = m_CurrentTarget->getPosition();
    glm::vec3 direction  = glm::normalize(targetPos - currentPos);
    float     distance   = GetDistanceToTarget();

    if (distance < 0.1f) {
        auto& checkpoints = Checkpoint::GetAllCheckpoints();
        m_CurrentCheckpointIndex++;

        if (m_CurrentCheckpointIndex >= static_cast<int>(checkpoints.size())) {
            TN_INFO("Enemy reached the end of the path!");
            ApplyNexusContactEffects(m_Owner);
            DestroyActorHierarchy(m_Owner);
            return;
        }

        m_CurrentTarget = checkpoints[m_CurrentCheckpointIndex];
        TN_INFO("Moving to checkpoint %d", m_CurrentTarget->getOrder());
        return;
    }

    float effectiveSpeed = m_Speed * GetSlowFactor() * GetSpeedBoostFactor();
    glm::vec3 newPos = currentPos + direction * effectiveSpeed * deltaTime;
    m_Transform->SetPosition(newPos);
}

void EnemyMovement::ApplySlow(float duration, float factor)
{
    if (duration > m_SlowTimer) {
        m_SlowTimer  = duration;
        m_SlowFactor = factor;
    }
}

void EnemyMovement::ApplyStun(float duration)
{
    m_StunTimer = std::max(m_StunTimer, duration);
}

void EnemyMovement::ApplySpeedBoost(float duration, float factor)
{
    if (duration > m_SpeedBoostTimer) {
        m_SpeedBoostTimer = duration;
        m_SpeedBoostFactor = std::max(1.0f, factor);
    }
}

float EnemyMovement::GetDistanceToTarget() const
{
    if (!m_CurrentTarget || !m_Transform)
        return FLT_MAX;

    glm::vec3 currentPos = m_Transform->GetLocalPosition();
    glm::vec3 targetPos  = m_CurrentTarget->getPosition();
    return glm::distance(currentPos, targetPos);
}

void EnemyMovement::Inspect()
{
    ImGui::DragFloat("Speed##enemy", &m_Speed, 0.1f, 0.0f, 50.0f);
    if (m_CurrentTarget) {
        ImGui::Text("Current Target Checkpoint: %d", m_CurrentTarget->getOrder());
        ImGui::Text("Distance to Target: %.2f", GetDistanceToTarget());
    }
    if (m_StunTimer > 0.0f)
        ImGui::Text("STUNNED: %.2fs remaining", m_StunTimer);
    if (m_SlowTimer > 0.0f)
        ImGui::Text("SLOWED (x%.1f): %.2fs remaining", m_SlowFactor, m_SlowTimer);
    if (m_SpeedBoostTimer > 0.0f)
        ImGui::Text("BOOSTED (x%.1f): %.2fs remaining", m_SpeedBoostFactor, m_SpeedBoostTimer);
}

void EnemyMovement::Serialize(nlohmann::json& out) const
{
    out["speed"] = m_Speed;
}

void EnemyMovement::Deserialize(const nlohmann::json& in)
{
    if (in.contains("speed"))
        m_Speed = in["speed"];
}
