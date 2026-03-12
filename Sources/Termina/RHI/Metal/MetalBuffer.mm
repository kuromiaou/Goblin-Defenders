#include "MetalBuffer.hpp"
#include "MetalDevice.hpp"

namespace Termina {
    MetalBuffer::MetalBuffer(MetalDevice* device, const BufferDesc& desc)
        : m_Parent(device)
    {
        m_Desc = desc;

        m_Buffer = [device->GetDevice() newBufferWithLength:desc.Size options:MTLResourceStorageModeShared];
        [device->GetResidencySet() addAllocation:m_Buffer];
        [device->GetResidencySet() commit];
    }

    MetalBuffer::~MetalBuffer()
    {
        [m_Parent->GetResidencySet() removeAllocation:m_Buffer];
    }

    void MetalBuffer::SetName(const std::string& name)
    {
        [m_Buffer setLabel:[NSString stringWithUTF8String:name.c_str()]];
    }

    void* MetalBuffer::Map()
    {
        return [m_Buffer contents];
    }

    void MetalBuffer::Unmap()
    {
    }

    uint64 MetalBuffer::GetGPUAddress() const
    {
        return m_Buffer.gpuAddress;
    }
} // namespace Termina
