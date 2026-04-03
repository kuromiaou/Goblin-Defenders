#pragma once
#include <Termina/Scripting/API/ScriptingAPI.hpp>

class Checkpoint : public TerminaScript::ScriptableComponent
{
public:
    Checkpoint() = default;
    Checkpoint(Termina::Actor* owner) : TerminaScript::ScriptableComponent(owner) {}

    void Start() override;
    void Update(float Deltatime) override;
    void Inspect() override;

    int getOrder() const { return m_Order; }
    glm::vec3 getPosition() const { return m_Transform->GetPosition(); }

    // Getter statique pour accéder à tous les checkpoints
    static std::vector<Checkpoint*>& GetAllCheckpoints() {
        static std::vector<Checkpoint*> checkpoints;
        return checkpoints;
    }

private:
    int m_Order = 0;
    int m_CheckPointMax = 999;
};