#pragma once
#include "graphics/GraphicsDef.h"
#include <vector>

namespace eg {
    namespace graphics {
        struct BufferInfo
        {
            BufferInfo() = default;
            BufferInfo(uint64_t size, uint64_t offset, BufferUsageFlag usage,
                MemoryPropertyFlag memProp,
                DescriptorType desc = DescriptorType::UNIFORM_BUFFER,
                ShaderStageFlag stage = ShaderStageFlag::VERTEX
                );
            BufferUsageFlag usage{BufferUsageFlag::UNIFORM};
            MemoryPropertyFlag memProp{MemoryPropertyFlag::DEVICE_LOCAL};
            SharingMode sharing{ SharingMode::EXCLUSIVE };
            uint64_t size{ 0 };
            uint64_t offset{ 0 };
            DescriptorType descType{ DescriptorType::UNIFORM_BUFFER };
            ShaderStageFlag stage{ ShaderStageFlag::VERTEX };
            std::vector<uint32_t> queueFamilyIndics{};
        };
        class Buffer
        {
        public:
            Buffer() = default;
            Buffer(const BufferInfo& info);
            virtual ~Buffer();
            inline const BufferInfo& getBufferInfo() const { return _info; }
            void update(const void* data);
            virtual void update(const void* buffer, uint64_t size, uint64_t offset) = 0;
            virtual void resize(uint64_t size) = 0;
            virtual void copyBuffer(std::shared_ptr<Buffer> src) = 0;
        protected:
            BufferInfo _info{};
            void* _data{nullptr};
        };

        struct BufferViewInfo
        {
            std::shared_ptr<Buffer> buffer;
            Format format;
            uint64_t offset;
            uint64_t range;
        };
        class BufferView {
        public:
            BufferView() = default;
            BufferView(const BufferViewInfo& info);
            virtual ~BufferView();
            inline const BufferViewInfo& getBufferViewInfo() const { return _info; }
        protected:
            BufferViewInfo _info{};
        };
    }
}
