/*
 *  VKRenderPass.h
 *
 *  Copyright (c) 2022 by XiangKui Zheng
 *  This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */

#pragma once
#include <graphics/RenderPass.h>
#include <vulkan/vulkan.h>
namespace eg {
	namespace graphics {
		class VKRenderPass: public RenderPass
		{
		public:
			VKRenderPass() = default;
			VKRenderPass(const RenderPassInfo&);
			~VKRenderPass() override;
			void destroy() override;
			const VkRenderPass& getVkRenderPass() const { return _vkRenderPass; }
		protected:
			VkRenderPass _vkRenderPass;
		};
	}
}
