/*
 *  VKDescriptorPool.h
 *
 *  Copyright (c) 2022 by XiangKui Zheng
 *  This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */

#pragma once
#include <graphics/DescriptorPool.h>
#include <vulkan/vulkan.h>
namespace eg {
	namespace graphics {
		class VKDescriptorPool: public DescriptorPool
		{
		public:
			VKDescriptorPool() = default;
			VKDescriptorPool(const DescriptorPoolInfo& info);
			~VKDescriptorPool() override;
			void destroy() override;
			inline const VkDescriptorPool& getVkDescriptorPool() const { return _pool; }
		protected:
			VkDescriptorPool _pool;
		};
	}
}
