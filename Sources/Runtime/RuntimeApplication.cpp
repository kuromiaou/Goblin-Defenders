#include "RuntimeApplication.hpp"
#include "Termina/RHI/RenderContext.hpp"
#include "Termina/RHI/RenderPipeline.hpp"
#include "Termina/RHI/Texture.hpp"

#include <Termina/Renderer/Renderer.hpp>
#include <Termina/Shader/ShaderManager.hpp>

RuntimeApplication::RuntimeApplication()
    : Application("Runtime")
{
    Termina::RendererSystem* renderer = m_SystemManager.AddSystem<Termina::RendererSystem>(m_Window);
    Termina::ShaderManager* shaderManager = m_SystemManager.AddSystem<Termina::ShaderManager>();

    Termina::RenderPipelineDesc rpDesc = Termina::RenderPipelineDesc().AddColorAttachmentFormat(Termina::TextureFormat::BGRA8_UNORM)
                                                                      .SetName("Triangle Pipeline");
    shaderManager->GetShaderServer().WatchPipeline("Assets/Shaders/Triangle.hlsl", rpDesc, Termina::PipelineType::Graphics);

    renderer->RegisterRenderCallback([this, shaderManager](Termina::RendererDevice* device, Termina::RendererSurface* surface, float deltaTime){
        auto context = surface->GetContext();

        Termina::RenderEncoderInfo rei = Termina::RenderEncoderInfo().AddColorAttachment(surface->GetCurrentTextureView(), true, glm::vec4(0.2f, 0.8f, 0.5f, 1.0f))
                                                   .SetName("Clear Color")
                                                   .SetDimensions(m_Window->GetWidth(), m_Window->GetHeight());
        Termina::RenderEncoder* re = context->CreateRenderEncoder(rei);
        re->SetPipeline(shaderManager->GetShaderServer().GetPipeline("Assets/Shaders/Triangle.hlsl"));
        re->SetViewport(0.0f, 0.0f, static_cast<float>(m_Window->GetWidth()), static_cast<float>(m_Window->GetHeight()));
        re->SetScissorRect(0.0f, 0.0f, static_cast<float>(m_Window->GetWidth()), static_cast<float>(m_Window->GetHeight()));
        re->Draw(3, 1, 0, 0);
        re->End();
    });
}
