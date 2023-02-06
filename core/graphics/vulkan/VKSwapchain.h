/*
 *  VKSwapchain.h
 *
 *  Copyright (c) 2022 by XiangKui Zheng
 *  This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */

#pragma once
#include <graphics/Swapchain.h>
#include <stdlib.h>
#include <string>
#include <assert.h>
#include <stdio.h>
#include <vector>

#include <vulkan/vulkan.h>
#include "graphics/vulkan/VKTools.h"
#include <graphics/vulkan/VKSurface.h>
#ifdef __ANDROID__
#include "graphics/vulkan/VKAndroid.h"
#endif
namespace eg {
	namespace graphics {
		class VKSwapchain : public Swapchain
		{
		public:
			VKSwapchain();
			~VKSwapchain() override;
			void create(const WindowHandler& handler) override;
			void destroy() override;
			inline uint32_t getPresentQueue() const { return _presentQueueIdx; }
			void acquireFrame() override;
			void presentFrame(const std::unique_ptr<CommandBuffer>& cmdBuff) override;
		protected:
			VkSwapchainKHR _vkSwapChain{ VK_NULL_HANDLE };
			uint32_t _presentQueueIdx{ UINT32_MAX };
			PFN_vkGetPhysicalDeviceSurfaceSupportKHR _fpGetPhysicalDeviceSurfaceSupportKHR;
			PFN_vkGetPhysicalDeviceSurfaceCapabilitiesKHR _fpGetPhysicalDeviceSurfaceCapabilitiesKHR;
			PFN_vkGetPhysicalDeviceSurfaceFormatsKHR _fpGetPhysicalDeviceSurfaceFormatsKHR;
			PFN_vkGetPhysicalDeviceSurfacePresentModesKHR _fpGetPhysicalDeviceSurfacePresentModesKHR;
			PFN_vkCreateSwapchainKHR _fpCreateSwapchainKHR;
			PFN_vkDestroySwapchainKHR _fpDestroySwapchainKHR;
			PFN_vkGetSwapchainImagesKHR _fpGetSwapchainImagesKHR;
			PFN_vkAcquireNextImageKHR _fpAcquireNextImageKHR;
			PFN_vkQueuePresentKHR _fpQueuePresentKHR;
			// Synchronization semaphores
			struct {
				// Swap chain image presentation
				VkSemaphore presentComplete;
				// Command buffer submission and execution
				VkSemaphore renderComplete;
			} _semaphores;
			std::vector<VkFence> _waitFences{};
			uint32_t _currTextureIdx{ 0 };
			void _initSurface(const WindowHandler& handler) override;
		};
	}
}
