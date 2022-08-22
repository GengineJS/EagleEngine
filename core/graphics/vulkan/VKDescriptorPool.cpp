/*
 *  VKDescriptorPool.cpp
 *
 *  Copyright (c) 2022 by XiangKui Zheng
 *  This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */

#include <graphics/vulkan/VKDescriptorPool.h>
#include <graphics/vulkan/VKContext.h>
#include <graphics/vulkan/VKDevice.h>
#include <graphics/vulkan/VKTools.h>

namespace eg {
	namespace graphics {
		VKDescriptorPool::VKDescriptorPool(const DescriptorPoolInfo& info): DescriptorPool(info)
		{
			auto context = Context::GetContext();
			auto device = std::dynamic_pointer_cast<VKDevice>(context->getDevice());
			VkDescriptorPoolCreateInfo poolInfo{};
			poolInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_POOL_CREATE_INFO;
			std::vector<VkDescriptorPoolSize> descPoolSizes{};
			for (auto descPoolSize: _info.pPoolSizes) {
				VkDescriptorPoolSize sizeInfo{};
				sizeInfo.type = static_cast<VkDescriptorType>(descPoolSize.type);
				sizeInfo.descriptorCount = descPoolSize.descCount;
				descPoolSizes.emplace_back(std::move(sizeInfo));
			}
			poolInfo.maxSets = _info.maxSets;
			poolInfo.flags = _info.flag;
			poolInfo.poolSizeCount = static_cast<uint32_t>(descPoolSizes.size());
			poolInfo.pPoolSizes = descPoolSizes.data();
			VK_CHECK_RESULT(vkCreateDescriptorPool(device->getLogicDevice(), &poolInfo, nullptr, &_pool));
		}
		VKDescriptorPool::~VKDescriptorPool()
		{
			destroy();
		}
		void VKDescriptorPool::destroy()
		{
			auto context = Context::GetContext();
			auto device = std::dynamic_pointer_cast<VKDevice>(context->getDevice());
			vkDestroyDescriptorPool(device->getLogicDevice(), _pool, nullptr);
		}
	}
}
