#pragma once

#include <Termina/World/Actor.hpp>
#include <Termina/World/World.hpp>
#include <Termina/Renderer/Components/MeshComponent.hpp>

#include <vector>

inline void CollectDescendantsPostOrder(Termina::Actor* actor, std::vector<Termina::Actor*>& out)
{
    if (!actor) return;
    for (Termina::Actor* child : actor->GetChildren()) {
        CollectDescendantsPostOrder(child, out);
        out.push_back(child);
    }
}

inline Termina::Actor* GetHierarchyRoot(Termina::Actor* actor)
{
    if (!actor) return nullptr;
    while (actor->GetParent())
        actor = actor->GetParent();
    return actor;
}

inline void DestroyActorHierarchy(Termina::Actor* root)
{
    Termina::Actor* hierarchyRoot = GetHierarchyRoot(root);
    if (!hierarchyRoot) return;

    Termina::World* world = hierarchyRoot->GetParentWorld();
    if (!world) return;

    std::vector<Termina::Actor*> descendants;
    CollectDescendantsPostOrder(hierarchyRoot, descendants);
    for (Termina::Actor* child : descendants) {
        if (child)
            world->DestroyActor(child);
    }

    world->DestroyActor(hierarchyRoot);
}

/// Removes the MeshComponent from an actor and all its descendants (recursively),
/// leaving the rest of the hierarchy intact.
inline void DestroyMeshHierarchy(Termina::Actor* root)
{
    Termina::Actor* hierarchyRoot = GetHierarchyRoot(root);
    if (!hierarchyRoot) return;

    // Remove mesh from the root itself
    if (hierarchyRoot->HasComponent<Termina::MeshComponent>())
        hierarchyRoot->RemoveComponent<Termina::MeshComponent>();

    // Remove mesh from every descendant
    std::vector<Termina::Actor*> descendants;
    CollectDescendantsPostOrder(hierarchyRoot, descendants);
    for (Termina::Actor* child : descendants) {
        if (child && child->HasComponent<Termina::MeshComponent>())
            child->RemoveComponent<Termina::MeshComponent>();
    }
}
