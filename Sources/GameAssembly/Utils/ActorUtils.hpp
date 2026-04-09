#pragma once

#include <Termina/World/Actor.hpp>
#include <Termina/World/World.hpp>

#include <vector>

inline void CollectDescendantsPostOrder(Termina::Actor* actor, std::vector<Termina::Actor*>& out)
{
    if (!actor) return;
    for (Termina::Actor* child : actor->GetChildren()) {
        CollectDescendantsPostOrder(child, out);
        out.push_back(child);
    }
}

inline void DestroyActorHierarchy(Termina::Actor* root)
{
    if (!root) return;
    Termina::World* world = root->GetParentWorld();
    if (!world) return;

    std::vector<Termina::Actor*> descendants;
    CollectDescendantsPostOrder(root, descendants);
    for (Termina::Actor* child : descendants) {
        if (child)
            world->DestroyActor(child);
    }

    world->DestroyActor(root);
}
