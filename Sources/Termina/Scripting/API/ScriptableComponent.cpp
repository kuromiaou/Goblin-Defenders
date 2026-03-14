#include "ScriptableComponent.hpp"
#include <Termina/Core/Application.hpp>
#include <Termina/World/WorldSystem.hpp>

namespace TerminaScript {
    void ScriptableComponent::OnInit()
    {
        m_Transform = &m_Owner->GetComponent<Termina::Transform>();
        m_Name = m_Owner->GetName();
        Awake();
    }

    Termina::Actor* ScriptableComponent::Instantiate(Termina::Actor* actor)
    {
        return m_Owner->GetParentWorld()->SpawnActorFrom(actor);
    }

    Termina::Actor* ScriptableComponent::Instantiate(const Prefab& prefab)
    {
        if (!prefab.IsValid())
            return nullptr;
        return m_Owner->GetParentWorld()->SpawnActorFromJSON(prefab.Path);
    }

    void ScriptableComponent::Destroy(Termina::Actor* actor)
    {
        m_Owner->GetParentWorld()->DestroyActor(actor);
    }

    void ScriptableComponent::LoadScene(const Scene& scene)
    {
        LoadScene(scene.Path);
    }

    void ScriptableComponent::LoadScene(const std::string& path)
    {
        Termina::WorldSystem* ws = Termina::Application::GetSystem<Termina::WorldSystem>();
        if (ws)
            ws->RequestSceneTransition(path);
    }
}
