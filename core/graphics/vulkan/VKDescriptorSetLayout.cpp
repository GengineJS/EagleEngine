/*
 *  VKDescriptorSetLayout.cpp
 *
 *  Copyright (c) 2022 by XiangKui Zheng
 *  This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */

#include <graphics/vulkan/VKDescriptorSetLayout.h>
#include <graphics/vulkan/VKContext.h>
#include <graphics/vulkan/VKDevice.h>
#include <vulkan/vulkan.h>
#include <graphics/vulkan/VKSampler.h>
#include <graphics/vulkan/VKTools.h>
namespace eg {
	namespace graphics{
		VKDescriptorSetLayout::VKDescriptorSetLayout(const DescriptorSetLayoutInfo& info): DescriptorSetLayout(info)
		{
			auto context = Context::GetContext();
			auto device = dynamic_cast<VKDevice*>(context->getDevice().get());
			std::vector<VkDescriptorSetLayoutBinding> vkLayoutBinds{};
			for (auto& bind: _info.bindings) {
				std::vector<VkSampler> samplers{};
				auto& imSamplers = bind.immutableSamplers;
				for (auto sampler: imSamplers) {
					auto vkSampler = dynamic_cast<VKSampler*>(sampler);
					samplers.emplace_back(vkSampler->getVkSampler());
				}
				vkLayoutBinds.emplace_back(VkDescriptorSetLayoutBinding{
					bind.binding,
					static_cast<VkDescriptorType>(bind.type),
					bind.descriptorCount,
					static_cast<VkShaderStageFlags>(bind.stage),
					samplers.data()
				});
			}
			VkDescriptorSetLayoutCreateInfo layoutInfo{};
			layoutInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_LAYOUT_CREATE_INFO;
			layoutInfo.bindingCount = static_cast<uint32_t>(vkLayoutBinds.size());
			layoutInfo.pBindings = vkLayoutBinds.data();
			layoutInfo.flags = static_cast<VkDescriptorSetLayoutCreateFlags>(info.flag);

			VK_CHECK_RESULT(vkCreateDescriptorSetLayout(device->getLogicDevice(), &layoutInfo, nullptr, &layout));
		}
		VKDescriptorSetLayout::~VKDescriptorSetLayout()
		{
			destroy();
		}
		void VKDescriptorSetLayout::destroy()
		{
			auto context = Context::GetContext();
			auto device = dynamic_cast<VKDevice*>(context->getDevice().get());
			vkDestroyDescriptorSetLayout(device->getLogicDevice(), layout, nullptr);
		}
	}
}