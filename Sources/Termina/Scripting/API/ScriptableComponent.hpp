#pragma once

#include <Termina/World/World.hpp>

namespace TerminaScript {
    /// Represents a scriptable component that can be attached to an actor.
    class ScriptableComponent : public Termina::Component
    {
    public:
        ScriptableComponent() = default;
        ScriptableComponent(Termina::Actor* owner) : Termina::Component(owner) {}

        /// Called when the component is first created.
        virtual void Awake() {}
        /// Called when the scene starts.
        virtual void Start() {}
        /// Called when the scene stops.
        virtual void Stop() {}
        /// Called before the component is updated.
        virtual void PreUpdate(float deltaTime) {}
        /// Called during the component's update.
        virtual void Update(float deltaTime) {}
        /// Called after the component is updated.
        virtual void PostUpdate(float deltaTime) {}
        /// Called before the component's physics are updated.
        virtual void PrePhysics(float deltaTime) {}
        /// Called during the component's physics update.
        virtual void Physics(float deltaTime) {}
        /// Called after the component's physics are updated.
        virtual void PostPhysics(float deltaTime) {}

        // Override to persist fields across hot-reloads and world saves.
        virtual void Serialize(nlohmann::json& out) const override {}
        virtual void Deserialize(const nlohmann::json& in) override {}

        // TODO: TriggerEnter, TriggerStay, TriggerExit, CollisionEnter, CollisionStay, CollisionExit

        Termina::UpdateFlags GetUpdateFlags() const override { return (Termina::UpdateFlags)0; }

    private:
        void OnInit()                override;
        void OnPlay()                override { Start(); }
        void OnStop()                override { Stop(); }
        void OnPreUpdate(float dt)   override { PreUpdate(dt); }
        void OnUpdate(float dt)      override { Update(dt); }
        void OnPostUpdate(float dt)  override { PostUpdate(dt); }
        void OnPrePhysics(float dt)  override { PrePhysics(dt); }
        void OnPhysics(float dt)     override { Physics(dt); }
        void OnPostPhysics(float dt) override { PostPhysics(dt); }

    protected:
        friend class ScriptManager;

        /// Instantiates a new actor.
        void Instantiate(Termina::Actor* actor);
        /// Destroys the given actor.
        void Destroy(Termina::Actor* actor);

        /// The transform component of this actor.
        Termina::Transform* m_Transform;
        /// The name of this actor.
        std::string m_Name;

        template<typename T>
        T* AddComponent()
        {
            return m_Owner->AddComponent<T>();
        }

        template<typename T>
        T* GetComponent()
        {
            return m_Owner->GetComponent<T>();
        }

        template<typename T>
        T* RemoveComponent()
        {
            return m_Owner->RemoveComponent<T>();
        }

        template<typename T>
        bool HasComponent()
        {
            return m_Owner->HasComponent<T>();
        }

        std::vector<Termina::Actor*> GetChildren()
        {
            return m_Owner->GetChildren();
        }
    };
}
