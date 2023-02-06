/*
 *  VKDescriptorSetLayout.h
 *
 *  Copyright (c) 2022 by XiangKui Zheng
 *  This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */

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

			inline const VkDescriptorSetLayout& getVkDescriptorSetLayout() const { return layout; }
		protected:
			VkDescriptorSetLayout layout;
		};
	}
}
