#include "EntityManager.hpp"

void EntityManager::Start()
{
    p_Goblin       = TerminaScript::Prefab("Assets/Prefab/goblin.trp");
    p_GoblinRapace = TerminaScript::Prefab("Assets/Prefab/goblinrapace.trp");
    p_HobGoblin    = TerminaScript::Prefab("Assets/Prefab/hobgoblin.trp");
    p_Magicien     = TerminaScript::Prefab("Assets/Prefab/magicien.trp");
    p_ShamanATK    = TerminaScript::Prefab("Assets/Prefab/shamanATK.trp");
    p_ShamanSPD    = TerminaScript::Prefab("Assets/Prefab/shamanSPD.trp");
    p_ShamanRES    = TerminaScript::Prefab("Assets/Prefab/shamanRES.trp");
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