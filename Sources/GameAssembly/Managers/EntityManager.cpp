#include "EntityManager.hpp"

void EntityManager::Start()
{
    m_Player = m_Owner->GetParentWorld()->GetActorByName("Player");

    // Chercher toutes les Doors dans la scène en itérant les acteurs
    const auto& actors = m_Owner->GetParentWorld()->GetActors();
    for (const auto& actorPtr : actors)
    {
        Termina::Actor* actor = actorPtr.get();
        if (actor && actor->HasComponent<Door>())
        {
            Door* door = &actor->GetComponent<Door>();
            RegisterDoor(door);
        }
    }
}

void EntityManager::RegisterDoor(Door* door)
{
    if (!door) return;
    all_doors.push_back(door);
    int door_id = all_doors.size() - 1;
    door->setDoorId(door_id);
}

glm::vec3 EntityManager::getRandomDoorPosition() const
{
    if (all_doors.empty())
        return glm::vec3(0.0f, 0.0f, 0.0f);

    int randomIndex = rand() % all_doors.size();
    return all_doors[randomIndex]->getPosition();
}

void EntityManager::Update(float deltaTime)
{
}

void EntityManager::SpawnWave(int waveIndex)
{
    if (waveIndex < 0 || waveIndex >= static_cast<int>(GoblinWave.size()))
        return;

    int goblinCount = GoblinWave[waveIndex];
    for (int i = 0; i < goblinCount; i++)
        SpawnGoblin(getRandomDoorPosition());

    int goblinRapaceCount = GoblinRapaceWave[waveIndex];
    for (int i = 0; i < goblinRapaceCount; i++)
        SpawnGoblinRapace(getRandomDoorPosition());

    int hobgoblinCount = HobGoblinWave[waveIndex];
    for (int i = 0; i < hobgoblinCount; i++)
        SpawnHobgoblin(getRandomDoorPosition());

    int magicienCount = MagicienWave[waveIndex];
    for (int i = 0; i < magicienCount; i++)
        SpawnMagicien(getRandomDoorPosition());

    int shamanATKCount = ShamanATKWave[waveIndex];
    for (int i = 0; i < shamanATKCount; i++)
        SpawnShamanATK(getRandomDoorPosition());

    int shamanSPDCount = ShamanSPDWave[waveIndex];
    for (int i = 0; i < shamanSPDCount; i++)
        SpawnShamanSPD(getRandomDoorPosition());

    int shamanRESCount = ShamanRESWave[waveIndex];
    for (int i = 0; i < shamanRESCount; i++)
        SpawnShamanRES(getRandomDoorPosition());
}

void EntityManager::SpawnGoblin(const glm::vec3& spawnPos)
{
    Termina::Actor* goblin = m_Owner->GetParentWorld()->SpawnActor();
    goblin->SetName("Goblin");
    goblin->GetComponent<Termina::Transform>().SetPosition(spawnPos);
    goblin->AddComponent<Goblin>();
    allEnemies.push_back(std::make_shared<Goblin>(goblin));
}

void EntityManager::SpawnGoblinRapace(const glm::vec3& spawnPos)
{
    Termina::Actor* goblinRapace = m_Owner->GetParentWorld()->SpawnActor();
    goblinRapace->SetName("Goblin Rapace");
    goblinRapace->GetComponent<Termina::Transform>().SetPosition(spawnPos);
    goblinRapace->AddComponent<GoblinRapace>();
    allEnemies.push_back(std::make_shared<GoblinRapace>(goblinRapace));
}

void EntityManager::SpawnHobgoblin(const glm::vec3& spawnPos)
{
    Termina::Actor* hobgoblin = m_Owner->GetParentWorld()->SpawnActor();
    hobgoblin->SetName("Hobgoblin");
    hobgoblin->GetComponent<Termina::Transform>().SetPosition(spawnPos);
    hobgoblin->AddComponent<Hobgoblin>();
    allEnemies.push_back(std::make_shared<Hobgoblin>(hobgoblin));
}

void EntityManager::SpawnMagicien(const glm::vec3& spawnPos)
{
    Termina::Actor* magicien = m_Owner->GetParentWorld()->SpawnActor();
    magicien->SetName("Magicien");
    magicien->GetComponent<Termina::Transform>().SetPosition(spawnPos);
    magicien->AddComponent<Magicien>();
    allEnemies.push_back(std::make_shared<Magicien>(magicien));
}

void EntityManager::SpawnShamanATK(const glm::vec3& spawnPos)
{
    Termina::Actor* shaman = m_Owner->GetParentWorld()->SpawnActor();
    shaman->SetName("Shaman ATK");
    shaman->GetComponent<Termina::Transform>().SetPosition(spawnPos);
    shaman->AddComponent<Shaman>();
    allEnemies.push_back(std::make_shared<Shaman>(shaman));
}

void EntityManager::SpawnShamanSPD(const glm::vec3& spawnPos)
{
    Termina::Actor* shaman = m_Owner->GetParentWorld()->SpawnActor();
    shaman->SetName("Shaman SPD");
    shaman->GetComponent<Termina::Transform>().SetPosition(spawnPos);
    shaman->AddComponent<Shaman>();
    allEnemies.push_back(std::make_shared<Shaman>(shaman));
}

void EntityManager::SpawnShamanRES(const glm::vec3& spawnPos)
{
    Termina::Actor* shaman = m_Owner->GetParentWorld()->SpawnActor();
    shaman->SetName("Shaman RES");
    shaman->GetComponent<Termina::Transform>().SetPosition(spawnPos);
    shaman->AddComponent<Shaman>();
    allEnemies.push_back(std::make_shared<Shaman>(shaman));
}