#include "EntityManager.hpp"

void EntityManager::Start()
{
    state = WaveState::IN_PROGRESS;
    if(p_Goblin       = TerminaScript::Prefab("Assets/Prefabs/Enemies/goblin.trp"))
        std::cout << "goblin loaded";
    p_GoblinRapace = TerminaScript::Prefab("Assets/Prefabs/Enemies/thief.trp");
    p_HobGoblin    = TerminaScript::Prefab("Assets/Prefabs/Enemies/hobgoblin.trp");
    p_Magicien     = TerminaScript::Prefab("Assets/Prefabs/Enemies/mage.trp");
    p_ShamanATK    = TerminaScript::Prefab("Assets/Prefabs/Enemies/shaman/shamanATK.trp");
    p_ShamanSPD    = TerminaScript::Prefab("Assets/Prefabs/Enemies/shaman/shamanSPD.trp");
    p_ShamanRES    = TerminaScript::Prefab("Assets/Prefabs/Enemies/shaman/shamanRES.trp");
}

void EntityManager::Update(float deltaTime)
{
    switch (state) {
    case WaveState::WAITING:

        break;

    case WaveState::IN_PROGRESS:
        if (!is_wave_spawned) {
            SpawnWave(current_wave);
            is_wave_spawned = true;
        }

        if (allEnemies.empty()) {
            state = WaveState::COMPLETED;
        }

        break;

    case WaveState::COMPLETED:

        is_wave_spawned = false;
        current_wave++;

        state = WaveState::WAITING;

        break;

    }
}

void EntityManager::SpawnWave(int waveIndex)
{
    // Vérifier que l'index est valide
    if (waveIndex < 0 || waveIndex >= static_cast<int>(GoblinWave.size()))
        return;

    // Spawner les Goblins
    int goblinCount = GoblinWave[waveIndex];
    for (int i = 0; i < goblinCount; i++)
    {
        SpawnGoblin();
        std::cout << "goblin spawned";
    }

    // Spawner les Goblin Rapace
    int goblinRapaceCount = GoblinRapaceWave[waveIndex];
    for (int i = 0; i < goblinRapaceCount; i++)
    {
        SpawnGoblinRapace();
        std::cout << "Goblin Rapace spawned";
    }

    // Spawner les Hobgoblins
    int hobgoblinCount = HobGoblinWave[waveIndex];
    for (int i = 0; i < hobgoblinCount; i++)
    {
        SpawnHobgoblin();
        std::cout << "Hobgoblins spawned";
    }

    // Spawner les Magiciens
    int magicienCount = MagicienWave[waveIndex];
    for (int i = 0; i < magicienCount; i++)
    {
        SpawnMagicien();
        std::cout << "Magiciens spawned";
    }

    // Spawner les Shamans ATK
    int shamanATKCount = ShamanATKWave[waveIndex];
    for (int i = 0; i < shamanATKCount; i++)
    {
        SpawnShamanATK();
        std::cout << "Shamans ATK spawned";
    }

    // Spawner les Shamans SPD
    int shamanSPDCount = ShamanSPDWave[waveIndex];
    for (int i = 0; i < shamanSPDCount; i++)
    {
        SpawnShamanSPD();
        std::cout << "Shamans SPD spawned";
    }

    // Spawner les Shamans RES
    int shamanRESCount = ShamanRESWave[waveIndex];
    for (int i = 0; i < shamanRESCount; i++)
    {
        SpawnShamanRES();
        std::cout << "Shamans RES spawned";
    }
}

void EntityManager::SpawnGoblin()
{
    Termina::Actor* Gob = Instantiate(p_Goblin);
    allEnemies.push_back(Gob);
}

void EntityManager::SpawnGoblinRapace()
{
    Termina::Actor* GobRapace = Instantiate(p_GoblinRapace);
    allEnemies.push_back(GobRapace);
}

void EntityManager::SpawnHobgoblin()
{

    Termina::Actor* HobGob = Instantiate(p_HobGoblin);
    allEnemies.push_back(HobGob);
}

void EntityManager::SpawnMagicien()
{
    Termina::Actor* Mage = Instantiate(p_Magicien);
    allEnemies.push_back(Mage);
}

void EntityManager::SpawnShamanATK()
{
    Termina::Actor* ShamanATK = Instantiate(p_ShamanATK);
    allEnemies.push_back(ShamanATK);
}

void EntityManager::SpawnShamanSPD()
{
    Termina::Actor* ShamanSPD = Instantiate(p_ShamanSPD);
    allEnemies.push_back(ShamanSPD);
}

void EntityManager::SpawnShamanRES()
{
    Termina::Actor* ShamanRES = Instantiate(p_ShamanRES);
    allEnemies.push_back(ShamanRES);
}