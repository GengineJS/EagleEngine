#pragma once

#include <graphics/DescriptorSetLayout.h>
#include <vulkan/vulkan.h>
namespace eg {
	namespace graphics {
		class VKDescriptorSetLayout: public DescriptorSetLayout {
		public:
			VKDescriptorSetLayout() = default;
			VKDescriptorSetLayout(const DescriptorSetLayoutInfo&);
			virtual ~VKDescriptorSetLayout();
			void destroy() override;

			inline const VkDescriptorSetLayout& getVkDescriptorSetLayout() const { return _layout; }
		protected:
			VkDescriptorSetLayout _layout;
		};
	}
}
