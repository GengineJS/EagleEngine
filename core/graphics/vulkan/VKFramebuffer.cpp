/*
 *  VKFramebuffer.cpp
 *
 *  Copyright (c) 2022 by XiangKui Zheng
 *  This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */

#include <graphics/vulkan/VKFramebuffer.h>
#include <graphics/vulkan/VKTexture.h>
#include <graphics/vulkan/VKContext.h>
#include <graphics/vulkan/VKDevice.h>
#include <graphics/vulkan/VKRenderPass.h>
#include <graphics/vulkan/VKTools.h>
namespace eg {
	namespace graphics {
		VKFramebuffer::VKFramebuffer(const FramebufferInfo& info) : Framebuffer(info)
		{
			auto context = std::dynamic_pointer_cast<VKContext>(Context::GetContext());
			auto device = std::dynamic_pointer_cast<VKDevice>(context->getDevice());
			auto vkRenderPass = std::dynamic_pointer_cast<VKRenderPass>(info.renderPass);
			std::vector<VkImageView> attachments{};
			for (auto tex : info.pAttachments) {
				auto vkTex = std::dynamic_pointer_cast<VKTexture>(tex);
				attachments.emplace_back(vkTex->getVkImageView());
			}
			VkFramebufferCreateInfo fboInfo{};
			fboInfo.sType = VK_STRUCTURE_TYPE_FRAMEBUFFER_CREATE_INFO;
			fboInfo.attachmentCount = static_cast<uint32_t>(attachments.size());
			fboInfo.height = info.height;
			fboInfo.width = info.width;
			fboInfo.layers = info.layers;
			fboInfo.pAttachments = attachments.data();
			fboInfo.renderPass = vkRenderPass->getVkRenderPass();
			VK_CHECK_RESULT(vkCreateFramebuffer(device->getLogicDevice(), &fboInfo, nullptr, &_vkFramebuffer));
		}

		VKFramebuffer::~VKFramebuffer()
		{
			destroy();
		}
		void VKFramebuffer::destroy()
		{
			auto context = Context::GetContext();
			auto device = std::dynamic_pointer_cast<VKDevice>(context->getDevice());
			vkDestroyFramebuffer(device->getLogicDevice(), _vkFramebuffer, nullptr);
		}
	}
}
