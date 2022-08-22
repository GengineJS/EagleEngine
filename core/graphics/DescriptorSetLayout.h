/*
 *  DescriptorSetLayout.h
 *
 *  Copyright (c) 2022 by XiangKui Zheng
 *  This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */

#pragma once
#include "graphics/GraphicsDef.h"
#include "graphics/Sampler.h"
#include <vector>
namespace eg {
    namespace graphics {
        struct DescriptorSetLayoutBinding {
            uint32_t binding;
            ShaderStageFlag stage;
            DescriptorType type;
            uint32_t descriptorCount{1};
            // TODO
            std::vector<std::shared_ptr<Sampler>> immutableSamplers{};
        };

        struct DescriptorSetLayoutInfo
        {
            std::vector<DescriptorSetLayoutBinding> bindings{};
            DescriptorSetLayoutCreateFlag flag{};
        };

        class DescriptorSetLayout
        {
        public:
            DescriptorSetLayout() = default;
            DescriptorSetLayout(const DescriptorSetLayoutInfo&);
            virtual ~DescriptorSetLayout();
            virtual void destroy() = 0;
            inline const DescriptorSetLayoutInfo& getDescriptorSetLayoutInfo() const { return _info; }
        protected:
            DescriptorSetLayoutInfo _info{};
        };
    }
}
