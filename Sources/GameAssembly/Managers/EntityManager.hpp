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
#include "GameAssembly/Enemy/EnemyMovement.hpp"

#include <iostream>
#include <chrono>
#include <algorithm>
#include <vector>

using namespace TerminaScript;

class EntityManager : public TerminaScript::ScriptableComponent {

public:
    EntityManager() = default;
    EntityManager(Termina::Actor* owner) : TerminaScript::ScriptableComponent(owner) {}

    void Start()  override;
    void Update(float deltaTime) override;

    // Lance le spawn de tous les ennemis de la vague donnée
    void SpawnWave(int waveIndex);

    std::vector<Door*> GetDoors();

    void CleanupDeadEnemies();

    void SpawnEnemyByType(int enemyType, const glm::vec3& position);

private:

    // Spawn individuels pour chaque type d'ennemi
    void SpawnGoblin(const glm::vec3& position);
    void SpawnGoblinRapace(const glm::vec3& position);
    void SpawnHobgoblin(const glm::vec3& position);
    void SpawnMagicien(const glm::vec3& position);
    void SpawnShamanATK(const glm::vec3& position);
    void SpawnShamanSPD(const glm::vec3& position);
    void SpawnShamanRES(const glm::vec3& position);

    enum class WaveState { WAITING, IN_PROGRESS, COMPLETED };


    int        current_wave = 1;
    int        max_fixed_waves = 10;
    WaveState  state;
    bool       is_endless = false, is_wave_spawned = false;

    // Stocke les acteurs des ennemis spawés (pas les composants)
    std::vector<Termina::Actor*> allEnemies;


    enum class EnemyType {
        GOBLIN = 0,
        GOBLINRAPACE = 1,
        HOBGOBLIN = 2,
        MAGICIEN = 3,
        SHAMANATK = 4,
        SHAMANSPD = 5,
        SHAMANRES = 6
    };


    std::vector<std::vector<int>> EnemySpawnOrder = {
         {},                   // Vague 0
         { 2, 3, 5, 1, 0 },       // Vague 1
         { 2, 0, 3, 4 },       // Vague 2
         { 2, 0, 3, 4 },       // Vague 3
         { 2, 0, 3, 4 },       // Vague 4
         { 2, 0, 3, 4 },       // Vague 5
         { 2, 0, 3, 4 },       // Vague 6
         { 2, 0, 3, 4 },       // Vague 7
         { 2, 0, 3, 4 },       // Vague 8
         { 2, 0, 3, 4 },       // Vague 9
         { 2, 0, 3, 4 }        // Vague 10
    };

    TerminaScript::Prefab p_Goblin;
    TerminaScript::Prefab p_GoblinRapace;
    TerminaScript::Prefab p_HobGoblin;
    TerminaScript::Prefab p_Magicien;
    TerminaScript::Prefab p_ShamanATK;
    TerminaScript::Prefab p_ShamanSPD;
    TerminaScript::Prefab p_ShamanRES;

    std::vector<Door*> m_Doors;

};