#include "ParticleSystem.hpp"

#include <Termina/Renderer/UIUtils.hpp>
#include <ImGui/imgui.h>
#include <GLM/gtx/quaternion.hpp>
#include <cmath>

// ── Lifecycle ────────────────────────────────────────────────────────────────

void ParticleSystemComponent::Awake()
{
    // Seed the RNG — pool is created in Start() so it is never
    // captured in the play-mode snapshot that WorldSystem takes
    // before calling OnPlay()/Start().
    m_Rng.seed(std::random_device{}());
}

void ParticleSystemComponent::Start()
{
    if (!m_ParticlePrefab.IsValid())
        return;

    m_Pool.resize(m_PoolSize);
    for (auto& p : m_Pool)
    {
        p.actor = Instantiate(m_ParticlePrefab);
        if (p.actor)
            p.actor->GetComponent<Termina::Transform>().SetScale(glm::vec3(0.0f));
        p.alive = false;
    }
}

void ParticleSystemComponent::Stop()
{
    for (auto& p : m_Pool)
    {
        if (p.actor)
        {
            Destroy(p.actor);
            p.actor = nullptr;
        }
    }
    m_Pool.clear();
    m_EmitAccumulator = 0.0f;
}

void ParticleSystemComponent::Update(float deltaTime)
{
    // ── Emission ─────────────────────────────────────────────────────────────
    if (m_ParticlePrefab.IsValid() && m_EmitRate > 0.0f)
    {
        m_EmitAccumulator += deltaTime;
        const float interval = 1.0f / m_EmitRate;
        while (m_EmitAccumulator >= interval)
        {
            EmitParticle();
            m_EmitAccumulator -= interval;
        }
    }

    // ── Simulate ─────────────────────────────────────────────────────────────
    for (auto& p : m_Pool)
    {
        if (!p.alive || !p.actor)
            continue;

        p.age += deltaTime;

        if (p.age >= p.lifetime)
        {
            // Return to pool — hide by zeroing scale
            p.alive = false;
            p.actor->GetComponent<Termina::Transform>().SetScale(glm::vec3(0.0f));
            continue;
        }

        const float t = p.age / p.lifetime;  // normalised age [0,1]

        // Integrate velocity
        p.velocity += m_Gravity * deltaTime;

        auto& xform = p.actor->GetComponent<Termina::Transform>();
        xform.SetPosition(xform.GetPosition() + p.velocity * deltaTime);

        // Quadratic scale fade: full size at birth, zero at death
        const float scaleFactor = 1.0f - (t * t);
        xform.SetScale(m_InitialScale * scaleFactor);
    }
}

// ── Emission helpers ──────────────────────────────────────────────────────────

void ParticleSystemComponent::EmitParticle()
{
    Particle* p = GetFreeParticle();
    if (!p || !p->actor)
        return;

    std::uniform_real_distribution<float> thetaDist(0.0f, glm::radians(m_SpreadAngle));
    std::uniform_real_distribution<float> phiDist(0.0f, glm::two_pi<float>());
    std::uniform_real_distribution<float> speedDist(m_SpeedMin, m_SpeedMax);
    std::uniform_real_distribution<float> lifeDist(
        m_Lifetime - m_LifetimeVar,
        m_Lifetime + m_LifetimeVar
    );

    const float theta = thetaDist(m_Rng);
    const float phi   = phiDist(m_Rng);
    const float speed = speedDist(m_Rng);

    // Build a random direction inside a cone around +Y
    glm::vec3 dir(
        sinf(theta) * cosf(phi),
        cosf(theta),
        sinf(theta) * sinf(phi)
    );

    // Rotate the cone to align with the emitter's local up axis
    const glm::vec3 worldUp   = glm::vec3(0.0f, 1.0f, 0.0f);
    const glm::vec3 emitterUp = m_Transform->GetUp();
    const float     dot       = glm::dot(worldUp, emitterUp);

    if (dot < -0.999f)
    {
        // Emitter pointing straight down — flip
        dir = -dir;
    }
    else if (dot < 0.999f)
    {
        const glm::quat q = glm::rotation(worldUp, emitterUp);
        dir = glm::normalize(q * dir);
    }

    p->velocity = dir * speed;
    p->age      = 0.0f;
    p->lifetime = glm::max(lifeDist(m_Rng), 0.01f);
    p->alive    = true;

    auto& xform = p->actor->GetComponent<Termina::Transform>();
    xform.SetPosition(m_Transform->GetPosition());
    xform.SetScale(m_InitialScale);
}

Particle* ParticleSystemComponent::GetFreeParticle()
{
    for (auto& p : m_Pool)
    {
        if (!p.alive)
            return &p;
    }
    return nullptr;
}

// ── Inspector ─────────────────────────────────────────────────────────────────

void ParticleSystemComponent::Inspect()
{
    m_ParticlePrefab.Inspect("Prefab");

    ImGui::Separator();

    ImGui::DragInt  ("Pool Size",     &m_PoolSize,    1,    1, 10000);
    ImGui::DragFloat("Emit Rate",     &m_EmitRate,    0.5f, 0.0f, 1000.0f, "%.1f /s");

    ImGui::Separator();

    ImGui::DragFloat("Lifetime",      &m_Lifetime,    0.05f, 0.01f, 60.0f, "%.2f s");
    ImGui::DragFloat("Lifetime Var",  &m_LifetimeVar, 0.05f, 0.00f, 30.0f, "%.2f s");

    ImGui::Separator();

    ImGui::DragFloat("Speed Min",     &m_SpeedMin,    0.1f, 0.0f, 200.0f, "%.1f m/s");
    ImGui::DragFloat("Speed Max",     &m_SpeedMax,    0.1f, 0.0f, 200.0f, "%.1f m/s");
    ImGui::DragFloat("Spread Angle",  &m_SpreadAngle, 0.5f, 0.0f, 180.0f, "%.1f deg");

    ImGui::Separator();

    ImGui::DragFloat3("Gravity",       &m_Gravity.x,      0.1f, -100.0f, 100.0f, "%.2f");
    ImGui::DragFloat3("Initial Scale", &m_InitialScale.x, 0.005f, 0.0f,   10.0f, "%.3f");

    // Live stats
    ImGui::Separator();
    int alive = 0;
    for (const auto& p : m_Pool)
        alive += p.alive ? 1 : 0;
    ImGui::LabelText("Active / Pool", "%d / %d", alive, (int)m_Pool.size());
}

// ── Serialization ─────────────────────────────────────────────────────────────

void ParticleSystemComponent::Serialize(nlohmann::json& out) const
{
    out["prefab"]       = m_ParticlePrefab.Path;
    out["poolSize"]     = m_PoolSize;
    out["emitRate"]     = m_EmitRate;
    out["lifetime"]     = m_Lifetime;
    out["lifetimeVar"]  = m_LifetimeVar;
    out["speedMin"]     = m_SpeedMin;
    out["speedMax"]     = m_SpeedMax;
    out["spreadAngle"]  = m_SpreadAngle;
    out["gravity"]      = { m_Gravity.x,      m_Gravity.y,      m_Gravity.z      };
    out["initialScale"] = { m_InitialScale.x, m_InitialScale.y, m_InitialScale.z };
}

void ParticleSystemComponent::Deserialize(const nlohmann::json& in)
{
    if (in.contains("prefab"))       m_ParticlePrefab.Path = in["prefab"];
    if (in.contains("poolSize"))     m_PoolSize            = in["poolSize"];
    if (in.contains("emitRate"))     m_EmitRate            = in["emitRate"];
    if (in.contains("lifetime"))     m_Lifetime            = in["lifetime"];
    if (in.contains("lifetimeVar"))  m_LifetimeVar         = in["lifetimeVar"];
    if (in.contains("speedMin"))     m_SpeedMin            = in["speedMin"];
    if (in.contains("speedMax"))     m_SpeedMax            = in["speedMax"];
    if (in.contains("spreadAngle"))  m_SpreadAngle         = in["spreadAngle"];

    if (in.contains("gravity"))
    {
        const auto& g = in["gravity"];
        m_Gravity = { g[0], g[1], g[2] };
    }
    if (in.contains("initialScale"))
    {
        const auto& s = in["initialScale"];
        m_InitialScale = { s[0], s[1], s[2] };
    }
}
