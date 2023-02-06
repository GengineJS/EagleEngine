/*
 *  VKCommandBuffer.h
 *
 *  Copyright (c) 2022 by XiangKui Zheng
 *  This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */

#pragma once
#include <graphics/CommandBuffer.h>
#include <vulkan/vulkan.h>
namespace eg {
	namespace graphics {
		class VKCommandBuffer : public CommandBuffer
		{
		public:
			VKCommandBuffer() = default;
			VKCommandBuffer(const CommandBufferInfo& info);
			~VKCommandBuffer() override;
            void reset() override;
            void beginCommandBuffer() override;
            void beginRenderPass(const RenderPassBeginInfo& info) override;
            void bindDescriptorSets(const BindDescriptorInfo& info) override;
            void bindDescriptorSets(uint32_t set, DescriptorSet* ds) override;
            void bindPipeline(const BindPipelineInfo& info) override;
            void bindPipeline(GraphicsPipeline* pipeline, PipelineBindPoint bindPoint = PipelineBindPoint::GRAPHICS) override;
            void copyTexture(Texture* srcTex, Texture* dstTex) override;
            void setViewport(const Viewport& viewport, uint32_t firstViewport = 0, uint32_t viewportCount = 1) override;
            void setLineWidth(const float lineWidth) override;
            void setBlendConstants(const Vector4& constant) override;
            void setDepthBias(const Vector3& bias) override;
            void setDepthBounds(const Vector2& bounds) override;
            void setScissor(const Rect2D& scissor, uint32_t firstScissor = 0, uint32_t scissorCount = 1) override;
            void draw(VertexInput* vi) override;
            void endRenderPass() override;
            void endCommandBuffer() override;
            const VkCommandBuffer& getVkCmdBuffer() const { return _cmdBuffer; }
            void beginSingleTimeCommand();
            void endSingleTimeCommand();
        protected:
            VkCommandBuffer _cmdBuffer{VK_NULL_HANDLE};
			VkCommandPool _cmdPool;
		};
	}
}
