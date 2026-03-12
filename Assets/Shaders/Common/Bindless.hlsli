#ifndef BINDLESS_HLSL
#define BINDLESS_HLSL

static const int INVALID_DESCRIPTOR = -1;

// Push constants
#if METAL
    #define PUSH_CONSTANTS(Type) ConstantBuffer<Type> PUSH : register(b0)
#elif VULKAN
    #define PUSH_CONSTANTS(Type) [[vk::push_constant]] ConstantBuffer<Type> PUSH : register(b0)
    #if RAYTRACING
        [[vk::binding(2, 0)]] RaytracingAccelerationStructure ASArray[];
    #endif
#endif

// Get raytracing acceleration structure by index
#if RAYTRACING
#if VULKAN
RaytracingAccelerationStructure GetAccelerationStructure(uint index)
{
    return ASArray[index];
}
#elif METAL
RaytracingAccelerationStructure GetAccelerationStructure(uint index)
{
    return ResourceDescriptorHeap[index];
}
#endif
#endif

// Draw ID
#if METAL
struct DrawIDStruct { uint id; };
cbuffer gDrawID : register(b1) { DrawIDStruct DrawID; }
uint GetDrawID() { return DrawID.id; }
#elif VULKAN
[[vk::ext_builtin_input(4426)]] static const uint gDrawID;
uint GetDrawID() { return gDrawID; }
#endif

#endif
