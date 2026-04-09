#pragma once

#include "Enemy.hpp"
#include "Goblin.hpp"
#include "Hobgoblin.hpp"
#include "GoblinRapace.hpp"
#include "Magicien.hpp"
#include "Shaman.hpp"

#include <Termina/World/World.hpp>
#include <vector>
#include <utility>

// Returns the Enemy interface for an actor, or nullptr if the actor is not an enemy.
inline Enemy* GetEnemyComponent(Termina::Actor* actor)
{
    if (!actor) return nullptr;
    if (actor->HasComponent<Goblin>())       return &actor->GetComponent<Goblin>();
    if (actor->HasComponent<Hobgoblin>())    return &actor->GetComponent<Hobgoblin>();
    if (actor->HasComponent<GoblinRapace>()) return &actor->GetComponent<GoblinRapace>();
    if (actor->HasComponent<Magicien>())     return &actor->GetComponent<Magicien>();
    if (actor->HasComponent<Shaman>())       return &actor->GetComponent<Shaman>();
    return nullptr;
}

// Returns (actor*, Enemy*) pairs for every live (hp > 0) enemy currently in the world.
inline std::vector<std::pair<Termina::Actor*, Enemy*>>
GetAllLiveEnemies(Termina::World* world)
{
    std::vector<std::pair<Termina::Actor*, Enemy*>> result;
    if (!world) return result;

    for (const auto& actor : world->GetActors())
    {
        if (!actor || !actor->IsActive()) continue;
        Enemy* e = GetEnemyComponent(actor.get());
        if (e && !e->isDead())
            result.emplace_back(actor.get(), e);
    }
    return result;
}
