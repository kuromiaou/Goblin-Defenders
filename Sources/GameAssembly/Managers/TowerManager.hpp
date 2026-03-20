#pragma once

#include <Termina/Scripting/API/ScriptingAPI.hpp>

#include "GameAssembly/Structures/TowerAOE.hpp"
#include "GameAssembly/Structures/TowerSingle.hpp"
#include "GameAssembly/Structures/TowerCC.hpp"

#include <iostream>
#include <chrono>
#include <algorithm>
#include <vector>

using namespace TerminaScript;

class TowerManager : public TerminaScript::ScriptableComponent
{
public:
    TowerManager() = default;
    TowerManager(Termina::Actor* owner) : TerminaScript::ScriptableComponent(owner) {}

    void Start()  override;
    void Update(float deltaTime) override;

private:
    std::vector<std::shared_ptr<Tower>> allTraps;
};
