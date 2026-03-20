#pragma once

#include <Termina/Scripting/API/ScriptingAPI.hpp>

// ============================================================
// DOOR — Point de spawn des ennemis (position fixe)
// ============================================================
class Door : public TerminaScript::ScriptableComponent {
public:
    Door() = default;
    Door(Termina::Actor* owner, int id, float x, float y, float z) : TerminaScript::ScriptableComponent(owner),
        door_id(id),
        pos_x(x), pos_y(y), pos_z(z),
        is_active(true) 
    {}

        void Update(float dt)override;
        void Start()override;

private:
    int   door_id;
    float pos_x;
    float pos_y;
    float pos_z;
    bool  is_active;

public:
    // --- Getters ---
    int   getDoorId()  const { return door_id; }
    float getPosX()    const { return pos_x; }
    float getPosY()    const { return pos_y; }
    float getPosZ()    const { return pos_z; }
    bool  isActive()   const { return is_active; }

    // --- Setters ---
    void setActive(bool v) { is_active = v; }
    void setPosition(float x, float y, float z) {
        pos_x = x; pos_y = y; pos_z = z;
    }
};
