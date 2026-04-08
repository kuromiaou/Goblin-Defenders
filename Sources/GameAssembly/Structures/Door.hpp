#pragma once

#include <Termina/Scripting/API/ScriptingAPI.hpp>

class Door : public TerminaScript::ScriptableComponent {
public:
    Door() = default;
    Door(Termina::Actor* owner) : TerminaScript::ScriptableComponent(owner),
        is_active(true), door_id(0)
    {
    }

    void Update(float dt) override;
    void Start() override;

private:
    int   door_id;
    bool  is_active;

public:
    // --- Getters ---
    int   getDoorId()     const { return door_id; }
    bool  isActive()      const { return is_active; }

    // Récupère la position depuis le Transform
    glm::vec3 getPosition() const { return m_Transform->GetLocalPosition(); }

    // --- Setters ---
    void setActive(bool v) { is_active = v; }
    void setDoorId(int id) { door_id = id; }
};