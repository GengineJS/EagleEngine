#pragma once
#include <graphics/DescriptorSet.h>
#include <vulkan/vulkan.h>
namespace eg {
	namespace graphics {
		class VKDescriptorSet: public DescriptorSet
		{
		public:
			VKDescriptorSet() = default;
			VKDescriptorSet(const DescriptorSetInfo& info);
			virtual ~VKDescriptorSet();
			void destroy() override;
			void create() override;
			inline const std::vector<VkDescriptorSet>& getVkDescriptorSets() const { return _descriptorSets; }
			void flush() override;
		private:
			std::vector<VkDescriptorSet> _descriptorSets{};
		};
	}
}
