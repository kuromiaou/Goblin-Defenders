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

    void Serialize(nlohmann::json& out) const override;
    void Deserialize(const nlohmann::json& in) override;

    int getOrder() const;
    glm::vec3 getPosition() const { return m_Transform->GetPosition(); }

    // Getter statique pour accéder à tous les checkpoints
    static std::vector<Checkpoint*>& GetAllCheckpoints() {
        static std::vector<Checkpoint*> checkpoints;
        return checkpoints;
    }

    // AJOUT: Méthode pour vider la liste
    static void ClearAllCheckpoints() {
        GetAllCheckpoints().clear();
    }

private:
    int m_Order = 0;
    int m_CheckPointMax = 999;
};