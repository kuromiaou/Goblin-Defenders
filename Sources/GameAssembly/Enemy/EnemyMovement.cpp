#include "EnemyMovement.hpp"
#include <algorithm>
#include <glm/geometric.hpp>
#include "ImGui/imgui.h"

void EnemyMovement::Start()
{
    // Trier les checkpoints par ordre
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
    if (!m_PathfindingInitialized || !m_CurrentTarget)
        return;

    Termina::Transform transform;
    glm::vec3 currentPos = transform.GetPosition();
    glm::vec3 targetPos = m_CurrentTarget->getPosition();

    // Direction vers le checkpoint
    glm::vec3 direction = glm::normalize(targetPos - currentPos);

    // Distance au checkpoint
    float distance = GetDistanceToTarget();

    // Si on est arrivé au checkpoint
    if (distance < 0.5f) {
        // Aller au prochain checkpoint
        auto& checkpoints = Checkpoint::GetAllCheckpoints();
        m_CurrentCheckpointIndex++;

        if (m_CurrentCheckpointIndex >= static_cast<int>(checkpoints.size())) {
            // On a atteint le dernier checkpoint (ex: la base ennemie)
            TN_INFO("Enemy reached the end of the path!");
            // Optionnel: détruire l'ennemi ou le faire disparaître
            return;
        }

        m_CurrentTarget = checkpoints[m_CurrentCheckpointIndex];
        TN_INFO("Moving to checkpoint %d", m_CurrentTarget->getOrder());
    }

    // Déplacer l'ennemi vers le checkpoint
    glm::vec3 newPos = currentPos + direction * m_Speed * deltaTime;
    transform.SetPosition(newPos);
}

float EnemyMovement::GetDistanceToTarget() const
{
    if (!m_CurrentTarget)
        return FLT_MAX;

    glm::vec3 currentPos = m_Transform->GetPosition();
    glm::vec3 targetPos = m_CurrentTarget->getPosition();
    return glm::distance(currentPos, targetPos);
}

void EnemyMovement::Inspect()
{
    ImGui::DragFloat("Speed##enemy", &m_Speed, 0.1f, 0.0f, 50.0f);
    if (m_CurrentTarget) {
        ImGui::Text("Current Target Checkpoint: %d", m_CurrentTarget->getOrder());
        ImGui::Text("Distance to Target: %.2f", GetDistanceToTarget());
    }
}