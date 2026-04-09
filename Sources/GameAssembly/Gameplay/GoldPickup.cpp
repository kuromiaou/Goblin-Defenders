#include "GoldPickup.hpp"

#include <ImGui/imgui.h>
#include <glm/geometric.hpp>

#include "GameAssembly/Enemy/GoblinRapace.hpp"
#include "GameAssembly/Player/Player.hpp"
#include "GameAssembly/Utils/ActorUtils.hpp"

void GoldPickup::Update(float deltaTime)
{
    (void)deltaTime;
    if (m_Consumed || !m_Owner || !m_Transform)
        return;

    Termina::World* world = m_Owner->GetParentWorld();
    if (!world) return;

    const glm::vec3 goldPos = m_Transform->GetPosition();

    for (const auto& actor : world->GetActors())
    {
        if (!actor || !actor->IsActive() || actor.get() == m_Owner) continue;
        if (!actor->HasComponent<Termina::Transform>()) continue;

        const glm::vec3 pos = actor->GetComponent<Termina::Transform>().GetPosition();
        if (glm::distance(pos, goldPos) > m_PickupRadius) continue;

        if (actor->HasComponent<GoblinRapace>())
        {
            auto& thief = actor->GetComponent<GoblinRapace>();
            if (!thief.isDead()) {
                thief.stealGold(m_GoldAmount);
                m_Consumed = true;
                DestroyActorHierarchy(m_Owner);
                return;
            }
        }

        if (!actor->HasComponent<Player>()) continue;

        actor->GetComponent<Player>().addGold(m_GoldAmount);
        m_Consumed = true;
        DestroyActorHierarchy(m_Owner);
        return;
    }
}

void GoldPickup::Inspect()
{
    ImGui::DragInt("Gold Amount", &m_GoldAmount, 1, 0, 10000);
    ImGui::DragFloat("Pickup Radius", &m_PickupRadius, 0.05f, 0.1f, 5.0f);
}

void GoldPickup::Serialize(nlohmann::json& out) const
{
    out["gold_amount"] = m_GoldAmount;
    out["pickup_radius"] = m_PickupRadius;
}

void GoldPickup::Deserialize(const nlohmann::json& in)
{
    if (in.contains("gold_amount"))   m_GoldAmount = in["gold_amount"];
    if (in.contains("pickup_radius")) m_PickupRadius = in["pickup_radius"];
}
