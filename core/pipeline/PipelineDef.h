#pragma once

#include <cstdint>
#include <string>
#include <utils/Utils.h>
#include <graphics/GraphicsDef.h>
namespace eg {
    namespace pipeline {
        ENUM_FLAGS(AccessState) {
            READ,
            WRITE,
            READ_WRITE,
        };
        ENUM_FLAGS(AttachmentState) {
            COLOR_TEXTURE,
            DEPTH_STENCIL,
        };
        ENUM_FLAGS(ClearFlag) {
            NONE = 0,
            COLOR = 0x1,
            DEPTH = 0x2,
            STENCIL = 0x4,
            DEPTH_STENCIL = DEPTH | STENCIL,
            ALL = COLOR | DEPTH | STENCIL,
        };
        struct ClearValue
        {
            graphics::ClearColor  color{};
            graphics::ClearDepthStencil depthStencil{};
        };
        struct PassView
        {
            ClearFlag clearFlag{ ClearFlag::ALL };
            ClearValue clearVal{};
            graphics::SampleCountFlag samples{ graphics::SampleCountFlag::COUNT_1 };
        };
        enum class ResourceUsage
        {
            NORMAL,
            BACK_BUFFER
        };
        enum class ResourceType {
            UNKNOWN,
            TEXTURE,
            DEPTH_STENCIL,
            BUFFER,
        };
        struct SlotDesc {
            uint32_t set{ 0 };
            uint32_t binding{ 1 };
            graphics::DescriptorType descType{ graphics::DescriptorType::COMBINED_IMAGE_SAMPLER };
            graphics::ShaderStageFlag stage{ graphics::ShaderStageFlag::FRAGMENT };
        };
    }
}