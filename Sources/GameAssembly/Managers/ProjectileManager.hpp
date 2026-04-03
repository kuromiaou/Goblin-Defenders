#pragma once
#include <Termina/Scripting/API/ScriptingAPI.hpp>

using namespace TerminaScript;

class ProjectileManager : public TerminaScript::ScriptableComponent
{
public:
    ProjectileManager() = default;
    ProjectileManager(Termina::Actor* owner) : TerminaScript::ScriptableComponent(owner) {}

    void Start()  override;
    void Update(float deltaTime) override;
};
