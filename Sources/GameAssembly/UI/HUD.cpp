#include "HUD.hpp"

#include <ImGui/imgui.h>
#include <Termina/Core/Application.hpp>
#include <Termina/World/WorldSystem.hpp>
#include <GameAssembly/Managers/EntityManager.hpp>

void HUDComponent::OnRender(float dt)
{
    ImGuiIO& io = ImGui::GetIO();

    float width = io.DisplaySize.x;
    float height = io.DisplaySize.y;

    // Fenêtre fullscreen
    ImGui::SetNextWindowPos(ImVec2(0, 0));
    ImGui::SetNextWindowSize(io.DisplaySize);

    ImGui::Begin("Background", nullptr,
        ImGuiWindowFlags_NoDecoration |
        ImGuiWindowFlags_NoMove |
        ImGuiWindowFlags_NoInputs |
        ImGuiWindowFlags_NoBringToFrontOnFocus);

    ImDrawList* drawList = ImGui::GetBackgroundDrawList();

    drawList->AddRectFilled(
        ImVec2(0, 0),
        io.DisplaySize,
        IM_COL32(0, 0, 0, 255)
    );

    ImGui::End();

    // ==== WAVE ====
    ImGui::SetNextWindowPos(ImVec2(width * 0.5f, height * 0.15f), ImGuiCond_Always, ImVec2(0.5f, 0.0f));
    ImGui::SetNextWindowBgAlpha(0.0f);

    ImGui::Begin("Wave", nullptr,
        ImGuiWindowFlags_NoDecoration |
        ImGuiWindowFlags_NoMove |
        ImGuiWindowFlags_AlwaysAutoResize);

    ImGui::Text("Wave 1");

    ImGui::End();

    // ==== START ====
    ImGui::SetNextWindowPos(ImVec2(width * 0.175f, height * 0.175f), ImGuiCond_Always, ImVec2(0.5f, 0.5f));
    ImGui::SetNextWindowBgAlpha(0.0f);

    ImGui::Begin("Start", nullptr,
        ImGuiWindowFlags_NoDecoration |
        ImGuiWindowFlags_NoMove |
        ImGuiWindowFlags_AlwaysAutoResize);

    if (ImGui::Button("Start", ImVec2(200, 50)))
    {
        // Get the EntityManager from the world and start the wave
        auto& worldActors = m_Owner->GetParentWorld()->GetActors();
        for (const auto& actor : worldActors) {
            if (actor->HasComponent<EntityManager>()) {
                EntityManager& manager = actor->GetComponent<EntityManager>();
                manager.state = EntityManager::WaveState::IN_PROGRESS;  
                break;
            }
        }
    }

    ImGui::End();
}