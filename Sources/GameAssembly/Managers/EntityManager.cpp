#include "EntityManager.hpp"

void EntityManager::Start()
{
    m_Player = m_Owner->GetParentWorld()->GetActorByName("Player");
}

void EntityManager::Update(float deltaTime)
{
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
    }

    // Spawner les Goblin Rapace
    int goblinRapaceCount = GoblinRapaceWave[waveIndex];
    for (int i = 0; i < goblinRapaceCount; i++)
    {
        SpawnGoblinRapace();
    }

    // Spawner les Hobgoblins
    int hobgoblinCount = HobGoblinWave[waveIndex];
    for (int i = 0; i < hobgoblinCount; i++)
    {
        SpawnHobgoblin();
    }

    // Spawner les Magiciens
    int magicienCount = MagicienWave[waveIndex];
    for (int i = 0; i < magicienCount; i++)
    {
        SpawnMagicien();
    }

    // Spawner les Shamans ATK
    int shamanATKCount = ShamanATKWave[waveIndex];
    for (int i = 0; i < shamanATKCount; i++)
    {
        SpawnShamanATK();
    }

    // Spawner les Shamans SPD
    int shamanSPDCount = ShamanSPDWave[waveIndex];
    for (int i = 0; i < shamanSPDCount; i++)
    {
        SpawnShamanSPD();
    }

    // Spawner les Shamans RES
    int shamanRESCount = ShamanRESWave[waveIndex];
    for (int i = 0; i < shamanRESCount; i++)
    {
        SpawnShamanRES();
    }
}

void EntityManager::SpawnGoblin()
{
    /*allEnemies.push_back(goblin);*/
}

void EntityManager::SpawnGoblinRapace()
{
    //allEnemies.push_back(goblinRapace);
}

void EntityManager::SpawnHobgoblin()
{

    //allEnemies.push_back(hobgoblin);
}

void EntityManager::SpawnMagicien()
{
    //allEnemies.push_back(magicien);
}

void EntityManager::SpawnShamanATK()
{
    //allEnemies.push_back(shaman);
}

void EntityManager::SpawnShamanSPD()
{
    //allEnemies.push_back(shaman);
}

void EntityManager::SpawnShamanRES()
{
    //allEnemies.push_back(shaman);
}