/*
 *  VKDescriptorSet.h
 *
 *  Copyright (c) 2022 by XiangKui Zheng
 *  This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */

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
