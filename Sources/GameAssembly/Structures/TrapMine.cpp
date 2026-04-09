#include "TrapMine.hpp"
#include <ImGui/imgui.h>
#include <glm/geometric.hpp>
#include "GameAssembly/Enemy/EnemyQuery.hpp"

void TrapMine::Start()
{
    triggered = false;
}

void TrapMine::Update(float deltaTime)
{
    if (triggered) return; // already detonated, waiting for actor cleanup

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

        int dmg = trigger(); // sets triggered = true, returns atk
        if (dmg > 0) {
            e->takeDamage(dmg, damage_type);
            TN_INFO("[TrapMine] Triggered! Dealt %d (%s) dmg to enemy",
                dmg, damage_type == DamageType::PHYSIQUE ? "Phys" : "Mag");
            Destroy(m_Owner); // single-use trap
        }
        return;
    }
}

int TrapMine::trigger()
{
    if (triggered) return 0;
    triggered = true;
    return atk;
}

void TrapMine::Inspect()
{
    ImGui::DragInt   ("ATK",            &atk,            1, 0, 9999);
    ImGui::DragFloat ("Trigger Radius", &trigger_radius, 0.1f, 0.1f, 5.0f);
    ImGui::Checkbox  ("Triggered",      &triggered);

    const char* dmg_names[] = { "Physique", "Magique" };
    int dmg_idx = static_cast<int>(damage_type);
    if (ImGui::Combo("Damage Type", &dmg_idx, dmg_names, 2))
        damage_type = static_cast<DamageType>(dmg_idx);
}

void TrapMine::Serialize(nlohmann::json& out) const
{
    out["atk"]            = atk;
    out["damage_type"]    = static_cast<int>(damage_type);
    out["triggered"]      = triggered;
    out["trigger_radius"] = trigger_radius;
}

void TrapMine::Deserialize(const nlohmann::json& in)
{
    if (in.contains("atk"))            atk            = in["atk"];
    if (in.contains("damage_type"))    damage_type    = static_cast<DamageType>(static_cast<int>(in["damage_type"]));
    if (in.contains("triggered"))      triggered      = in["triggered"];
    if (in.contains("trigger_radius")) trigger_radius = in["trigger_radius"];
}
