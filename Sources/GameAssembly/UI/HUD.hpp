#pragma once

#include <Termina/Scripting/API/ScriptingAPI.hpp>

using namespace TerminaScript;

class HUDComponent : public ScriptableComponent
{
public:
    HUDComponent() = default;
    HUDComponent(Termina::Actor* owner)
        : ScriptableComponent(owner) {
    }

    void OnRender(float dt) override;

private:
    Termina::Actor* m_PlayerActor = nullptr;
};
