#include "graphics/vulkan/VKCommandBuffer.h"
#include "graphics/vulkan/VKContext.h"
#include "graphics/vulkan/VKDevice.h"
#include <graphics/vulkan/VKTools.h>
#include <graphics/vulkan/VKSwapchain.h>
#include <array>
#include <graphics/vulkan/VKFramebuffer.h>
#include <graphics/vulkan/VKRenderPass.h>
#include <graphics/vulkan/VKDescriptorSet.h>
#include <graphics/vulkan/VKGraphicsPipeline.h>
#include <graphics/vulkan/VKPipelineLayout.h>
#include <graphics/vulkan/VKVertexInput.h>
#include <graphics/vulkan/VKBuffer.h>
#include <graphics/vulkan/VKTexture.h>
namespace eg {
	namespace graphics {
		VKCommandBuffer::VKCommandBuffer(const CommandBufferInfo& info) : CommandBuffer(info)
		{
			auto context = Context::GetContext();
			auto device = std::dynamic_pointer_cast<VKDevice>(context->getDevice());
			VkCommandPoolCreateInfo cmdPoolInfo{};
			cmdPoolInfo.sType = VK_STRUCTURE_TYPE_COMMAND_POOL_CREATE_INFO;
			cmdPoolInfo.flags = static_cast<VkCommandPoolCreateFlags>(info.flags);
			cmdPoolInfo.queueFamilyIndex = info.queueIdx;
			VK_CHECK_RESULT(vkCreateCommandPool(device->getLogicDevice(), &cmdPoolInfo, nullptr, &_cmdPool));
			VkCommandBufferAllocateInfo cmdBufAllocateInfo{};
			cmdBufAllocateInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
			cmdBufAllocateInfo.commandPool = _cmdPool;
			cmdBufAllocateInfo.commandBufferCount = 1;
			cmdBufAllocateInfo.level = static_cast<VkCommandBufferLevel>(info.level);
			VK_CHECK_RESULT(vkAllocateCommandBuffers(device->getLogicDevice(), &cmdBufAllocateInfo, &_cmdBuffer));
		}

		VKCommandBuffer::~VKCommandBuffer()
		{
			auto context = Context::GetContext();
			auto device = std::dynamic_pointer_cast<VKDevice>(context->getDevice());
			vkFreeCommandBuffers(device->getLogicDevice(), _cmdPool, 1, &_cmdBuffer);
		}

		void VKCommandBuffer::reset()
		{
			CommandBuffer::reset();
			vkResetCommandBuffer(_cmdBuffer, 0);
		}

		void VKCommandBuffer::beginCommandBuffer()
		{

			CommandBuffer::beginCommandBuffer();
			VkCommandBufferBeginInfo beginInfo{};
			beginInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;
			vkBeginCommandBuffer(_cmdBuffer, &beginInfo);
		}

		void VKCommandBuffer::beginRenderPass(const RenderPassBeginInfo& info)
		{
			auto framebuffer = std::dynamic_pointer_cast<VKFramebuffer>(info.framebuffer);
			auto renderpass = std::dynamic_pointer_cast<VKRenderPass>(info.renderPass);
			CommandBuffer::beginRenderPass(info);
			VkRenderPassBeginInfo renderPassInfo{};
			renderPassInfo.sType = VK_STRUCTURE_TYPE_RENDER_PASS_BEGIN_INFO;
			std::array<VkClearValue, 2> clearValues{};
			clearValues[0].color = { info.clearColor.r, info.clearColor.g, info.clearColor.b, info.clearColor.a };
			clearValues[1].depthStencil = { info.clearDepthStencil.depth, info.clearDepthStencil.stencil };
			renderPassInfo.clearValueCount = static_cast<uint32_t>(clearValues.size());
			renderPassInfo.pClearValues = clearValues.data();
			renderPassInfo.framebuffer = framebuffer->getVkFramebuffer();
			renderPassInfo.renderArea = { info.renderArea.x, info.renderArea.y,  info.renderArea.w, info.renderArea.h };
			renderPassInfo.renderPass = renderpass->getVkRenderPass();
			vkCmdBeginRenderPass(_cmdBuffer, &renderPassInfo, static_cast<VkSubpassContents>(info.subpassContents));
		}

		void VKCommandBuffer::bindDescriptorSets(const BindDescriptorInfo& info)
		{
			assert(info.ds);
			auto desc = std::dynamic_pointer_cast<VKDescriptorSet>(info.ds);
			auto pipeLayout = std::dynamic_pointer_cast<VKPipelineLayout>(desc->getPipelineLayout());
			vkCmdBindDescriptorSets(_cmdBuffer, static_cast<VkPipelineBindPoint>(info.bindPoint),
				pipeLayout->getVkPipelineLayout(),
				info.firstSet, info.descriptorSetCount, desc->getVkDescriptorSets().data(), info.dynamicOffsetCount,
				info.pDynamicOffsets.get());
		}

		void VKCommandBuffer::bindDescriptorSets(uint32_t set, std::shared_ptr<DescriptorSet> ds)
		{
			BindDescriptorInfo info(set, ds);
			bindDescriptorSets(info);
		}

		void VKCommandBuffer::bindPipeline(const BindPipelineInfo& info)
		{
			_dynamics = info.pipeline->getGraphicsPipelineInfo().dynamicInfo.dynamicStates;
			auto pipeline = std::dynamic_pointer_cast<VKGraphicsPipeline>(info.pipeline);
			vkCmdBindPipeline(_cmdBuffer, static_cast<VkPipelineBindPoint>(info.bindPoint), pipeline->getVkPipeline());
		}

		void VKCommandBuffer::bindPipeline(std::shared_ptr<GraphicsPipeline> pipeline, PipelineBindPoint bindPoint)
		{
			BindPipelineInfo info{};
			info.bindPoint = bindPoint;
			info.pipeline = pipeline;
			bindPipeline(info);
		}

		void VKCommandBuffer::copyTexture(std::shared_ptr<Texture> srcTex, std::shared_ptr<Texture> dstTex)
		{
			auto srcVkTex = std::dynamic_pointer_cast<VKTexture>(srcTex);
			auto dstVkTex = std::dynamic_pointer_cast<VKTexture>(dstTex);
			VkImageCopy imageCopyRegion{};
			imageCopyRegion.srcSubresource.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
			imageCopyRegion.srcSubresource.layerCount = 1;
			imageCopyRegion.dstSubresource.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
			imageCopyRegion.dstSubresource.layerCount = 1;
			imageCopyRegion.extent.width = dstTex->getTextureInfo().width;
			imageCopyRegion.extent.height = dstTex->getTextureInfo().height;
			imageCopyRegion.extent.depth = 1;

			// Issue the copy command
			vkCmdCopyImage(
				_cmdBuffer,
				srcVkTex->getVkImage(), VK_IMAGE_LAYOUT_TRANSFER_SRC_OPTIMAL,
				dstVkTex->getVkImage(), VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL,
				1,
				&imageCopyRegion);
		}

		void VKCommandBuffer::setViewport(const Viewport& viewport, uint32_t firstViewport, uint32_t viewportCount)
		{
			CommandBuffer::setViewport(viewport, firstViewport, viewportCount);
			VkViewport vkViewport{};
			vkViewport.x = viewport.x;
			vkViewport.y = viewport.y;
			vkViewport.width = viewport.w;
			vkViewport.height = viewport.h;
			vkViewport.minDepth = viewport.minDepth;
			vkViewport.maxDepth = viewport.maxDepth;
			vkCmdSetViewport(_cmdBuffer, firstViewport, viewportCount, &vkViewport);
		}

		void VKCommandBuffer::setLineWidth(const float lineWidth)
		{
			vkCmdSetLineWidth(_cmdBuffer, lineWidth);
		}

		void VKCommandBuffer::setBlendConstants(const Vector4& constant)
		{
			float constans[4] = { constant.x, constant.y, constant.z, constant.w };
			vkCmdSetBlendConstants(_cmdBuffer, constans);
		}

		void VKCommandBuffer::setDepthBias(const Vector3& bias)
		{
			vkCmdSetDepthBias(_cmdBuffer, bias.x, bias.y, bias.z);
		}

		void VKCommandBuffer::setDepthBounds(const Vector2& bounds)
		{
			vkCmdSetDepthBounds(_cmdBuffer, bounds.x, bounds.y);
		}

		void VKCommandBuffer::setScissor(const Rect2D& scissor, uint32_t firstScissor, uint32_t scissorCount)
		{
			CommandBuffer::setScissor(scissor, firstScissor, scissorCount);
			VkRect2D rect{ scissor.x, scissor.y, scissor.w, scissor.h };
			vkCmdSetScissor(_cmdBuffer, firstScissor, scissorCount, &rect);
		}

		void VKCommandBuffer::draw(const std::shared_ptr<VertexInput> vi)
		{
			CommandBuffer::draw(vi);
			assert(vi);
			auto vkVI = std::dynamic_pointer_cast<VKVertexInput>(vi);
			VertexInputInfo inputInfo = vkVI->getVertexInputInfo();
			auto idxBuffer = std::dynamic_pointer_cast<VKBuffer>(vkVI->getIndexBuffer());
			auto vertBuffer = std::dynamic_pointer_cast<VKBuffer>(vkVI->getVertexBuffer());
			assert(idxBuffer || vertBuffer);
			if (vertBuffer) {
				auto buff = vertBuffer->getVkBuffer();
				std::vector<VkDeviceSize> offsets{ 0 };
				vkCmdBindVertexBuffers(_cmdBuffer, 0, 1, &buff, offsets.data());
			}
			if (idxBuffer) {
				vkCmdBindIndexBuffer(_cmdBuffer, idxBuffer->getVkBuffer(),
					idxBuffer->getBufferInfo().offset,
					VK_INDEX_TYPE_UINT32);

			}
			/*if (inputInfo.indirectBuffer) {
				BufferInfo buffInfo = inputInfo.indirectBuffer->getBufferInfo();
				vkCmdDrawIndirect(_currCmdBuffer, dynamic_cast<VKBuffer*>(inputInfo.indirectBuffer)->getVkBuffer(),
					buffInfo.offset,
					static_cast<uint32_t>(buffInfo.size),
					static_cast<uint32_t>(buffInfo.stride));
			}

			else */
			if (idxBuffer) {
				vkCmdDrawIndexed(_cmdBuffer, static_cast<uint32_t>(idxBuffer->getBufferInfo().size),
					1, 0, 0, 0);
			}
			else if (vertBuffer) {
				vkCmdDraw(_cmdBuffer, static_cast<uint32_t>(vertBuffer->getBufferInfo().size),
					1, 0, 0);
			}
		}

		void VKCommandBuffer::endRenderPass()
		{
			vkCmdEndRenderPass(_cmdBuffer);
		}

		void VKCommandBuffer::endCommandBuffer()
		{
			vkEndCommandBuffer(_cmdBuffer);
		}

		void VKCommandBuffer::beginSingleTimeCommand()
		{
			auto context = Context::GetContext();
			auto device = std::dynamic_pointer_cast<VKDevice>(context->getDevice());
			VkCommandBufferBeginInfo beginInfo{};
			beginInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;
			beginInfo.flags = VK_COMMAND_BUFFER_USAGE_ONE_TIME_SUBMIT_BIT;
			vkBeginCommandBuffer(_cmdBuffer, &beginInfo);
		}

		void VKCommandBuffer::endSingleTimeCommand()
		{
			auto context = Context::GetContext();
			auto device = std::dynamic_pointer_cast<VKDevice>(context->getDevice());
			vkEndCommandBuffer(_cmdBuffer);
			VkSubmitInfo submitInfo{};
			submitInfo.commandBufferCount = 1;
			submitInfo.pCommandBuffers = &_cmdBuffer;
			submitInfo.sType = VK_STRUCTURE_TYPE_SUBMIT_INFO;

			vkQueueSubmit(device->getGraphicQueue(), 1, &submitInfo, VK_NULL_HANDLE);
			vkQueueWaitIdle(device->getGraphicQueue());
			vkFreeCommandBuffers(device->getLogicDevice(), _cmdPool, 1, &_cmdBuffer);
		}
	}
}
