#include <Termina/Scripting/API/ScriptingAPI.hpp>
#include <Termina/World/ComponentRegistry.hpp>
#include <ImGui/imgui.h>

#include "Camera/FlyCamComponent.hpp"
#include "Tests/ParticleSystem.hpp"
#include "Tests/PhysicsTestComponent.hpp"

// --- Goblin Defenders ---
#include "Player/PlayerController.hpp"
#include "Player/Player.hpp"
#include "Enemy/Goblin.hpp"
#include "Enemy/Hobgoblin.hpp"
#include "Enemy/Magicien.hpp"
#include "Enemy/Shaman.hpp"
#include "Enemy/GoblinRapace.hpp"
#include "Structures/TowerSingle.hpp"
#include "Structures/TowerAOE.hpp"
#include "Structures/TowerCC.hpp"
#include "Structures/TrapWolf.hpp"
#include "Structures/TrapMine.hpp"
#include "Structures/TrapAuraGold.hpp"
#include "Structures/Nexus.hpp"
#include "Structures/Door.hpp"
#include "Managers/ProjectileManager.hpp"
#include "Managers/EntityManager.hpp"
#include "Enemy/CheckPoint.hpp"
<<<<<<< HEAD
#include "Structures/MainMenu.hpp"
=======
#include "Enemy/EnemyMovement.hpp"
>>>>>>> BaseFeatures

COMPONENT_MODULE_BEGIN()
    // Termina
    REGISTER_COMPONENT(FlyCamComponent,        "Fly Cam Component")
    REGISTER_COMPONENT(ParticleSystemComponent,"Particle System")
    REGISTER_COMPONENT(PhysicsTestComponent,   "Physics Test")

    // Goblin Defenders
    REGISTER_COMPONENT(PlayerController,       "Player Controller")
    REGISTER_COMPONENT(Player,                 "Player")
    REGISTER_COMPONENT(Nexus,                  "Nexus")
    REGISTER_COMPONENT(Goblin,                 "Goblin")
    REGISTER_COMPONENT(Hobgoblin,              "Hobgoblin")
    REGISTER_COMPONENT(Magicien,               "Magicien")
    REGISTER_COMPONENT(Shaman,                 "Shaman")
    REGISTER_COMPONENT(Checkpoint,             "CheckPoint")
    REGISTER_COMPONENT(Door,                   "Spawner")
    REGISTER_COMPONENT(EnemyMovement,          "Enemy Movement")
    REGISTER_COMPONENT(GoblinRapace,           "Goblin Rapace")
    REGISTER_COMPONENT(TowerSingle,            "Tour Single")
    REGISTER_COMPONENT(TowerAOE,               "Tour AOE")
    REGISTER_COMPONENT(TowerCC,                "Tour CC")
    REGISTER_COMPONENT(TrapWolf,               "Piège à Loup")
    REGISTER_COMPONENT(TrapMine,               "Mine")
    REGISTER_COMPONENT(TrapAuraGold,           "Aura Gold")

    REGISTER_COMPONENT(MainMenuComponent,      "MainMenuComponent")

    REGISTER_COMPONENT(EntityManager,          "Entity Manager")
    REGISTER_COMPONENT(ProjectileManager,      "Projectile Manager")
COMPONENT_MODULE_END()

TERMINA_DLL_EXPORT void SetImGuiContext(void* ctx, void* allocFunc, void* freeFunc, void* userData)
{
    ImGui::SetCurrentContext(static_cast<ImGuiContext*>(ctx));
    ImGui::SetAllocatorFunctions(
        reinterpret_cast<ImGuiMemAllocFunc>(allocFunc),
        reinterpret_cast<ImGuiMemFreeFunc>(freeFunc),
        userData);
}
