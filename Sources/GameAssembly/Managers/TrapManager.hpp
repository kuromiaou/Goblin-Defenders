#pragma once

#include <Termina/Scripting/API/ScriptingAPI.hpp>

#include "GameAssembly/Structures/TrapAuraGold.hpp"
#include "GameAssembly/Structures/TrapMine.hpp"
#include "GameAssembly/Structures/TrapWolf.hpp"

#include <iostream>
#include <chrono>
#include <algorithm>
#include <vector>

using namespace TerminaScript;

class TrapManager : public TerminaScript::ScriptableComponent
{
public:
    TrapManager() = default;
    TrapManager(Termina::Actor* owner) : TerminaScript::ScriptableComponent(owner) {}

    void Start()  override;
    void Update(float deltaTime) override;

private:
    std::vector<std::shared_ptr<Trap>> allTraps;
};
