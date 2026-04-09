#include "HUD.hpp"

#include <algorithm>
#include <ImGui/imgui.h>
#include <Termina/Core/Application.hpp>
#include <Termina/World/WorldSystem.hpp>
#include <GameAssembly/Managers/EntityManager.hpp>
#include <GameAssembly/Player/Player.hpp>
#include <GameAssembly/Structures/Nexus.hpp>

namespace
{
constexpr const char* kPlayerActorName = "Player";
constexpr const char* kNexusActorName = "Nexus";
constexpr const char* kMenuWorldPath = "Assets/Worlds/Menu";
}

void HUDComponent::OnRender(float dt)
{
    ImGuiIO& io = ImGui::GetIO();

    float width = io.DisplaySize.x;
    float height = io.DisplaySize.y;
    auto& worldActors = m_Owner->GetParentWorld()->GetActors();

    // Fen�tre fullscreen
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

    int currentGold = 0;
    if (!m_PlayerActor || !m_PlayerActor->IsActive() || !m_PlayerActor->HasComponent<Player>()) {
        m_PlayerActor = nullptr;
        for (const auto& actor : worldActors) {
            if (actor->HasComponent<Player>() || actor->GetName() == kPlayerActorName) {
                m_PlayerActor = actor.get();
                break;
            }
        }
    }
    if (m_PlayerActor && !m_PlayerActor->HasComponent<Player>()) {
        // NOTE: Le composant "Player" doit être posé dans la scène/prefab du joueur (pas d'AddComponent runtime).
        TN_ERROR("HUD: missing Player component on '%s'.", m_PlayerActor->GetName().c_str());
        m_PlayerActor = nullptr;
    }
    if (m_PlayerActor && m_PlayerActor->HasComponent<Player>()) {
        currentGold = m_PlayerActor->GetComponent<Player>().getGold();
    }

    int currentNexusHP = 0;
    int currentNexusHPMax = 1;
    if (!m_NexusActor || !m_NexusActor->IsActive() || !m_NexusActor->HasComponent<Nexus>()) {
        m_NexusActor = nullptr;
        for (const auto& actor : worldActors) {
            if (actor->HasComponent<Nexus>() || actor->GetName() == kNexusActorName) {
                m_NexusActor = actor.get();
                break;
            }
        }
    }
    if (m_NexusActor && !m_NexusActor->HasComponent<Nexus>()) {
        // NOTE: Le composant "Nexus" doit être posé dans la scène/prefab du nexus (pas d'AddComponent runtime).
        TN_ERROR("HUD: missing Nexus component on '%s'.", m_NexusActor->GetName().c_str());
        m_NexusActor = nullptr;
    }
    if (m_NexusActor && m_NexusActor->HasComponent<Nexus>()) {
        const Nexus& nexus = m_NexusActor->GetComponent<Nexus>();
        currentNexusHP = nexus.getHP();
        currentNexusHPMax = std::max(1, nexus.getHPMax());
    }

    // Résoudre l'EntityManager
    if (!m_EntityManagerActor || !m_EntityManagerActor->IsActive() || !m_EntityManagerActor->HasComponent<EntityManager>()) {
        m_EntityManagerActor = nullptr;
        for (const auto& actor : worldActors) {
            if (actor->HasComponent<EntityManager>()) {
                m_EntityManagerActor = actor.get();
                break;
            }
        }
    }

    bool isWin  = m_EntityManagerActor && m_EntityManagerActor->HasComponent<EntityManager>()
                  && m_EntityManagerActor->GetComponent<EntityManager>().hasWon;
    bool isLose = (currentNexusHP <= 0) && m_NexusActor != nullptr;

    // ==== OVERLAY WIN / LOSE ====
    if (isWin || isLose)
    {
        // Fond semi-transparent
        ImDrawList* overlayDraw = ImGui::GetForegroundDrawList();
        overlayDraw->AddRectFilled(ImVec2(0, 0), io.DisplaySize, IM_COL32(0, 0, 0, 160));

        const char* resultText = isWin ? "Victoire" : "Défaite";
        ImVec4      textColor  = isWin ? ImVec4(0.0f, 1.0f, 0.0f, 1.0f)
                                       : ImVec4(1.0f, 0.0f, 0.0f, 1.0f);

        // Texte centré au milieu de l'écran
        ImGui::SetNextWindowPos(ImVec2(width * 0.5f, height * 0.45f), ImGuiCond_Always, ImVec2(0.5f, 0.5f));
        ImGui::SetNextWindowBgAlpha(0.0f);
        ImGui::Begin("GameResult", nullptr,
            ImGuiWindowFlags_NoDecoration |
            ImGuiWindowFlags_NoMove |
            ImGuiWindowFlags_AlwaysAutoResize |
            ImGuiWindowFlags_NoNav);

        ImGui::PushStyleColor(ImGuiCol_Text, textColor);
        ImGui::SetWindowFontScale(3.0f);
        ImGui::Text("%s", resultText);
        ImGui::SetWindowFontScale(1.0f);
        ImGui::PopStyleColor();

        ImGui::End();

        // Bouton retour au menu
        ImGui::SetNextWindowPos(ImVec2(width * 0.5f, height * 0.6f), ImGuiCond_Always, ImVec2(0.5f, 0.5f));
        ImGui::SetNextWindowBgAlpha(0.0f);
        ImGui::Begin("ReturnMenu", nullptr,
            ImGuiWindowFlags_NoDecoration |
            ImGuiWindowFlags_NoMove |
            ImGuiWindowFlags_AlwaysAutoResize |
            ImGuiWindowFlags_NoNav);

        if (ImGui::Button("Menu Principal", ImVec2(220, 55)))
        {
            auto* worldSystem = Termina::Application::GetSystem<Termina::WorldSystem>();
            if (worldSystem)
            {
                worldSystem->LoadWorld(kMenuWorldPath);
                worldSystem->Play();
            }
        }

        ImGui::End();

        // Ne pas afficher le reste du HUD pendant l'écran de résultat
        return;
    }

    ImGui::SetNextWindowPos(ImVec2(width * 0.18f, height * 0.15f), ImGuiCond_Always, ImVec2(0.5f, 0.0f));
    ImGui::SetNextWindowBgAlpha(0.0f);
    ImGui::Begin("Nexus Health", nullptr,
        ImGuiWindowFlags_NoDecoration |
        ImGuiWindowFlags_NoMove |
        ImGuiWindowFlags_AlwaysAutoResize);
    ImGui::Text("Nexus: %d / %d", currentNexusHP, currentNexusHPMax);
    ImGui::ProgressBar(
        static_cast<float>(currentNexusHP) / static_cast<float>(currentNexusHPMax),
        ImVec2(220.0f, 18.0f));
    ImGui::End();

    ImGui::SetNextWindowPos(ImVec2(width * 0.85f, height * 0.08f), ImGuiCond_Always, ImVec2(0.5f, 0.0f));
    ImGui::SetNextWindowBgAlpha(0.0f);
    ImGui::Begin("Gold", nullptr,
        ImGuiWindowFlags_NoDecoration |
        ImGuiWindowFlags_NoMove |
        ImGuiWindowFlags_AlwaysAutoResize);
    ImGui::Text("Gold: %d", currentGold);
    ImGui::End();

    // ==== WAVE ====
    ImGui::SetNextWindowPos(ImVec2(width * 0.5f, height * 0.15f), ImGuiCond_Always, ImVec2(0.5f, 0.0f));
    ImGui::SetNextWindowBgAlpha(0.0f);

    ImGui::Begin("Wave", nullptr,
        ImGuiWindowFlags_NoDecoration |
        ImGuiWindowFlags_NoMove |
        ImGuiWindowFlags_AlwaysAutoResize);

    // Get current wave from EntityManager
    int currentWave = 1;
    for (const auto& actor : worldActors) {
        if (actor->HasComponent<EntityManager>()) {
            EntityManager& manager = actor->GetComponent<EntityManager>();
            currentWave = manager.current_wave;
            break;
        }
    }

    ImGui::Text("Wave %d", currentWave);

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
