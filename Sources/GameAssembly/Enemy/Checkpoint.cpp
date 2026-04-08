#include "Checkpoint.hpp"
#include <ImGui/imgui.h>

void Checkpoint::Start()
{
    // S'ajouter à la liste globale des checkpoints
    GetAllCheckpoints().push_back(this);
    TN_INFO("Checkpoint %d registered", m_Order);
}

void Checkpoint::Update(float Deltatime)
{
}

void Checkpoint::Inspect()
{
    ImGui::DragInt("CheckPoint", &m_Order, 1, 0, m_CheckPointMax);
}

int Checkpoint::getOrder() const
{
    return m_Order;
}

void Checkpoint::Serialize(nlohmann::json& out) const
{
    out["order"] = m_Order;
}

void Checkpoint::Deserialize(const nlohmann::json& in)
{
    if (in.contains("order")) {
        m_Order = in["order"];
    }
}