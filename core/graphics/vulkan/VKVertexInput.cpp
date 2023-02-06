/*
 *  VKInputAssembler.cpp
 *
 *  Copyright (c) 2022 by XiangKui Zheng
 *  This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */

#include <graphics/vulkan/VKVertexInput.h>
#include <graphics/vulkan/VKBuffer.h>
#include <graphics/vulkan/VKContext.h>
#include <graphics/vulkan/VKDevice.h>

namespace eg {
	namespace graphics {
		VKInputAssembler::VKInputAssembler(const InputAssemblerInfo& info) : InputAssembler(info)
		{
			_assembler.sType = VK_STRUCTURE_TYPE_PIPELINE_INPUT_ASSEMBLY_STATE_CREATE_INFO;
			_assembler.primitiveRestartEnable = _info.isPrimitiveRestart;
			_assembler.topology = static_cast<VkPrimitiveTopology>(info.topology);
		}
		VKInputAssembler::~VKInputAssembler()
		{
			destroy();
		}
		void VKInputAssembler::destroy()
		{
		}
		VKVertexInput::VKVertexInput(const VertexInputInfo& info): VertexInput(info)
		{
			_vertState.sType = VK_STRUCTURE_TYPE_PIPELINE_VERTEX_INPUT_STATE_CREATE_INFO;
			
			for (auto& binding : _stateInfo.bindings) {
				vkBindings.emplace_back(VkVertexInputBindingDescription{
					binding.binding,
					binding.stride,
					static_cast<VkVertexInputRate>(binding.inputRate)
					});
			}
			_vertState.vertexBindingDescriptionCount = static_cast<uint32_t>(vkBindings.size());
			_vertState.pVertexBindingDescriptions = vkBindings.data();

			for (auto& attr : _stateInfo.attributes) {
				vkAttrs.emplace_back(VkVertexInputAttributeDescription{
					attr.location,
					attr.binding,
					static_cast<VkFormat>(attr.format),
					attr.offset
					});
			}
			_vertState.vertexAttributeDescriptionCount = static_cast<uint32_t>(vkAttrs.size());
			_vertState.pVertexAttributeDescriptions = vkAttrs.data();
		}
		VKVertexInput::~VKVertexInput()
		{
			destroy();
		}
		void VKVertexInput::destroy()
		{
			vkBindings.clear();
			vkAttrs.clear();
		}
	}
}