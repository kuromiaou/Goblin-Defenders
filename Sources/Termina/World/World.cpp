#include "World.hpp"

#include <Termina/Core/Application.hpp>

namespace Termina {
    World::~World()
    {
        Clear();
    }

    void World::DestroyActor(Actor* actor)
    {
        if (!actor) return;

        for (uint64 i = 0; i < m_Actors.size(); ++i) {
            if (m_Actors[i]->GetID() == actor->GetID()) {
                m_Actors.erase(m_Actors.begin() + i);
                break;
            }
        }
    }

    Actor* World::GetActorById(uint64 id)
    {
        for (uint64 i = 0; i < m_Actors.size(); ++i) {
            if (m_Actors[i]->GetID() == id) {
                return m_Actors[i].get();
            }
        }
        return nullptr;
    }

    Actor* World::GetActorByName(const std::string& name)
    {
        for (uint64 i = 0; i < m_Actors.size(); ++i) {
            if (m_Actors[i]->GetName() == name) {
                return m_Actors[i].get();
            }
        }
        return nullptr;
    }

    std::vector<Actor*> World::GetRootActors() const
    {
        std::vector<Actor*> rootActors;
        for (const auto& actorPtr : m_Actors) {
            if (actorPtr->GetParent() == nullptr) {
                rootActors.push_back(actorPtr.get());
            }
        }
        return rootActors;
    }

    void World::OnInit()
    {
        for (auto& actor : m_Actors) actor->OnInit();
    }

    void World::OnShutdown()
    {
        for (auto& actor : m_Actors) actor->OnShutdown();
    }

    void World::OnPlay()
    {
        for (auto& actor : m_Actors) actor->OnPlay();
    }

    void World::OnStop()
    {
        for (auto& actor : m_Actors) actor->OnStop();
    }

    void World::OnPreUpdate(float deltaTime)
    {
        for (auto& actor : m_Actors) actor->OnPreUpdate(deltaTime);
    }

    void World::OnUpdate(float deltaTime)
    {
        for (auto& actor : m_Actors) actor->OnUpdate(deltaTime);
    }

    void World::OnPostUpdate(float deltaTime)
    {
        for (auto& actor : m_Actors) actor->OnPostUpdate(deltaTime);
    }

    void World::OnPrePhysics(float deltaTime)
    {
        for (auto& actor : m_Actors) actor->OnPrePhysics(deltaTime);
    }

    void World::OnPhysics(float deltaTime)
    {
        for (auto& actor : m_Actors) actor->OnPhysics(deltaTime);
    }

    void World::OnPostPhysics(float deltaTime)
    {
        for (auto& actor : m_Actors) actor->OnPostPhysics(deltaTime);
    }

    void World::OnPreRender(float deltaTime)
    {
        for (auto& actor : m_Actors) actor->OnPreRender(deltaTime);
    }

    void World::OnRender(float deltaTime)
    {
        for (auto& actor : m_Actors) actor->OnRender(deltaTime);
    }

    void World::OnPostRender(float deltaTime)
    {
        for (auto& actor : m_Actors) actor->OnPostRender(deltaTime);
    }

    void World::Clear()
    {
        m_Actors.clear();
    }

    void World::LoadFromFile(const std::string& filename)
    {
        m_CurrentPath = filename;
        
        // TODO
    }

    void World::SaveToFile(const std::string& filename)
    {
        if (m_CurrentPath != filename && !filename.empty())
            m_CurrentPath = filename;

        // TODO
    }
}
