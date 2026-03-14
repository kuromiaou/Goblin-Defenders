#include "LauncherApplication.hpp"
#include "ImGui/imgui.h"
#include "Termina/Input/InputSystem.hpp"

#include <Termina/Renderer/Renderer.hpp>
#include <Termina/Shader/ShaderManager.hpp>
#include <Termina/World/WorldSystem.hpp>
#include <Termina/World/ComponentRegistry.hpp>

LauncherApplication::LauncherApplication()
    : Application("LAUNCHER")
{
    m_SystemManager.AddSystem<Termina::InputSystem>(m_Window->GetHandle());
    m_SystemManager.AddSystem<Termina::WorldSystem>();
    auto renderer = m_SystemManager.AddSystem<Termina::RendererSystem>(m_Window);
    m_SystemManager.AddSystem<Termina::ShaderManager>();

    renderer->SetShouldImGuiClear(true);

    Termina::ComponentRegistry::Get().Report();
}

void LauncherApplication::OnPreUpdate(float dt)
{
    ImGui::ShowDemoWindow();
}
