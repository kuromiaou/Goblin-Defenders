#include "Checkpoint.hpp"
#include <ImGui/imgui.h>

void Checkpoint::Start()
{
}

void Checkpoint::Update(float Deltatime)
{
}

void Checkpoint::Inspect()
{
	ImGui::DragInt("CheckPoint", &m_Order, 1, 0, m_CheckPointMax);
}

