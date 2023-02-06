/*
 *  Texture.h
 *
 *  Copyright (c) 2022 by XiangKui Zheng
 *  This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */

#pragma once
#include "graphics/GraphicsDef.h"
#include "graphics/Sampler.h"
namespace eg {
    namespace graphics {
        struct TextureSubresourceRange {
            uint32_t baseMipLevel{ 0 };
            uint32_t levelCount{ 1 };
            uint32_t baseArrayLayer{ 0 };
            uint32_t layerCount{ 1 };
            TextureAspectFlag aspect{ TextureAspectFlag::COLOR };
        };

        struct TextureInfo
        {
            TextureInfo() = default;
            TextureInfo(uint32_t w, uint32_t h,
                Format format, TextureUsageFlag usage,
                MemoryPropertyFlag mem = MemoryPropertyFlag::DEVICE_LOCAL,
                DescriptorType desc = DescriptorType::COMBINED_IMAGE_SAMPLER,
                ShaderStageFlag stage = ShaderStageFlag::FRAGMENT
            ) {
                width = w;
                height = h;
                memProp = mem;
                this->format = format;
                this->usage = usage;
                descType = desc;
                this->stage = stage;
            }
            MemoryPropertyFlag memProp{ MemoryPropertyFlag::DEVICE_LOCAL };
            uint32_t width{ 0 };
            uint32_t height{ 0 };
            uint32_t depth{ 1 };
            TextureType type{ TextureType::TEX_2D };
            uint32_t levels{ 1 };
            uint32_t layers{ 1 };
            SharingMode sharing{ SharingMode::EXCLUSIVE };
            TextureLayout initialLayout{ TextureLayout::UNDEFINED };
            TextureUsageFlag usage{ TextureUsageFlag::COLOR_ATTACHMENT };
            SampleCountFlag samples{ SampleCountFlag::COUNT_1 };
            TextureTiling tiling{ TextureTiling::OPTIMAL };
            Format format{ Format::B8G8R8A8_UNORM };
            TextureSubresourceRange subresourceRange{};
            DescriptorType descType{ DescriptorType::COMBINED_IMAGE_SAMPLER };
            ShaderStageFlag stage{ ShaderStageFlag::FRAGMENT };
            ComponentMapping components {
                ComponentSwizzle::IDENTITY,
                ComponentSwizzle::IDENTITY,
                ComponentSwizzle::IDENTITY,
                ComponentSwizzle::IDENTITY
            };
        };

        class Texture
        {
        public:
            Texture() = default;
            Texture(const TextureInfo& info);
            virtual void create();
            virtual const std::unique_ptr<Sampler>& createSampler(const SamplerInfo& info = {});
            virtual ~Texture();
            inline const TextureInfo& getTextureInfo() const { return _info; }
            virtual void loadTexture(const std::string& filePath) = 0;
            inline const uint64_t getSize() const { return _size; }
            inline const std::unique_ptr<Sampler>& getSampler() const { return _sampler; }
        protected:
            TextureInfo _info{};
            uint64_t _size{ 0 };
            std::unique_ptr<Sampler> _sampler{ nullptr };
            void* _data{ nullptr };
        };
    }
}
