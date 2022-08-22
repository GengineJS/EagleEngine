/*
 *  VKSurface.h
 *
 *  Copyright (c) 2022 by XiangKui Zheng
 *  This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */

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
