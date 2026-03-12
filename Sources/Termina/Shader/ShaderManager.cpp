#include "ShaderManager.hpp"

#include <Termina/Core/Application.hpp>
#include <Termina/Renderer/Renderer.hpp>

#if defined(TRMN_MACOS)
    #include "Metal/Metal_ShaderConverter.hpp"
#endif

namespace Termina {
    ShaderManager::ShaderManager()
    {
        std::string libraryPath = "dxcompiler.dll";
    #if defined(TRMN_LINUX)
        libraryPath = "libdxcompiler.so";
    #elif defined(TRMN_MACOS)
        libraryPath = "libdxcompiler.dylib";
    #endif

        m_Library.Load(libraryPath.c_str());
        m_ShaderServer.ConnectDevice(Application::GetSystem<RendererSystem>()->GetDevice());

    #if defined(TRMN_MACOS)
        MetalShaderConverter::Initialize();
    #endif
    }

    ShaderManager::~ShaderManager()
    {
        m_ShaderServer.Clear();
    }

    void ShaderManager::PostRender(float dt)
    {
    #if !defined(TRMN_RETAIL)
        m_ShaderServer.ReloadModifiedPipelines();
    #endif
    }
}
