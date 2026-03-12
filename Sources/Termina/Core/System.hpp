#pragma once

#include "Common.hpp"

#include <string>

namespace Termina {
    /// Flags for specifying update behavior of a system.
    enum class UpdateFlags : uint32
    {
        /// Update the system during the editor.
        UpdateDuringEditor = BIT(0),
        /// Update the system during physics updates in the editor.
        PhysicsUpdateDuringEditor = BIT(1),
        /// Update the system during render updates in the editor.
        RenderUpdateDuringEditor = BIT(2),
    };
    ENUM_CLASS_FLAG_OPERATORS(UpdateFlags)

    /// Interface for a system that can be updated by the engine.
    class ISystem
    {
    public:
        virtual ~ISystem() = default;

        virtual void PreUpdate(float deltaTime) {};
        virtual void Update(float deltaTime) {};
        virtual void PostUpdate(float deltaTime) {};
        virtual void PrePhysics(float deltaTime) {};
        virtual void Physics(float deltaTime) {};
        virtual void PostPhysics(float deltaTime) {};
        virtual void PreRender(float deltaTime) {};
        virtual void Render(float deltaTime) {};
        virtual void PostRender(float deltaTime) {};

        /// Register any components that the system needs.
        virtual void RegisterComponents() {};

        /// Unregister any components that the system needs.
        virtual void UnregisterComponents() {};

        /// Get the update flags for the system.
        virtual UpdateFlags GetUpdateFlags() const = 0;

        /// Get the name of the system.
        virtual std::string GetName() const = 0;

        /// Get the priority of the system.
        virtual int GetPriority() const = 0;
    };
}
