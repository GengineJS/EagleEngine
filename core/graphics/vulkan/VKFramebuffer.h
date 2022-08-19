#pragma once
#include <graphics/Framebuffer.h>
#include <vulkan/vulkan.h>
namespace eg {
	namespace graphics {
		class VKFramebuffer : public Framebuffer
		{
		public:
			VKFramebuffer() = default;
			VKFramebuffer(const FramebufferInfo&);
			~VKFramebuffer() override;
			void destroy() override;
			inline const VkFramebuffer& getVkFramebuffer() const { return _vkFramebuffer; }
		protected:
			VkFramebuffer _vkFramebuffer;
		};
	}
}
