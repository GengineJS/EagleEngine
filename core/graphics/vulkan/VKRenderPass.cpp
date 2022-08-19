#include <graphics/vulkan/VKRenderPass.h>
#include <assert.h>
#include <graphics/vulkan/VKTools.h>
#include <graphics/vulkan/VKContext.h>
#include <graphics/vulkan/VKDevice.h>
namespace eg {
	namespace graphics {
		VKRenderPass::VKRenderPass(const RenderPassInfo& info):RenderPass(info)
		{
            auto context = Context::GetContext();
            auto device = std::dynamic_pointer_cast<VKDevice>(context->getDevice());
			if (!_info.colorAttachments.size()) {
				_info.colorAttachments.emplace_back(ColorAttachment{});
			}
			bool isEmptySubpass = false;
			if (!_info.subpasses.size()) {
				isEmptySubpass = true;
				_info.subpasses.emplace_back(SubpassInfo{});
			}
			uint32_t attachmentIdx{ 0 };
			std::vector<VkAttachmentDescription> attachments{};
			for (auto& colorAttach: _info.colorAttachments) {
				VkAttachmentDescription colDesc{};
				colDesc.format = static_cast<VkFormat>(colorAttach.format);
				colDesc.samples = static_cast<VkSampleCountFlagBits>(colorAttach.samples);
				colDesc.loadOp = static_cast<VkAttachmentLoadOp>(colorAttach.loadOp);
				colDesc.storeOp = static_cast<VkAttachmentStoreOp>(colorAttach.storeOp);
				colDesc.stencilLoadOp = VK_ATTACHMENT_LOAD_OP_DONT_CARE;
				colDesc.stencilStoreOp = VK_ATTACHMENT_STORE_OP_DONT_CARE;
				colDesc.initialLayout = static_cast<VkImageLayout>(colorAttach.initLayout);
				colDesc.finalLayout = static_cast<VkImageLayout>(colorAttach.finalLayout);
				attachments.emplace_back(std::move(colDesc));
				if (isEmptySubpass) {
					_info.subpasses[0].colors.emplace_back(attachmentIdx);
				}
				attachmentIdx++;
			}
			VkAttachmentDescription depthDesc{};
            depthDesc.format = static_cast<VkFormat>(_info.depthStencilAttachment.format);
			depthDesc.samples = static_cast<VkSampleCountFlagBits>(_info.depthStencilAttachment.samples);
			depthDesc.loadOp = static_cast<VkAttachmentLoadOp>(_info.depthStencilAttachment.loadOp);
			depthDesc.storeOp = static_cast<VkAttachmentStoreOp>(_info.depthStencilAttachment.storeOp);
			depthDesc.stencilLoadOp = static_cast<VkAttachmentLoadOp>(_info.depthStencilAttachment.loadOp);
			depthDesc.stencilStoreOp = static_cast<VkAttachmentStoreOp>(_info.depthStencilAttachment.storeOp);
			depthDesc.initialLayout = static_cast<VkImageLayout>(_info.depthStencilAttachment.initLayout);
			depthDesc.finalLayout = static_cast<VkImageLayout>(_info.depthStencilAttachment.finalLayout);
			attachments.emplace_back(std::move(depthDesc));
			if (isEmptySubpass) {
				_info.subpasses[0].depthStencil = attachmentIdx;
			}
			std::vector<VkSubpassDescription> subpassesDesc{};
			std::vector<VkAttachmentReference> colorRefs{};
			for (auto& subpass: _info.subpasses) {
				VkSubpassDescription currSubpassDesc{};
				currSubpassDesc.pipelineBindPoint = static_cast<VkPipelineBindPoint>(subpass.bindPoint);
				currSubpassDesc.colorAttachmentCount = static_cast<uint32_t>(subpass.colors.size());
				
				for (auto idx: subpass.colors) {
					VkAttachmentReference colorRef{};
					colorRef.attachment = idx;
					colorRef.layout = VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL;
					colorRefs.emplace_back(std::move(colorRef));
				}
				currSubpassDesc.pColorAttachments = colorRefs.data();
				currSubpassDesc.inputAttachmentCount = static_cast<uint32_t>(subpass.inputs.size());
				std::vector<VkAttachmentReference> inputRefs{};
				for (auto idx: subpass.inputs) {
					VkAttachmentReference inputRef{};
					inputRef.attachment = idx;
					inputRef.layout = VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL;
					inputRefs.emplace_back(std::move(inputRef));
				}
				currSubpassDesc.pInputAttachments = inputRefs.data();
				VkAttachmentReference depthRef{};
				if (subpass.depthStencil != UINT32_MAX) {
					depthRef.attachment = subpass.depthStencil;
					depthRef.layout = VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL;
					currSubpassDesc.pDepthStencilAttachment = &depthRef;
				}
				std::vector<VkAttachmentReference> resolveRefs{};
				for (auto idx: subpass.resolves) {
					VkAttachmentReference resolveRef{};
					resolveRef.attachment = idx;
					resolveRef.layout = VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL;
					resolveRefs.emplace_back(std::move(resolveRef));
				}
				currSubpassDesc.pResolveAttachments = resolveRefs.data();
				currSubpassDesc.preserveAttachmentCount = static_cast<uint32_t>(subpass.preserves.size());
				currSubpassDesc.pPreserveAttachments = subpass.preserves.data();
				subpassesDesc.emplace_back(std::move(currSubpassDesc));
			}
			if (!_info.dependencies.size()) {
				SubpassDependency dependFirst{};
				dependFirst.srcSubPass = VK_SUBPASS_EXTERNAL;
				dependFirst.srcStage = PipelineStageFlag::BOTTOM_OF_PIPE;
				dependFirst.dstStage = PipelineStageFlag::COLOR_ATTACHMENT_OUTPUT;
				dependFirst.srcAccess = AccessFlag::MEMORY_READ;
				dependFirst.dstAccess = AccessFlag::COLOR_ATTACHMENT_READ | AccessFlag::COLOR_ATTACHMENT_WRITE;
				_info.dependencies.emplace_back(std::move(dependFirst));
				SubpassDependency dependSecond{};
				dependSecond.dstSubPass = VK_SUBPASS_EXTERNAL;
				dependSecond.srcStage = PipelineStageFlag::COLOR_ATTACHMENT_OUTPUT;
				dependSecond.dstStage = PipelineStageFlag::BOTTOM_OF_PIPE;
				dependSecond.srcAccess = AccessFlag::COLOR_ATTACHMENT_READ | AccessFlag::COLOR_ATTACHMENT_WRITE;
				dependSecond.dstAccess = AccessFlag::MEMORY_READ;
				_info.dependencies.emplace_back(std::move(dependSecond));
			}
			std::vector<VkSubpassDependency> subpassesDep{};
			for (auto &dep: _info.dependencies) {
				VkSubpassDependency dependency{};
				dependency.srcSubpass = dep.srcSubPass;
				dependency.dstSubpass = dep.dstSubPass;
				dependency.srcStageMask = static_cast<VkPipelineStageFlags>(dep.srcStage);
				dependency.dstStageMask = static_cast<VkPipelineStageFlags>(dep.dstStage);
				dependency.srcAccessMask = static_cast<VkAccessFlags>(dep.srcAccess);
				dependency.dstAccessMask = static_cast<VkAccessFlags>(dep.dstAccess);
				dependency.dependencyFlags = static_cast<VkDependencyFlags>(dep.dependency);
				subpassesDep.emplace_back(std::move(dependency));
			}
			VkRenderPassCreateInfo renderPassInfo = {};
			renderPassInfo.sType = VK_STRUCTURE_TYPE_RENDER_PASS_CREATE_INFO;
			renderPassInfo.attachmentCount = static_cast<uint32_t>(attachments.size());
			renderPassInfo.pAttachments = attachments.data();
			renderPassInfo.subpassCount = static_cast<uint32_t>(subpassesDesc.size());
			renderPassInfo.pSubpasses = subpassesDesc.data();
			renderPassInfo.dependencyCount = static_cast<uint32_t>(subpassesDep.size());
			renderPassInfo.pDependencies = subpassesDep.data();

			VK_CHECK_RESULT(vkCreateRenderPass(device->getLogicDevice(), &renderPassInfo, nullptr, &_vkRenderPass));
		}
		VKRenderPass::~VKRenderPass()
		{
			destroy();
		}
		void VKRenderPass::destroy()
		{	
			auto context = Context::GetContext();
			auto device = std::dynamic_pointer_cast<VKDevice>(context->getDevice());
			vkDestroyRenderPass(device->getLogicDevice(), _vkRenderPass, nullptr);
		}
	}
}
