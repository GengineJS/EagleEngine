#include "VKSurface.h"
#include <type_traits>
#include <graphics/vulkan/VKContext.h>
#include <graphics/vulkan/VKTools.h>
namespace eg {
	namespace graphics {
		VKSurface::VKSurface(const SurfaceInfo& info) : Surface(info)
		{
			VkResult err{ VK_SUCCESS };
			auto context = std::dynamic_pointer_cast<VKContext>(Context::GetContext());
#if defined(VK_USE_PLATFORM_WIN32_KHR)
			VkWin32SurfaceCreateInfoKHR surfaceCreateInfo = {};
			surfaceCreateInfo.sType = VK_STRUCTURE_TYPE_WIN32_SURFACE_CREATE_INFO_KHR;
			surfaceCreateInfo.hinstance = _info.hinstance;
			surfaceCreateInfo.hwnd = _info.hwnd;
			err = vkCreateWin32SurfaceKHR(context->getVkInstance(), &surfaceCreateInfo, nullptr, &_surface);
#elif defined(VK_USE_PLATFORM_ANDROID_KHR)
			VkAndroidSurfaceCreateInfoKHR surfaceCreateInfo = {};
			surfaceCreateInfo.sType = VK_STRUCTURE_TYPE_ANDROID_SURFACE_CREATE_INFO_KHR;
			surfaceCreateInfo.window = info.window;
			err = vkCreateAndroidSurfaceKHR(context->getVkInstance(), &surfaceCreateInfo, nullptr, &_surface);
#elif defined(VK_USE_PLATFORM_IOS_MVK)
			VkIOSSurfaceCreateInfoMVK surfaceCreateInfo = {};
			surfaceCreateInfo.sType = VK_STRUCTURE_TYPE_IOS_SURFACE_CREATE_INFO_MVK;
			surfaceCreateInfo.pNext = NULL;
			surfaceCreateInfo.flags = 0;
			surfaceCreateInfo.pView = info.pView;
			err = vkCreateIOSSurfaceMVK(context->getVkInstance(), &surfaceCreateInfo, nullptr, &_surface);
#elif defined(VK_USE_PLATFORM_MACOS_MVK)
			VkMacOSSurfaceCreateInfoMVK surfaceCreateInfo = {};
			surfaceCreateInfo.sType = VK_STRUCTURE_TYPE_MACOS_SURFACE_CREATE_INFO_MVK;
			surfaceCreateInfo.pNext = NULL;
			surfaceCreateInfo.flags = 0;
			surfaceCreateInfo.pView = info.pView;
			err = vkCreateMacOSSurfaceMVK(context->getVkInstance(), &surfaceCreateInfo, NULL, &_surface);
#elif defined(_DIRECT2DISPLAY)
			createDirect2DisplaySurface(width, height);
#elif defined(VK_USE_PLATFORM_DIRECTFB_EXT)
			VkDirectFBSurfaceCreateInfoEXT surfaceCreateInfo = {};
			surfaceCreateInfo.sType = VK_STRUCTURE_TYPE_DIRECTFB_SURFACE_CREATE_INFO_EXT;
			surfaceCreateInfo.dfb = dfb;
			surfaceCreateInfo.surface = window;
			err = vkCreateDirectFBSurfaceEXT(context->getVkInstance(), &surfaceCreateInfo, nullptr, &_surface);
#elif defined(VK_USE_PLATFORM_WAYLAND_KHR)
			VkWaylandSurfaceCreateInfoKHR surfaceCreateInfo = {};
			surfaceCreateInfo.sType = VK_STRUCTURE_TYPE_WAYLAND_SURFACE_CREATE_INFO_KHR;
			surfaceCreateInfo.display = display;
			surfaceCreateInfo.surface = window;
			err = vkCreateWaylandSurfaceKHR(context->getVkInstance(), &surfaceCreateInfo, nullptr, &_surface);
#elif defined(VK_USE_PLATFORM_XCB_KHR)
			VkXcbSurfaceCreateInfoKHR surfaceCreateInfo = {};
			surfaceCreateInfo.sType = VK_STRUCTURE_TYPE_XCB_SURFACE_CREATE_INFO_KHR;
			surfaceCreateInfo.connection = connection;
			surfaceCreateInfo.window = window;
			err = vkCreateXcbSurfaceKHR(context->getVkInstance(), &surfaceCreateInfo, nullptr, &_surface);
#elif defined(VK_USE_PLATFORM_HEADLESS_EXT)
			VkHeadlessSurfaceCreateInfoEXT surfaceCreateInfo = {};
			surfaceCreateInfo.sType = VK_STRUCTURE_TYPE_HEADLESS_SURFACE_CREATE_INFO_EXT;
			PFN_vkCreateHeadlessSurfaceEXT fpCreateHeadlessSurfaceEXT = (PFN_vkCreateHeadlessSurfaceEXT)vkGetInstanceProcAddr(instance, "vkCreateHeadlessSurfaceEXT");
			if (!fpCreateHeadlessSurfaceEXT) {
				exitFatal("Could not fetch function pointer for the headless extension!", -1);
			}
			err = fpCreateHeadlessSurfaceEXT(context->getVkInstance(), &surfaceCreateInfo, nullptr, &_surface);
#endif
			if (err != VK_SUCCESS) {
				exitFatal("Could not create surface!", err);
			}
		}

		VKSurface::~VKSurface()
		{
			auto context = std::dynamic_pointer_cast<VKContext>(Context::GetContext());
			vkDestroySurfaceKHR(context->getVkInstance(), _surface, nullptr);
		}
	}
}
