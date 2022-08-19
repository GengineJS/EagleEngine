#pragma once
#include "graphics/ShaderStage.h"
#include "graphics/VertexInput.h"
#include "graphics/PipelineLayout.h"
#include "graphics/GraphicsDef.h"
#include "graphics/Sampler.h"
#include <graphics/RenderPass.h>
namespace eg {
    namespace graphics {
        struct RasterizationStateInfo
        {
            bool isDepthClamp{ false };
            bool isDiscard{ false };
            PolygonMode polygonMode{ PolygonMode::FILL };
            CullModeFlag cullMode{ CullModeFlag::BACK };
            FrontFace frontFace{ FrontFace::COUNTER_CLOCKWISE };
            bool isDepthBias{ false };
            float depthBiasConstant{ 0.0f };
            float depthBiasClamp{ 0.0f };
            float depthBiasSlopeFactor{ 0.0f };
            float lineWidth{ 1.0f };
        };

        struct ViewportInfo {
            Viewport viewport;
            uint32_t first{ 0 };
            uint32_t count{ 1 };
        };

        struct ScissorInfo {
            Rect2D scissor;
            uint32_t first{ 0 };
            uint32_t count{ 1 };
        };

        struct MultiSampleStateInfo {
            SampleCountFlag rasterizationSamples{ SampleCountFlag::COUNT_1 };
            bool isSampleShading{ false };
            std::shared_ptr<SampleMask> sampleMask{ nullptr };
            float minSampleShading{1.f};
            bool isAlphaToCoverage{ false };
            bool isAlphaToOne{ false };
        };

        struct StencilOpState {
            StencilOp    failOp;
            StencilOp    passOp;
            StencilOp    depthFailOp;
            CompareOp    compareOp;
            uint32_t     compareMask;
            uint32_t     writeMask;
            uint32_t     reference;
        };

        struct DepthStencilStateInfo {
            bool isDepthTest{ true };
            bool isDepthWrite{ true };
            CompareOp depthCompare{ CompareOp::LESS_OR_EQUAL };
            bool isDepthBoundsTest{ false };
            bool isStencilTest{ false };
            StencilOpState front;
            StencilOpState back;
            float minDepthBounds{ 0.0f };
            float maxDepthBounds{ 1.0f };
        };

        struct ColorBlendAttachmentState {
            bool                 blendEnable{false};
            BlendFactor          srcColorBlend;
            BlendFactor          dstColorBlend;
            BlendOp              colorBlendOp;
            BlendFactor          srcAlphaBlend;
            BlendFactor          dstAlphaBlend;
            BlendOp              alphaBlendOp;
            ColorComponentFlag   colorWriteMask{ ColorComponentFlag::R | ColorComponentFlag::G | ColorComponentFlag::B | ColorComponentFlag::A };
        };

        struct ColorBlendStateInfo {
            bool isLogicOp{ false };
            LogicOp logicOp;
            std::vector<ColorBlendAttachmentState> attachments;
            float blendConstants[4] = {0.f, 0.f, 0.f, 0.f};
        };

        struct DynamicStateInfo {
            std::vector<DynamicState> dynamicStates{
                DynamicState::VIEWPORT,
                DynamicState::SCISSOR
            };
        };
        struct GraphicsPipelineInfo
        {
            std::shared_ptr<ShaderStage> shader{ nullptr };
            std::shared_ptr<PipelineLayout> pipelineLayout{ nullptr };
            std::shared_ptr<VertexInput> vertexInput{nullptr};
            RasterizationStateInfo raster{};
            MultiSampleStateInfo multiSample{};
            DepthStencilStateInfo depthStencil{};
            ColorBlendStateInfo colorBlend{};
            DynamicStateInfo dynamicInfo{};
            std::shared_ptr<RenderPass> renderPass{nullptr};
            uint32_t subpass{ 0 };
            int32_t basePipelineIdx{-1};
            class std::shared_ptr<class GraphicsPipeline> basePipeline{ nullptr };
        };

        class GraphicsPipeline
        {
        public:
            GraphicsPipeline() = default;
            GraphicsPipeline(const GraphicsPipelineInfo& info);
            virtual ~GraphicsPipeline();
            virtual void destroy() = 0;
            inline GraphicsPipelineInfo& getGraphicsPipelineInfo() { return _info; }
        protected:
            GraphicsPipelineInfo _info{};
        };
    }
}
