#pragma once

#include "Common.hpp"

#include <string>

namespace Termina {
    enum class UpdateFlags : uint32
    {
        PreUpdate = BIT(0),
        Update = BIT(1),
        PostUpdate = BIT(2),
        UpdateDuringEditor = BIT(3),

        PrePhysics = BIT(4),
        Physics = BIT(5),
        PostPhysics = BIT(6),
        PhysicsUpdateDuringEditor = BIT(7),

        PreRender = BIT(8),
        Render = BIT(9),
        PostRender = BIT(10),
        RenderUpdateDuringEditor = BIT(11),
    };

    class ISystem
    {
    public:
        virtual ~ISystem() = default;

        virtual void PreUpdate(float deltaTime) = 0;
        virtual void Update(float deltaTime) = 0;
        virtual void PostUpdate(float deltaTime) = 0;
        virtual void PrePhysics(float deltaTime) = 0;
        virtual void Physics(float deltaTime) = 0;
        virtual void PostPhysics(float deltaTime) = 0;
        virtual void PreRender(float deltaTime) = 0;
        virtual void Render(float deltaTime) = 0;
        virtual void PostRender(float deltaTime) = 0;

        virtual UpdateFlags GetUpdateFlags() const = 0;
        virtual std::string GetName() const = 0;
        virtual int GetPriority() const = 0;
    };
}
