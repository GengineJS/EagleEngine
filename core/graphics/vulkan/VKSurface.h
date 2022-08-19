#pragma once
#include <graphics/Surface.h>
#include <vulkan/vulkan.h>
namespace eg {
	namespace graphics {
		class VKSurface : public Surface {
		public:
			VKSurface() = default;
			VKSurface(const SurfaceInfo& info);
			~VKSurface() override;
			inline const VkSurfaceKHR& getVkSurfaceKHR()const { return _surface; }
		protected:
			VkSurfaceKHR _surface;
		};
	}
}
