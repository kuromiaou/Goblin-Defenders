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

    void Serialize(nlohmann::json& out) const override;
    void Deserialize(const nlohmann::json& in) override;

    void SetSpeed(float speed) { m_Speed = speed; }
    float GetSpeed() const { return m_Speed; }

    // Returns the index of the checkpoint currently being tracked (proxy for path progress).
    int GetCheckpointIndex() const { return m_CurrentCheckpointIndex; }

    // Apply a speed slow for `duration` seconds. `factor` in (0,1] — 0.5 = half speed.
    void ApplySlow(float duration, float factor = 0.5f);
    // Stun the enemy (stop movement) for `duration` seconds.
    void ApplyStun(float duration);
    // Apply a speed boost for `duration` seconds. `factor` >= 1.0.
    void ApplySpeedBoost(float duration, float factor = 1.25f);

    bool  IsStunned()     const { return m_StunTimer > 0.0f; }
    bool  IsSlowed()      const { return m_SlowTimer > 0.0f; }
    float GetSlowFactor() const { return m_SlowTimer > 0.0f ? m_SlowFactor : 1.0f; }
    bool  IsSpeedBoosted() const { return m_SpeedBoostTimer > 0.0f; }
    float GetSpeedBoostFactor() const { return m_SpeedBoostTimer > 0.0f ? m_SpeedBoostFactor : 1.0f; }

private:
    float GetDistanceToTarget() const;

    float m_Speed = 10.0f;
    int   m_CurrentCheckpointIndex = 0;
    Checkpoint* m_CurrentTarget = nullptr;

    bool  m_PathfindingInitialized = false;
    bool  m_reachedNexus = false;

    float m_SlowTimer  = 0.0f;
    float m_SlowFactor = 0.5f;   // effective factor while slow is active
    float m_StunTimer  = 0.0f;
    float m_SpeedBoostTimer = 0.0f;
    float m_SpeedBoostFactor = 1.25f;
};
