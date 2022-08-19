#pragma once
#include <graphics/Buffer.h>
#include <vulkan/vulkan.h>
namespace eg {
    namespace graphics {
        class VKBuffer : public Buffer
        {
        public:
            VKBuffer() = default;
            VKBuffer(const BufferInfo& info);
            ~VKBuffer() override;

            void update(const void* buffer, uint64_t size, uint64_t offset) override;
            void resize(uint64_t size) override;
            inline const VkBuffer& getVkBuffer() const { return _buffer; }
            inline const VkDeviceMemory& getDeviceMemory() const { return _mem; }
            void copyBuffer(std::shared_ptr<Buffer> src) override;
        protected:
            void _create();
            void _destroy();
            VkBuffer _buffer;
            VkDeviceMemory _mem;
        };

        class VKBufferView : public BufferView {
        public:
            VKBufferView() = default;
            VKBufferView(const BufferViewInfo& info);
            ~VKBufferView();
            inline const VkBufferView& getVkBufferView() const { return _bufferView; }
        protected:
            VkBufferView _bufferView;

        };
    }
}
