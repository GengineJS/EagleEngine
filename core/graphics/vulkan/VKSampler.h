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
