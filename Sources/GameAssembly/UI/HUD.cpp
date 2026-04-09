#include "HUD.hpp"

#include <algorithm>
#include <string>
#include <GLM/glm.hpp>
#include <ImGui/imgui.h>
#include <Termina/Core/Application.hpp>
#include <Termina/World/WorldSystem.hpp>
#include <GameAssembly/Managers/EntityManager.hpp>
#include <GameAssembly/Player/Player.hpp>
#include <GameAssembly/Structures/Nexus.hpp>
#include <GameAssembly/Structures/TowerSingle.hpp>
#include <GameAssembly/Structures/TowerAOE.hpp>
#include <GameAssembly/Structures/TowerCC.hpp>

namespace
{
constexpr const char* kPlayerActorName = "Player";
constexpr const char* kNexusActorName = "Nexus";
constexpr const char* kMenuWorldPath = "Assets/Worlds/Menu";
constexpr int kTowerBuildCost = 50;

bool IsTowerActor(const Termina::Actor* actor)
{
    return actor && (actor->HasComponent<TowerSingle>() || actor->HasComponent<TowerAOE>() || actor->HasComponent<TowerCC>());
}

bool IsTowerSlotName(const std::string& name)
{
    return name.rfind("Tower_Base", 0) == 0 && name != "Tower_Bases";
}

bool IsSlotOccupied(const Termina::Actor* slot, const std::vector<std::shared_ptr<Termina::Actor>>& worldActors)
{
    if (!slot || !slot->HasComponent<Termina::Transform>()) return true;

    const glm::vec3 slotPos = slot->GetComponent<Termina::Transform>().GetPosition();
    constexpr float kOccupiedDistance = 1.2f;

    for (const auto& actor : worldActors) {
        if (!actor || actor.get() == slot || !actor->IsActive() || !actor->HasComponent<Termina::Transform>()) continue;
        if (!IsTowerActor(actor.get())) continue;

        const float distance = glm::distance(slotPos, actor->GetComponent<Termina::Transform>().GetPosition());
        if (distance <= kOccupiedDistance) return true;
    }
    return false;
}

bool WorldToScreen(const glm::vec3& worldPos, const Termina::Camera& camera, float width, float height, ImVec2& outScreenPos)
{
    const glm::vec4 clip = camera.ViewProjection * glm::vec4(worldPos, 1.0f);
    if (clip.w <= 0.0f) return false;

    const glm::vec3 ndc = glm::vec3(clip) / clip.w;
    if (ndc.z < -1.0f || ndc.z > 1.0f) return false;

    outScreenPos.x = (ndc.x * 0.5f + 0.5f) * width;
    outScreenPos.y = (1.0f - (ndc.y * 0.5f + 0.5f)) * height;
    return true;
}
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

    bool isWin  = false;
    if (m_EntityManagerActor && m_EntityManagerActor->HasComponent<EntityManager>()) {
        isWin = m_EntityManagerActor->GetComponent<EntityManager>().hasWon;
    }
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

    // ==== TOWER SLOTS ====
    if (m_PlayerActor && m_PlayerActor->HasComponent<Player>() && m_TowerPrefab.IsValid())
    {
        auto* world = m_Owner->GetParentWorld();
        const Termina::Camera camera = world->GetMainCamera();
        Player& player = m_PlayerActor->GetComponent<Player>();

        for (const auto& actor : worldActors)
        {
            if (!actor || !actor->IsActive() || !actor->HasComponent<Termina::Transform>()) continue;
            if (!IsTowerSlotName(actor->GetName())) continue;

            const bool occupied = IsSlotOccupied(actor.get(), worldActors);
            const glm::vec3 worldPos = actor->GetComponent<Termina::Transform>().GetPosition() + glm::vec3(0.0f, 0.5f, 0.0f);

            ImVec2 screenPos;
            if (!WorldToScreen(worldPos, camera, width, height, screenPos)) continue;

            const std::string windowId = "TowerSlotUI##" + std::to_string(actor->GetID());
            ImGui::SetNextWindowPos(screenPos, ImGuiCond_Always, ImVec2(0.5f, 0.5f));
            ImGui::SetNextWindowBgAlpha(0.25f);
            ImGui::Begin(windowId.c_str(), nullptr,
                ImGuiWindowFlags_NoDecoration |
                ImGuiWindowFlags_NoMove |
                ImGuiWindowFlags_AlwaysAutoResize |
                ImGuiWindowFlags_NoSavedSettings);

            if (occupied) {
                ImGui::Text("Occupé");
            } else {
                const bool canAfford = player.getGold() >= kTowerBuildCost;
                const std::string buildLabel = "Construire (" + std::to_string(kTowerBuildCost) + "G)";
                ImGui::BeginDisabled(!canAfford);
                if (ImGui::Button(buildLabel.c_str(), ImVec2(130.0f, 0.0f)))
                {
                    if (player.spendGold(kTowerBuildCost))
                    {
                        if (Termina::Actor* newTower = Instantiate(m_TowerPrefab))
                        {
                            newTower->GetComponent<Termina::Transform>().SetPosition(actor->GetComponent<Termina::Transform>().GetPosition());
                            currentGold = player.getGold();
                        }
                        else
                        {
                            player.addGold(kTowerBuildCost);
                        }
                    }
                }
                ImGui::EndDisabled();
            }

            ImGui::End();
        }
    }

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
    ImGui::SetNextWindowPos(ImVec2(width * 0.175f, height * 0.8f), ImGuiCond_Always, ImVec2(0.5f, 0.5f));
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
