/*
 *  PipelineLayout.h
 *
 *  Copyright (c) 2022 by XiangKui Zheng
 *  This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */

#pragma once
#include <graphics/DescriptorSetLayout.h>
#include <graphics/GraphicsDef.h>
#include <vector>
namespace eg {
    namespace graphics {
        struct PushConstantRange
        {
            ShaderStageFlag stageFlag{ShaderStageFlag::NONE};
            uint32_t offset{ 0 };
            uint32_t size{0};
        };

        struct PipelineLayoutInfo
        {
            std::vector<DescriptorSetLayout*> pSetLayouts{};
            std::vector<PushConstantRange> pushConstantRanges{};

        };

        class PipelineLayout {
        public:
            PipelineLayout() = default;
            PipelineLayout(const PipelineLayoutInfo& info);
            virtual ~PipelineLayout();
            virtual void destroy() = 0;
            inline const PipelineLayoutInfo& getPipelineLayoutInfo() const { return _info; }
        protected:
            PipelineLayoutInfo _info{};
        };
    }
}
