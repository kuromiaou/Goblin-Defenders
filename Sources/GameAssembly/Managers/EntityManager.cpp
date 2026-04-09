#include "EntityManager.hpp"

void EntityManager::Start()
{
    Checkpoint::ClearAllCheckpoints();

    state = WaveState::WAITING;

    // Charger les pr�fabs des ennemis
    p_Goblin = TerminaScript::Prefab("Assets/Prefabs/Enemies/goblin.trp");
    if (p_Goblin.IsValid())
        TN_INFO("Goblin prefab loaded");

    p_GoblinRapace = TerminaScript::Prefab("Assets/Prefabs/Enemies/thief.trp");
    p_HobGoblin    = TerminaScript::Prefab("Assets/Prefabs/Enemies/hobgoblin.trp");
    p_Magicien     = TerminaScript::Prefab("Assets/Prefabs/Enemies/mage.trp");
    p_ShamanATK    = TerminaScript::Prefab("Assets/Prefabs/Enemies/shaman ATK .trp");
    p_ShamanSPD    = TerminaScript::Prefab("Assets/Prefabs/Enemies/shaman SPD .trp");
    p_ShamanRES    = TerminaScript::Prefab("Assets/Prefabs/Enemies/shaman DEF .trp");

    // R�cup�rer et cacher les portes
    m_Doors = GetDoors();

    if (m_Doors.empty())
        TN_WARN("No doors found in the world!");
}

void EntityManager::Update(float deltaTime)
{
    // Nettoyer les ennemis morts/d�truits
    CleanupDeadEnemies();

    switch (state) {
    case WaveState::WAITING:
        // Attendre avant de commencer la vague suivante
        break;

    case WaveState::IN_PROGRESS:
        if (!is_wave_spawned) {
            SpawnWave(current_wave);
            is_wave_spawned = true;
            TN_INFO("Wave %d started with %zu enemies", current_wave, allEnemies.size());
        }

        // V�rifier si tous les ennemis de la vague sont morts
        if (allEnemies.empty()) {
            state = WaveState::COMPLETED;
            TN_INFO("Wave %d completed!", current_wave);
        }
        break;

    case WaveState::COMPLETED:
        is_wave_spawned = false;
        current_wave++;

        // V�rifier si on a atteint le max des vagues fixes
        if (current_wave > max_fixed_waves) {
            if (is_endless) {
                // G�n�rer une vague infinie al�atoire
                state = WaveState::WAITING;
            }
            else {
                TN_INFO("All waves completed! Game Over!");
                state = WaveState::WAITING;
            }
        }
        else {
            state = WaveState::WAITING;
        }
        break;
    }
}

std::vector<Door*> EntityManager::GetDoors()
{
    std::vector<Door*> doors;
    const auto& actors = m_Owner->GetParentWorld()->GetActors();

    for (const auto& actor : actors) {
        if (actor->HasComponent<Door>()) {
            doors.push_back(&actor->GetComponent<Door>());
        }
    }

    return doors;
}

void EntityManager::CleanupDeadEnemies()
{
    // Supprimer les ennemis qui ont �t� d�truits
    allEnemies.erase(
        std::remove_if(allEnemies.begin(), allEnemies.end(),
            [this](Termina::Actor* enemy) {
                // V�rifier si l'ennemi a �t� supprim� du monde
                if (!enemy) return true;

                const auto& actors = m_Owner->GetParentWorld()->GetActors();
                return std::find_if(actors.begin(), actors.end(),
                    [enemy](const std::shared_ptr<Termina::Actor>& actor) {
                        return actor.get() == enemy;
                    }) == actors.end();
            }),
        allEnemies.end()
    );
}

void EntityManager::SpawnWave(int waveIndex)
{
    // V�rifier que l'index est valide
    if (waveIndex < 0 || waveIndex >= static_cast<int>(EnemySpawnOrder.size())) {
        TN_WARN("Wave index %d out of range!", waveIndex);
        return;
    }

    if (m_Doors.empty()) {
        TN_ERROR("No doors available to spawn enemies!");
        return;
    }

    // R�cup�rer la liste des types d'ennemis pour cette vague
    const auto& waveEnemies = EnemySpawnOrder[waveIndex];

    // Spawner chaque ennemi de la vague
    for (int i = 0; i < static_cast<int>(waveEnemies.size()); i++) {
        int enemyType = waveEnemies[i];

        // S�lectionner une porte al�atoire pour le spawn
        Door* door = m_Doors[i % m_Doors.size()];
        glm::vec3 spawnPosition = door->getPosition();

        // Ajouter un l�ger offset pour �viter que les ennemis spawn au m�me endroit
        spawnPosition.x += (float)(i % 3);
        spawnPosition.z += (float)(i / 3);

        SpawnEnemyByType(enemyType, spawnPosition);
    }
}

void EntityManager::SpawnEnemyByType(int enemyType, const glm::vec3& position)
{
    Termina::Actor* enemy = nullptr;

    switch (static_cast<EnemyType>(enemyType)) {
    case EnemyType::GOBLIN:
        SpawnGoblin(position);
        break;
    case EnemyType::GOBLINRAPACE:
        SpawnGoblinRapace(position);
        break;
    case EnemyType::HOBGOBLIN:
        SpawnHobgoblin(position);
        break;
    case EnemyType::MAGICIEN:
        SpawnMagicien(position);
        break;
    case EnemyType::SHAMANATK:
        SpawnShamanATK(position);
        break;
    case EnemyType::SHAMANSPD:
        SpawnShamanSPD(position);
        break;
    case EnemyType::SHAMANRES:
        SpawnShamanRES(position);
        break;
    default:
        TN_WARN("Unknown enemy type: %d", enemyType);
        break;
    }
}

void EntityManager::SpawnGoblin(const glm::vec3& position)
{
    Termina::Actor* goblin = Instantiate(p_Goblin);
    if (goblin && goblin->HasComponent<Termina::Transform>()) {
        goblin->GetComponent<Termina::Transform>().SetPosition(position);
        allEnemies.push_back(goblin);
        TN_INFO("Goblin spawned at (%.2f, %.2f, %.2f)", position.x, position.y, position.z);
    }
}

void EntityManager::SpawnGoblinRapace(const glm::vec3& position)
{
    Termina::Actor* goblinRapace = Instantiate(p_GoblinRapace);
    if (goblinRapace && goblinRapace->HasComponent<Termina::Transform>()) {
        goblinRapace->GetComponent<Termina::Transform>().SetPosition(position);
        allEnemies.push_back(goblinRapace);
        TN_INFO("Goblin Rapace spawned at (%.2f, %.2f, %.2f)", position.x, position.y, position.z);
    }
}

void EntityManager::SpawnHobgoblin(const glm::vec3& position)
{
    Termina::Actor* hobgoblin = Instantiate(p_HobGoblin);
    if (hobgoblin && hobgoblin->HasComponent<Termina::Transform>()) {
        hobgoblin->GetComponent<Termina::Transform>().SetPosition(position);
        allEnemies.push_back(hobgoblin);
        TN_INFO("Hobgoblin spawned at (%.2f, %.2f, %.2f)", position.x, position.y, position.z);
    }
}

void EntityManager::SpawnMagicien(const glm::vec3& position)
{
    Termina::Actor* magicien = Instantiate(p_Magicien);
    if (magicien && magicien->HasComponent<Termina::Transform>()) {
        magicien->GetComponent<Termina::Transform>().SetPosition(position);
        allEnemies.push_back(magicien);
        TN_INFO("Magicien spawned at (%.2f, %.2f, %.2f)", position.x, position.y, position.z);
    }
}

void EntityManager::SpawnShamanATK(const glm::vec3& position)
{
    Termina::Actor* shamanATK = Instantiate(p_ShamanATK);
    if (shamanATK && shamanATK->HasComponent<Termina::Transform>()) {
        shamanATK->GetComponent<Termina::Transform>().SetPosition(position);
        allEnemies.push_back(shamanATK);
        TN_INFO("Shaman ATK spawned at (%.2f, %.2f, %.2f)", position.x, position.y, position.z);
    }
}

void EntityManager::SpawnShamanSPD(const glm::vec3& position)
{
    Termina::Actor* shamanSPD = Instantiate(p_ShamanSPD);
    if (shamanSPD && shamanSPD->HasComponent<Termina::Transform>()) {
        shamanSPD->GetComponent<Termina::Transform>().SetPosition(position);
        allEnemies.push_back(shamanSPD);
        TN_INFO("Shaman SPD spawned at (%.2f, %.2f, %.2f)", position.x, position.y, position.z);
    }
}

void EntityManager::SpawnShamanRES(const glm::vec3& position)
{
    Termina::Actor* shamanRES = Instantiate(p_ShamanRES);
    if (shamanRES && shamanRES->HasComponent<Termina::Transform>()) {
        shamanRES->GetComponent<Termina::Transform>().SetPosition(position);
        allEnemies.push_back(shamanRES);
        TN_INFO("Shaman RES spawned at (%.2f, %.2f, %.2f)", position.x, position.y, position.z);
    }
}
