#pragma once
#include <Termina/Scripting/API/ScriptingAPI.hpp>
#include "GameAssembly/Enemy/Checkpoint.hpp"
#include <glm/glm.hpp>

class EnemyMovement : public TerminaScript::ScriptableComponent
{
public:
    EnemyMovement() = default;
    EnemyMovement(Termina::Actor* owner) : TerminaScript::ScriptableComponent(owner) {}

    void Start() override;
    void Update(float deltaTime) override;
    void Inspect() override;

    // AJOUT: Sérialisation
    void Serialize(nlohmann::json& out) const override;
    void Deserialize(const nlohmann::json& in) override;

    void SetSpeed(float speed) { m_Speed = speed; }
    float GetSpeed() const { return m_Speed; }

private:
    // Distance au checkpoint actuel
    float GetDistanceToTarget() const;

    float m_Speed = 10.0f;
    int m_CurrentCheckpointIndex = 0;
    Checkpoint* m_CurrentTarget = nullptr;

    bool m_PathfindingInitialized = false;
};