#include "Metal_ShaderConverter.hpp"
#include <MetalShaderConverter/metal_irconverter.h>

#include <Termina/Core/Logger.hpp>

namespace Termina {
    IRRootSignature* MetalShaderConverter::s_RootSignature;

    void MetalShaderConverter::Initialize()
    {
        IRRootParameter1 rootSigParams[2];
        rootSigParams[0] = {
            .ParameterType = IRRootParameterType32BitConstants,
            .Constants = {
                .ShaderRegister = 0,
                .RegisterSpace = 0,
                .Num32BitValues = 128 / sizeof(uint32_t)
            },
            .ShaderVisibility = IRShaderVisibilityAll
        };
        rootSigParams[1] = {
            .ParameterType = IRRootParameterType32BitConstants,
            .Constants = {
                .ShaderRegister = 1,
                .RegisterSpace = 0,
                .Num32BitValues = 1
            },
            .ShaderVisibility = IRShaderVisibilityAll
        };

        IRVersionedRootSignatureDescriptor rootSignature = {};
        rootSignature.version = IRRootSignatureVersion_1_1;
        rootSignature.desc_1_1.Flags = IRRootSignatureFlags(IRRootSignatureFlagSamplerHeapDirectlyIndexed | IRRootSignatureFlagCBVSRVUAVHeapDirectlyIndexed);
        rootSignature.desc_1_1.pParameters = rootSigParams;
        rootSignature.desc_1_1.NumParameters = 2;

        IRError* pRootSigError = nullptr;
        s_RootSignature = IRRootSignatureCreateFromDescriptor(&rootSignature, &pRootSigError);
        if (pRootSigError) {
            auto errorCode = IRErrorGetCode(pRootSigError);

            TN_ERROR("Failed to create root signature with code %u", errorCode);
            IRErrorDestroy(pRootSigError);
        }
    }

    void MetalShaderConverter::Shutdown()
    {
        IRRootSignatureDestroy(s_RootSignature);
    }

    std::vector<uint8> MetalShaderConverter::Convert(std::vector<uint8> dxil, std::string entryPoint)
    {
        auto module = IRObjectCreateFromDXIL(dxil.data(), dxil.size(), IRBytecodeOwnershipNone);

        IRCompiler* compiler = IRCompilerCreate();
        IRCompilerSetEntryPointName(compiler, entryPoint.c_str());
        IRCompilerSetMinimumDeploymentTarget(compiler, IROperatingSystem_macOS, "15.0");
        IRCompilerSetGlobalRootSignature(compiler, s_RootSignature);

        IRError* compileError = nullptr;
        auto metalIR = IRCompilerAllocCompileAndLink(compiler, entryPoint.c_str(), module, &compileError);
        if (compileError) {
            auto errorCode = IRErrorGetCode(compileError);

            TN_ERROR("Metal IR generation failed with code %u", errorCode);
            IRErrorDestroy(compileError);
        }

        IRMetalLibBinary* pMetallib = IRMetalLibBinaryCreate();
        IRObjectGetMetalLibBinary(metalIR, IRObjectGetMetalIRShaderStage(metalIR), pMetallib);
        dxil.clear();
        dxil.resize(IRMetalLibGetBytecodeSize(pMetallib));
        IRMetalLibGetBytecode(pMetallib, dxil.data());

        IRMetalLibBinaryDestroy(pMetallib);
        IRObjectDestroy(module);
        IRObjectDestroy(metalIR);
        IRCompilerDestroy(compiler);

        return dxil;
    }
}
