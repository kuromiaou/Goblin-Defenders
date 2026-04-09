#include "TrapWolf.hpp"
#include <ImGui/imgui.h>
#include <glm/geometric.hpp>
#include "GameAssembly/Enemy/EnemyQuery.hpp"
#include "GameAssembly/Enemy/EnemyMovement.hpp"

void TrapWolf::Start()
{
    triggered = false;
}

void TrapWolf::Update(float deltaTime)
{
    if (triggered) return;

    auto* world = m_Owner->GetParentWorld();
    if (!world) return;

    glm::vec3 trapPos = m_Transform->GetPosition();

    for (const auto& actor : world->GetActors())
    {
        if (!actor || !actor->IsActive()) continue;
        Enemy* e = GetEnemyComponent(actor.get());
        if (!e || e->isDead()) continue;

        glm::vec3 ePos = actor->GetComponent<Termina::Transform>().GetPosition();
        if (glm::distance(trapPos, ePos) > trigger_radius) continue;

        float stunDur = trigger(); // sets triggered = true
        if (stunDur > 0.0f && actor->HasComponent<EnemyMovement>()) {
            actor->GetComponent<EnemyMovement>().ApplyStun(stunDur);
            TN_INFO("[TrapWolf] Triggered! Stunned enemy for %.2fs", stunDur);
        }
        Destroy(m_Owner); // single-use trap
        return;
    }
}

float TrapWolf::getStunDuration() const
{
    float t = static_cast<float>(rand()) / RAND_MAX;
    return stun_min + t * (stun_max - stun_min);
}

float TrapWolf::trigger()
{
    if (triggered) return 0.0f;
    triggered = true;
    return getStunDuration();
}

void TrapWolf::Inspect()
{
    ImGui::Checkbox ("Triggered",      &triggered);
    ImGui::DragFloat("Stun Min",       &stun_min,       0.1f, 0.0f, stun_max);
    ImGui::DragFloat("Stun Max",       &stun_max,       0.1f, stun_min, 5.0f);
    ImGui::DragFloat("Trigger Radius", &trigger_radius, 0.1f, 0.1f, 5.0f);
}

void TrapWolf::Serialize(nlohmann::json& out) const
{
    out["triggered"]      = triggered;
    out["stun_min"]       = stun_min;
    out["stun_max"]       = stun_max;
    out["trigger_radius"] = trigger_radius;
}

void TrapWolf::Deserialize(const nlohmann::json& in)
{
    if (in.contains("triggered"))      triggered      = in["triggered"];
    if (in.contains("stun_min"))       stun_min       = in["stun_min"];
    if (in.contains("stun_max"))       stun_max       = in["stun_max"];
    if (in.contains("trigger_radius")) trigger_radius = in["trigger_radius"];
}
