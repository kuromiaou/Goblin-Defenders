#include <Termina/Scripting/API/ScriptingAPI.hpp>
#include <Termina/World/ComponentRegistry.hpp>

#include "FlyCamComponent.hpp"
#include "ParticleSystem.hpp"
#include "PhysicsTestComponent.hpp"

// --- Goblin Defenders ---
#include "Player.hpp"
#include "Nexus.hpp"
#include "Goblin.hpp"
#include "Hobgoblin.hpp"
#include "Magicien.hpp"
#include "Shaman.hpp"
#include "GoblinRapace.hpp"
#include "TowerSingle.hpp"
#include "TowerAOE.hpp"
#include "TowerCC.hpp"
#include "TrapWolf.hpp"
#include "TrapMine.hpp"
#include "TrapAuraGold.hpp"

COMPONENT_MODULE_BEGIN()
    // Moteur
    REGISTER_COMPONENT(FlyCamComponent,       "Fly Cam Component")
    REGISTER_COMPONENT(ParticleSystemComponent,"Particle System")
    REGISTER_COMPONENT(PhysicsTestComponent,  "Physics Test")

    // Goblin Defenders
    REGISTER_COMPONENT(Player,       "Player")
    REGISTER_COMPONENT(Nexus,        "Nexus")
    REGISTER_COMPONENT(Goblin,       "Goblin")
    REGISTER_COMPONENT(Hobgoblin,    "Hobgoblin")
    REGISTER_COMPONENT(Magicien,     "Magicien")
    REGISTER_COMPONENT(Shaman,       "Shaman")
    REGISTER_COMPONENT(GoblinRapace, "Goblin Rapace")
    REGISTER_COMPONENT(TowerSingle,  "Tour Single")
    REGISTER_COMPONENT(TowerAOE,     "Tour AOE")
    REGISTER_COMPONENT(TowerCC,      "Tour CC")
    REGISTER_COMPONENT(TrapWolf,     "Piège à Loup")
    REGISTER_COMPONENT(TrapMine,     "Mine")
    REGISTER_COMPONENT(TrapAuraGold, "Aura Gold")
COMPONENT_MODULE_END()
