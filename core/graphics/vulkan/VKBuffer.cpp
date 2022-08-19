#include <graphics/vulkan/VKBuffer.h>
#include <graphics/vulkan/VKTools.h>
#include <graphics/vulkan/VKContext.h>
#include <graphics/vulkan/VKDevice.h>
#include <graphics/vulkan/VKCommandBuffer.h>
#include <stdlib.h>
namespace eg {
	namespace graphics {
		VKBuffer::VKBuffer(const BufferInfo& info) : Buffer(info)
		{
			_create();
		}

		VKBuffer::~VKBuffer()
		{
			_destroy();
		}

		void VKBuffer::update(const void* buffer, uint64_t size, uint64_t offset)
		{
			if (_info.offset != offset) {
				_info.offset = offset;
			}
			auto context = Context::GetContext();
			auto device = std::dynamic_pointer_cast<VKDevice>(context->getDevice());
			resize(size);
			vkMapMemory(device->getLogicDevice(), _mem, offset, size, 0, &_data);
			memcpy(_data, buffer, size);
			vkUnmapMemory(device->getLogicDevice(), _mem);
		}

		void VKBuffer::resize(uint64_t size)
		{
			if (size != _info.size) {
				_info.size = size;
				_destroy();
				_create();
			}
		}

		void VKBuffer::copyBuffer(std::shared_ptr<Buffer> src)
		{
			assert(src);
			auto context = Context::GetContext();
			auto device = std::dynamic_pointer_cast<VKDevice>(context->getDevice());
			auto srcBuff = std::dynamic_pointer_cast<VKBuffer>(src);
			CommandBufferInfo cmdInfo{};
			cmdInfo.queueIdx = device->getQueueFamilyIndices().graphics;
			std::shared_ptr<VKCommandBuffer> cmdBuff = std::make_shared<VKCommandBuffer>(cmdInfo);
			cmdBuff->beginSingleTimeCommand();

			VkBufferCopy bufferCopy{};
			bufferCopy.size = _info.size;
			bufferCopy.dstOffset = 0;
			bufferCopy.srcOffset = 0;
			vkCmdCopyBuffer(cmdBuff->getVkCmdBuffer(), srcBuff->getVkBuffer(), _buffer, 1, &bufferCopy);

			cmdBuff->endSingleTimeCommand();
		}

		void VKBuffer::_create()
		{
			auto context = Context::GetContext();
			auto device = std::dynamic_pointer_cast<VKDevice>(context->getDevice());
			VkBufferCreateInfo bufferCreateInfo{};
			bufferCreateInfo.sType = VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO;
			bufferCreateInfo.usage = static_cast<VkBufferUsageFlagBits>(_info.usage);
			bufferCreateInfo.sharingMode = static_cast<VkSharingMode>(_info.sharing);
			bufferCreateInfo.size = _info.size;
			bufferCreateInfo.pQueueFamilyIndices = _info.queueFamilyIndics.data();
			bufferCreateInfo.queueFamilyIndexCount = static_cast<uint32_t>(_info.queueFamilyIndics.size());
			VK_CHECK_RESULT(vkCreateBuffer(device->getLogicDevice(), &bufferCreateInfo, nullptr, &_buffer));

			VkMemoryRequirements memoryRequire{};
			vkGetBufferMemoryRequirements(device->getLogicDevice(), _buffer, &memoryRequire);

			VkMemoryAllocateInfo allocateInfo{};
			allocateInfo.sType = VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO;
			allocateInfo.allocationSize = memoryRequire.size;
			allocateInfo.memoryTypeIndex = device->getMemoryTypeIndex(memoryRequire.memoryTypeBits, static_cast<VkMemoryPropertyFlags>(_info.memProp));
			VK_CHECK_RESULT(vkAllocateMemory(device->getLogicDevice(), &allocateInfo, nullptr, &_mem));
			vkBindBufferMemory(device->getLogicDevice(), _buffer, _mem, 0);
		}

		void VKBuffer::_destroy()
		{
			auto context = Context::GetContext();
			auto device = std::dynamic_pointer_cast<VKDevice>(context->getDevice());
			if (_buffer) {
				vkDestroyBuffer(device->getLogicDevice(), _buffer, nullptr);
			}
			if (_mem) {
				vkFreeMemory(device->getLogicDevice(), _mem, nullptr);
			}
		}

		VKBufferView::VKBufferView(const BufferViewInfo& info)
		{
			auto context = Context::GetContext();
			auto device = std::dynamic_pointer_cast<VKDevice>(context->getDevice());
			auto currBuff = std::dynamic_pointer_cast<VKBuffer>(info.buffer);
			VkBufferViewCreateInfo _createInfo{};
			_createInfo.sType = VK_STRUCTURE_TYPE_BUFFER_VIEW_CREATE_INFO;
			_createInfo.buffer = currBuff->getVkBuffer();
			_createInfo.format = static_cast<VkFormat>(info.format);
			_createInfo.offset = info.offset;
			_createInfo.range = info.range;
			VK_CHECK_RESULT(vkCreateBufferView(device->getLogicDevice(), &_createInfo, nullptr, &_bufferView));
		}

		VKBufferView::~VKBufferView()
		{
			auto context = Context::GetContext();
			auto device = std::dynamic_pointer_cast<VKDevice>(context->getDevice());
			vkDestroyBufferView(device->getLogicDevice(), _bufferView, nullptr);
		}
	}
}
