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
    if (waveIndex < 0 || waveIndex >= GoblinWave.size())
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

    // Spawner les Shamans
    int shamanATKCount = ShamanATKWave[waveIndex];
    for (int i = 0; i < shamanATKCount; i++)
    {
        SpawnShamanATK();
    }

    int shamanSPDCount = ShamanSPDWave[waveIndex];
    for (int i = 0; i < shamanSPDCount; i++)
    {
        SpawnShamanSPD();
    }

    int shamanRESCount = ShamanRESWave[waveIndex];
    for (int i = 0; i < shamanRESCount; i++)
    {
        SpawnShamanRES();
    }
}

void EntityManager::SpawnGoblin()
{
    Termina::Actor* goblin = m_Owner->GetParentWorld()->SpawnActor();
    goblin->SetName("Goblin");
    goblin->AddComponent<Goblin>();
    allEnemies.push_back(std::make_shared<Goblin>(goblin));
}

void EntityManager::SpawnGoblinRapace()
{
    Termina::Actor* goblinRapace = m_Owner->GetParentWorld()->SpawnActor();
    goblinRapace->SetName("Goblin Rapace");
    goblinRapace->AddComponent<GoblinRapace>();
    allEnemies.push_back(std::make_shared<GoblinRapace>(goblinRapace));
}

void EntityManager::SpawnHobgoblin()
{
    Termina::Actor* hobgoblin = m_Owner->GetParentWorld()->SpawnActor();
    hobgoblin->SetName("Hobgoblin");
    hobgoblin->AddComponent<Hobgoblin>();
    allEnemies.push_back(std::make_shared<Hobgoblin>(hobgoblin));
}

void EntityManager::SpawnMagicien()
{
    Termina::Actor* magicien = m_Owner->GetParentWorld()->SpawnActor();
    magicien->SetName("Magicien");
    magicien->AddComponent<Magicien>();
    allEnemies.push_back(std::make_shared<Magicien>(magicien));
}

void EntityManager::SpawnShamanATK()
{
    Termina::Actor* shaman = m_Owner->GetParentWorld()->SpawnActor();
    shaman->SetName("Shaman ATK");
    shaman->AddComponent<Shaman>();
    allEnemies.push_back(std::make_shared<Shaman>(shaman));
}

void EntityManager::SpawnShamanSPD()
{
    Termina::Actor* shaman = m_Owner->GetParentWorld()->SpawnActor();
    shaman->SetName("Shaman SPD");
    shaman->AddComponent<Shaman>();
    allEnemies.push_back(std::make_shared<Shaman>(shaman));
}

void EntityManager::SpawnShamanRES()
{
    Termina::Actor* shaman = m_Owner->GetParentWorld()->SpawnActor();
    shaman->SetName("Shaman RES");
    shaman->AddComponent<Shaman>();
    allEnemies.push_back(std::make_shared<Shaman>(shaman));
}