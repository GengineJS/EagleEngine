/*
 *  CommandBuffer.h
 *
 *  Copyright (c) 2022 by XiangKui Zheng
 *  This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */

#pragma once
#include "graphics/GraphicsDef.h"
#include "graphics/RenderPass.h"
#include "graphics/DescriptorSet.h"
#include "graphics/GraphicsPipeline.h"
#include "graphics/VertexInput.h"
#include <unordered_map>

namespace eg {
    namespace graphics {
        struct CommandBufferInfo {
            uint32_t queueIdx{ UINT32_MAX };
            CommandPoolCreateFlag flags{ CommandPoolCreateFlag::RESET_COMMAND_BUFFER };
            CommandBufferLevel level{ CommandBufferLevel::PRIMARY };
        };
        struct BindDescriptorInfo
        {
            BindDescriptorInfo() = default;
            BindDescriptorInfo(uint32_t set, DescriptorSet* ds):
                firstSet(set), ds(ds) {}
            DescriptorSet* ds{nullptr};
            std::shared_ptr<const uint32_t> pDynamicOffsets{nullptr};
            PipelineBindPoint bindPoint{ PipelineBindPoint::GRAPHICS };
            uint32_t        firstSet{ 0 };
            uint32_t        descriptorSetCount{ 1 };
            uint32_t        dynamicOffsetCount{ 0 };
        };
        struct BindPipelineInfo {
            GraphicsPipeline* pipeline{ nullptr };
            PipelineBindPoint bindPoint{ PipelineBindPoint::GRAPHICS };
        };
        class CommandBuffer
        {
        public:
            CommandBuffer() = default;
            virtual ~CommandBuffer();
            CommandBuffer(const CommandBufferInfo& info);
            virtual void destroy();
            virtual void reset();
            virtual void beginCommandBuffer();
            virtual void beginRenderPass(const RenderPassBeginInfo& info);
            virtual void bindDescriptorSets(const BindDescriptorInfo& info) = 0;
            virtual void bindDescriptorSets(uint32_t set, DescriptorSet* ds) = 0;
            virtual void bindPipeline(const BindPipelineInfo& info) = 0;
            virtual void bindPipeline(GraphicsPipeline* pipeline, PipelineBindPoint bindPoint = PipelineBindPoint::GRAPHICS) = 0;
            virtual void copyTexture(Texture* srcTex, Texture* dstTex) = 0;
            virtual void setViewport(const Viewport& viewport, uint32_t firstViewport = 0, uint32_t viewportCount = 1);
            virtual void setLineWidth(const float lineWidth) = 0;
            virtual void setBlendConstants(const Vector4& constant) = 0;
            virtual void setDepthBias(const Vector3& bias) = 0;
            virtual void setDepthBounds(const Vector2& bounds) = 0;
            virtual void setScissor(const Rect2D& scissor, uint32_t firstScissor = 0, uint32_t scissorCount = 1);
            virtual void draw(VertexInput* vi);
            virtual void endRenderPass() = 0;
            virtual void endCommandBuffer() = 0;
            RenderPass* getCurrentRenderPass() const { return _currPass; }
            Framebuffer* getCurrentFramebuffer() const { return _currFramebuffer; }
        protected:
            CommandBufferInfo _info{};
            bool _isSetDynViewport{ false };
            bool _isSetDynScissor{ false };
            Rect2D _renderArea{};
            std::vector<DynamicState> _dynamics{};
            RenderPass* _currPass{ nullptr };
            Framebuffer* _currFramebuffer{ nullptr };
        };
    }
}
