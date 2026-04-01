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

    // Lance le spawn de tous les ennemis de la vague donnée
    void SpawnWave(int waveIndex);

private:
    // Spawn individuels pour chaque type d'ennemi
    void SpawnGoblin();
    void SpawnGoblinRapace();
    void SpawnHobgoblin();
    void SpawnMagicien();
    void SpawnShamanATK();
    void SpawnShamanSPD();
    void SpawnShamanRES();

    enum class WaveState { WAITING, IN_PROGRESS, COMPLETED };

    int        current_wave = 0;
    int        max_fixed_waves = 10;
    WaveState  state;
    bool       is_endless = false;

    // Stocke les acteurs des ennemis spawés (pas les composants)
    std::vector<Termina::Actor*> allEnemies;

    // Vagues définissant le nombre d'ennemis à spawn à chaque vague
    std::vector<int> GoblinWave = { 0,1,1,2,2,2,3,3,3,3,3 };
    std::vector<int> GoblinRapaceWave = { 0,1,1,2,2,2,3,3,3,3,3 };
    std::vector<int> HobGoblinWave = { 0,1,1,2,2,2,3,3,3,3,3 };
    std::vector<int> MagicienWave = { 0,1,1,2,2,2,3,3,3,3,3 };
    std::vector<int> ShamanATKWave = { 0,1,1,2,2,2,3,3,3,3,3 };
    std::vector<int> ShamanSPDWave = { 0,1,1,2,2,2,3,3,3,3,3 };
    std::vector<int> ShamanRESWave = { 0,1,1,2,2,2,3,3,3,3,3 };

    Termina::Actor* m_Player = nullptr;
};