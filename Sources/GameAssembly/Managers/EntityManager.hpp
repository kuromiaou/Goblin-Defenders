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

    //Un getter de enemyCurrentWave en fonction de current_wave

private:

    enum class WaveState { WAITING, IN_PROGRESS, COMPLETED };


    int        current_wave = 0;
    int        max_fixed_waves = 10;
    WaveState  state;
    bool       is_endless = false;

    std::vector<std::shared_ptr<Enemy>> allEnemies;

    

    //Un getter de enemyCurrentWave en fonction de current_wave
    std::vector<int> GoblinWave         = { 0,1,1,2,2,2,3,3,3,3,3 };
    std::vector<int> GoblinRapaceWave   = { 0,1,1,2,2,2,3,3,3,3,3 };
    std::vector<int> HobGoblinWave      = { 0,1,1,2,2,2,3,3,3,3,3 };
    std::vector<int> MagicienWave       = { 0,1,1,2,2,2,3,3,3,3,3 };
    std::vector<int> ShamanWave         = { 0,1,1,2,2,2,3,3,3,3,3 };
    std::vector<int> enemyCurrentWave   = {};
};
