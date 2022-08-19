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
