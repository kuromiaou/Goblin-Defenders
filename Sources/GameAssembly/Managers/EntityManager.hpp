#pragma once

#include <Termina/Scripting/API/ScriptingAPI.hpp>

#include "GameAssembly/Enemy/Goblin.hpp"
#include "GameAssembly/Enemy/GoblinRapace.hpp"
#include "GameAssembly/Enemy/Hobgoblin.hpp"
#include "GameAssembly/Enemy/Magicien.hpp"
#include "GameAssembly/Enemy/Shaman.hpp"
#include "GameAssembly/Player/Player.hpp"
#include "GameAssembly/Structures/Door.hpp"
#include "GameAssembly/Structures/Nexus.hpp"


#include <iostream>
#include <chrono>
#include <algorithm>
#include <vector>

using namespace TerminaScript;

class EntityManager : public TerminaScript::ScriptableComponent
{
public:
    EntityManager() = default;
    EntityManager(Termina::Actor* owner) : TerminaScript::ScriptableComponent(owner) {}

    void Start()  override;
    void Update(float deltaTime) override;

private:
    std::vector<std::shared_ptr<Enemy>> allEnemies;
};
