#pragma once

#include <Termina/Renderer/RenderPass.hpp>

namespace Termina {
    /// Renders a simple triangle using the given render pass execute info.
    class TrianglePass : public RenderPass
    {
    public:
        TrianglePass();
        ~TrianglePass() override = default;

        void Execute(RenderPassExecuteInfo& Info) override;
    };
}
