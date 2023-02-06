/*
 *  VKPipelineLayout.cpp
 *
 *  Copyright (c) 2022 by XiangKui Zheng
 *  This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */

#include <graphics/vulkan/VKPipelineLayout.h>
#include <graphics/vulkan/VKContext.h>
#include <graphics/vulkan/VKDevice.h>
#include <graphics/vulkan/VKDescriptorSetLayout.h>
#include <graphics/vulkan/VKTools.h>

namespace eg {
	namespace graphics {
		VKPipelineLayout::VKPipelineLayout(const PipelineLayoutInfo& info): PipelineLayout(info)
		{
			auto context = Context::GetContext();
			auto device = dynamic_cast<VKDevice*>(context->getDevice().get());
			VkPipelineLayoutCreateInfo pipLayoutInfo{};
			pipLayoutInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_LAYOUT_CREATE_INFO;
			std::vector<VkDescriptorSetLayout> vkDescLayouts{};
			for (auto currLayout: _info.pSetLayouts) {
				auto vkDesc = dynamic_cast<VKDescriptorSetLayout*>(currLayout);
				vkDescLayouts.emplace_back(vkDesc->getVkDescriptorSetLayout());
			}
			pipLayoutInfo.setLayoutCount = static_cast<uint32_t>(vkDescLayouts.size());
			pipLayoutInfo.pSetLayouts = vkDescLayouts.data();
			std::vector<VkPushConstantRange> vkPushConstants{};
			for (auto currConstant: info.pushConstantRanges) {
				vkPushConstants.emplace_back(VkPushConstantRange{
					static_cast<VkShaderStageFlags>(currConstant.stageFlag),
					currConstant.offset,
					currConstant.size });
			}
			pipLayoutInfo.pushConstantRangeCount = static_cast<uint32_t>(vkPushConstants.size());
			pipLayoutInfo.pPushConstantRanges = vkPushConstants.data();
			VK_CHECK_RESULT(vkCreatePipelineLayout(device->getLogicDevice(), &pipLayoutInfo, nullptr, &layout));
		}
		VKPipelineLayout::~VKPipelineLayout()
		{
			destroy();
		}
		void VKPipelineLayout::destroy()
		{
			auto context = Context::GetContext();
			auto device = dynamic_cast<VKDevice*>(context->getDevice().get());
			vkDestroyPipelineLayout(device->getLogicDevice(), layout, nullptr);
		}
	}
}