/*
 *  VKSampler.h
 *
 *  Copyright (c) 2022 by XiangKui Zheng
 *  This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */

#pragma once

#include <graphics/Sampler.h>
#include <vulkan/vulkan.h>
namespace eg {
	namespace graphics {
		class VKSampler : public Sampler {
		public:
			VKSampler() = default;
			VKSampler(const SamplerInfo& info);
			~VKSampler() override;
			inline const VkSampler& getVkSampler() const { return _sampler; }
		protected:
			VkSampler _sampler;
		};
	}
}
