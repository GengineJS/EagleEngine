/*
 *  RenderPass.h
 *
 *  Copyright (c) 2022 by XiangKui Zheng
 *  This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */

#pragma once
#include <graphics/GraphicsDef.h>
#include <graphics/Framebuffer.h>
#include <vector>
namespace eg {
    namespace graphics {
        struct SubpassInfo
        {
            PipelineBindPoint bindPoint{ PipelineBindPoint::GRAPHICS };
            uint32_t depthStencil{ UINT32_MAX };
            std::vector<uint32_t> inputs;
            std::vector<uint32_t> colors;
            std::vector<uint32_t> resolves;
            std::vector<uint32_t> preserves;
        };

        struct SubpassDependency {
            uint32_t srcSubPass{ 0 };
            uint32_t dstSubPass{ 0 };
            PipelineStageFlag srcStage;
            PipelineStageFlag dstStage;
            AccessFlag srcAccess;
            AccessFlag dstAccess;
            DependencyFlag dependency{ DependencyFlag::BY_REGION };
        };

        struct RenderPassInfo {
            std::vector<ColorAttachment> colorAttachments{};
            DepthStencilAttachment depthStencilAttachment{};
            std::vector<SubpassInfo> subpasses{};
            std::vector<SubpassDependency> dependencies{};
            RenderPassInfo() = default;
            RenderPassInfo(const std::vector<ColorAttachment>& colors, DepthStencilAttachment depth) {
                colorAttachments = colors;
                depthStencilAttachment = depth;
            }
        };

        class RenderPass;
        struct RenderPassBeginInfo {
            std::shared_ptr<RenderPass> renderPass{ nullptr };
            std::shared_ptr<Framebuffer> framebuffer{ nullptr };
            Rect2D              renderArea{};
            ClearColor          clearColor{};
            ClearDepthStencil   clearDepthStencil{};
            SubpassContents subpassContents{ SubpassContents::INLINE };
        };

        class RenderPass
        {
        public:
            RenderPass() = default;
            RenderPass(const RenderPassInfo&);
            virtual ~RenderPass();
            virtual void destroy() = 0;
        protected:
            RenderPassInfo _info{};
        };

    }
}
