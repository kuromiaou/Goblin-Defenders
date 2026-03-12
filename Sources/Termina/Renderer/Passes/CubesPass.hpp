#pragma once

#include <Termina/Renderer/RenderPass.hpp>

namespace Termina {
    /// Renders cubes using the given render pass execute info.
    class CubesPass : public RenderPass
    {
    public:
        CubesPass();
        ~CubesPass() override;

        void Resize(int32 width, int32 height) override;
        void Execute(RenderPassExecuteInfo& Info) override;
    private:
        RendererTexture* m_ColorTexture;
        RendererTexture* m_DepthTexture;
    };
}
