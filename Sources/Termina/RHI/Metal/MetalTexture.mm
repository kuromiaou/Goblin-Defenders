#include "MetalTexture.hpp"
#include "MetalDevice.hpp"

namespace Termina {
    MetalTexture::MetalTexture(MetalDevice* device, const TextureDesc& desc)
        : m_Parent(device)
    {
        m_Desc = desc;

        MTLTextureDescriptor* descriptor = [MTLTextureDescriptor new];
        descriptor.width = desc.Width;
        descriptor.height = desc.Height;
        descriptor.depth = desc.Depth;
        descriptor.arrayLength = desc.ArrayLayers;
        descriptor.mipmapLevelCount = desc.MipLevels;
        descriptor.pixelFormat = ConvertTextureFormatToMetal(desc.Format);
        descriptor.usage = ConvertTextureUsageToMetal(desc.Usage);
        descriptor.resourceOptions = MTLResourceStorageModePrivate;
        descriptor.textureType = ConvertTextureTypeToMetal(desc);

        m_Texture = [device->GetDevice() newTextureWithDescriptor:descriptor];
        [device->GetResidencySet() addAllocation:m_Texture];
        [device->GetResidencySet() commit];
    }

    MetalTexture::~MetalTexture()
    {
        if (m_ShouldDestroy) {
            [m_Parent->GetResidencySet() removeAllocation:m_Texture];
        }
    }

    void MetalTexture::SetName(const std::string& name)
    {
        m_Texture.label = [NSString stringWithUTF8String:name.c_str()];
    }
} // namespace Termina
