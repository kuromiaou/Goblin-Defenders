#pragma once

#include <Termina/Scripting/API/ScriptingAPI.hpp>

#include <vector>
#include <random>

using namespace TerminaScript;

/// Per-particle runtime state, paired with a pooled actor.
struct Particle
{
    Termina::Actor* actor    = nullptr;
    glm::vec3       velocity = glm::vec3(0.0f);
    float           age      = 0.0f;
    float           lifetime = 0.0f;
    bool            alive    = false;
};

/// Object-pool particle system.
///
/// Attach to any actor. Assign a prefab, tune the parameters, and the system
/// will pre-spawn `m_PoolSize` actors on Start, then recycle them continuously
/// as particles are emitted and expire.
///
/// Emission fires along the actor's local +Y axis inside a configurable spread
/// cone. Gravity, speed variance, and lifetime variance give a natural look.
class ParticleSystemComponent : public TerminaScript::ScriptableComponent
{
public:
    ParticleSystemComponent() = default;
    ParticleSystemComponent(Termina::Actor* owner) : TerminaScript::ScriptableComponent(owner) {}

    void Awake()                 override;
    void Start()                 override;
    void Stop()                  override;
    void Update(float deltaTime) override;

    void Inspect()                              override;
    void Serialize(nlohmann::json& out) const  override;
    void Deserialize(const nlohmann::json& in)  override;

    // ── Inspector fields ────────────────────────────────────────────────────
    Prefab    m_ParticlePrefab;                                  ///< Prefab to pool (.trp)
    int       m_PoolSize     = 100;                              ///< Max simultaneous particles
    float     m_EmitRate     = 20.0f;                           ///< Particles emitted per second
    float     m_Lifetime     = 2.5f;                            ///< Base particle lifetime (s)
    float     m_LifetimeVar  = 0.5f;                            ///< ± lifetime variance (s)
    float     m_SpeedMin     = 3.0f;                            ///< Min launch speed (m/s)
    float     m_SpeedMax     = 7.0f;                            ///< Max launch speed (m/s)
    float     m_SpreadAngle  = 25.0f;                           ///< Half-angle of emission cone (°)
    glm::vec3 m_Gravity      = glm::vec3(0.0f, -5.0f, 0.0f);  ///< Acceleration (m/s²)
    glm::vec3 m_InitialScale = glm::vec3(0.12f);               ///< Scale at birth

private:
    void      EmitParticle();
    Particle* GetFreeParticle();

    std::vector<Particle> m_Pool;
    float                 m_EmitAccumulator = 0.0f;
    std::mt19937          m_Rng;
};
