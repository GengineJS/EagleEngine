/*
 *  VKSwapchain.cpp
 *
 *  Copyright (c) 2022 by XiangKui Zheng
 *  This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */

#include <graphics/vulkan/VKSwapchain.h>
#include <graphics/vulkan/VKContext.h>
#include <graphics/vulkan/VKDevice.h>
#include <graphics/vulkan/VKTexture.h>
#include <graphics/vulkan/VKCommandBuffer.h>
namespace eg {
	namespace graphics {
		VKSwapchain::VKSwapchain(): Swapchain()
		{
			auto context = std::dynamic_pointer_cast<VKContext>(Context::GetContext());
			VkDevice device = std::dynamic_pointer_cast<VKDevice>(context->getDevice())->getLogicDevice();
			_fpGetPhysicalDeviceSurfaceSupportKHR = reinterpret_cast<PFN_vkGetPhysicalDeviceSurfaceSupportKHR>(vkGetInstanceProcAddr(context->getVkInstance(), "vkGetPhysicalDeviceSurfaceSupportKHR"));
			_fpGetPhysicalDeviceSurfaceCapabilitiesKHR = reinterpret_cast<PFN_vkGetPhysicalDeviceSurfaceCapabilitiesKHR>(vkGetInstanceProcAddr(context->getVkInstance(), "vkGetPhysicalDeviceSurfaceCapabilitiesKHR"));
			_fpGetPhysicalDeviceSurfaceFormatsKHR = reinterpret_cast<PFN_vkGetPhysicalDeviceSurfaceFormatsKHR>(vkGetInstanceProcAddr(context->getVkInstance(), "vkGetPhysicalDeviceSurfaceFormatsKHR"));
			_fpGetPhysicalDeviceSurfacePresentModesKHR = reinterpret_cast<PFN_vkGetPhysicalDeviceSurfacePresentModesKHR>(vkGetInstanceProcAddr(context->getVkInstance(), "vkGetPhysicalDeviceSurfacePresentModesKHR"));

			_fpCreateSwapchainKHR = reinterpret_cast<PFN_vkCreateSwapchainKHR>(vkGetDeviceProcAddr(device, "vkCreateSwapchainKHR"));
			_fpDestroySwapchainKHR = reinterpret_cast<PFN_vkDestroySwapchainKHR>(vkGetDeviceProcAddr(device, "vkDestroySwapchainKHR"));
			_fpGetSwapchainImagesKHR = reinterpret_cast<PFN_vkGetSwapchainImagesKHR>(vkGetDeviceProcAddr(device, "vkGetSwapchainImagesKHR"));
			_fpAcquireNextImageKHR = reinterpret_cast<PFN_vkAcquireNextImageKHR>(vkGetDeviceProcAddr(device, "vkAcquireNextImageKHR"));
			_fpQueuePresentKHR = reinterpret_cast<PFN_vkQueuePresentKHR>(vkGetDeviceProcAddr(device, "vkQueuePresentKHR"));
			// Create synchronization objects
			VkSemaphoreCreateInfo semaphoreInfo{};
			semaphoreInfo.sType = VK_STRUCTURE_TYPE_SEMAPHORE_CREATE_INFO;
			// Create a semaphore used to synchronize image presentation
			// Ensures that the image is displayed before we start submitting new commands to the queue
			VK_CHECK_RESULT(vkCreateSemaphore(device, &semaphoreInfo, nullptr, &_semaphores.presentComplete));
			// Create a semaphore used to synchronize command submission
			// Ensures that the image is not presented until all commands have been submitted and executed
			VK_CHECK_RESULT(vkCreateSemaphore(device, &semaphoreInfo, nullptr, &_semaphores.renderComplete));
		}

		VKSwapchain::~VKSwapchain()
		{
			auto context = Context::GetContext();
			auto device = std::dynamic_pointer_cast<VKDevice>(context->getDevice());
			VkDevice logicDevice = device->getLogicDevice();
			_fpDestroySwapchainKHR(logicDevice, _vkSwapChain, nullptr);
			_vkSwapChain = VK_NULL_HANDLE;
			destroy();
		}

		void VKSwapchain::_initSurface(const AppHandler& handler) {
			auto context = Context::GetContext();
			auto device = std::dynamic_pointer_cast<VKDevice>(context->getDevice());
			SurfaceInfo surInfo{};
			// Create the os-specific surface
#if defined(VK_USE_PLATFORM_WIN32_KHR)
			surInfo.hinstance = reinterpret_cast<HINSTANCE>(handler.windowInstance);
			surInfo.hwnd = reinterpret_cast<HWND>(handler.window);
#elif defined(VK_USE_PLATFORM_ANDROID_KHR)
			surInfo.window = handler.androidApp->window;
#elif defined(VK_USE_PLATFORM_DIRECTFB_EXT)
			surInfo.dfb = handler.dfb;
			surInfo.surface = handler.window;
#elif defined(VK_USE_PLATFORM_WAYLAND_KHR)
			surInfo.display = handler.display;
			surInfo.surface = handler.surface;
#elif defined(VK_USE_PLATFORM_XCB_KHR)
			surInfo.connection = handler.connection;
			surInfo.window = handler.window;
#elif (defined(VK_USE_PLATFORM_IOS_MVK) || defined(VK_USE_PLATFORM_MACOS_MVK))
			surInfo.pView = handler.view;
#elif (defined(_DIRECT2DISPLAY) || defined(VK_USE_PLATFORM_HEADLESS_EXT))
			surInfo.width = handler.width;
			surInfo.height = handler.height;
#endif
			_surface = std::make_shared<VKSurface>(surInfo);
			VkSurfaceKHR vkSurface = std::dynamic_pointer_cast<VKSurface>(_surface)->getVkSurfaceKHR();
			VkPhysicalDevice physicalDevice = std::dynamic_pointer_cast<VKDevice>(context->getDevice())->getPhysicalDevice();
			// Get available queue family properties
			uint32_t queueCount;
			vkGetPhysicalDeviceQueueFamilyProperties(physicalDevice, &queueCount, NULL);
			assert(queueCount >= 1);

			std::vector<VkQueueFamilyProperties> queueProps(queueCount);
			vkGetPhysicalDeviceQueueFamilyProperties(physicalDevice, &queueCount, queueProps.data());

			// Iterate over each queue to learn whether it supports presenting:
			// Find a queue with present support
			// Will be used to present the swap chain images to the windowing system
			std::vector<VkBool32> supportsPresent(queueCount);
			for (uint32_t i = 0; i < queueCount; i++)
			{
				_fpGetPhysicalDeviceSurfaceSupportKHR(physicalDevice, i, vkSurface, &supportsPresent[i]);
			}

			// Search for a graphics and a present queue in the array of queue
			// families, try to find one that supports both
			uint32_t graphicsQueueIndex = UINT32_MAX;
			uint32_t presentQueueIndex = UINT32_MAX;
			for (uint32_t i = 0; i < queueCount; i++)
			{
				if ((queueProps[i].queueFlags & VK_QUEUE_GRAPHICS_BIT) != 0)
				{
					if (graphicsQueueIndex == UINT32_MAX)
					{
						graphicsQueueIndex = i;
					}

					if (supportsPresent[i] == VK_TRUE)
					{
						graphicsQueueIndex = i;
						presentQueueIndex = i;
						break;
					}
				}
			}
			if (presentQueueIndex == UINT32_MAX)
			{
				// If there's no queue that supports both present and graphics
				// try to find a separate present queue
				for (uint32_t i = 0; i < queueCount; ++i)
				{
					if (supportsPresent[i] == VK_TRUE)
					{
						presentQueueIndex = i;
						break;
					}
				}
			}

			// Exit if either a graphics or a presenting queue hasn't been found
			if (graphicsQueueIndex == UINT32_MAX || presentQueueIndex == UINT32_MAX)
			{
				exitFatal("Could not find a graphics and/or presenting queue!", -1);
			}

			// todo : Add support for separate graphics and presenting queue
			if (graphicsQueueIndex != presentQueueIndex)
			{
				exitFatal("Separate graphics and presenting queues are not supported yet!", -1);
			}

			_presentQueueIdx = graphicsQueueIndex;
			device->setPresentQueueIdx(_presentQueueIdx);
			// Get list of supported surface formats
			uint32_t formatCount;
			VK_CHECK_RESULT(_fpGetPhysicalDeviceSurfaceFormatsKHR(physicalDevice, vkSurface, &formatCount, NULL));
			assert(formatCount > 0);

			std::vector<VkSurfaceFormatKHR> surfaceFormats(formatCount);
			VK_CHECK_RESULT(_fpGetPhysicalDeviceSurfaceFormatsKHR(physicalDevice, vkSurface, &formatCount, surfaceFormats.data()));

			// If the surface format list only includes one entry with VK_FORMAT_UNDEFINED,
			// there is no preferred format, so we assume VK_FORMAT_B8G8R8A8_UNORM
			if ((formatCount == 1) && (surfaceFormats[0].format == VK_FORMAT_UNDEFINED))
			{
				_format = Format::B8G8R8A8_UNORM;
				_colorSpace = static_cast<ColorSpace>(surfaceFormats[0].colorSpace);
			}
			else
			{
				// iterate over the list of available surface format and
				// check for the presence of VK_FORMAT_B8G8R8A8_UNORM
				bool found_B8G8R8A8_UNORM = false;
				for (auto&& surfaceFormat : surfaceFormats)
				{
					if (surfaceFormat.format == VK_FORMAT_B8G8R8A8_UNORM)
					{
						_format = static_cast<Format>(surfaceFormat.format);
						_colorSpace = static_cast<ColorSpace>(surfaceFormat.colorSpace);
						found_B8G8R8A8_UNORM = true;
						break;
					}
				}

				// in case VK_FORMAT_B8G8R8A8_UNORM is not available
				// select the first available color format
				if (!found_B8G8R8A8_UNORM)
				{
					_format = static_cast<Format>(surfaceFormats[0].format);
					_colorSpace = static_cast<ColorSpace>(surfaceFormats[0].colorSpace);
				}
			}
		}

		void VKSwapchain::create(const AppHandler& handler)
		{
		    // Store the current swap chain handle so we can use it later on to ease up recreation
		    // TODO: There is a problem with android reusing swapchain
            VkSwapchainKHR oldSwapchain = _surface ? _vkSwapChain : VK_NULL_HANDLE;
			if(!_surface) _initSurface(handler);

			auto context = Context::GetContext();
			VkSurfaceKHR vkSurface = std::dynamic_pointer_cast<VKSurface>(_surface)->getVkSurfaceKHR();
			auto device = std::dynamic_pointer_cast<VKDevice>(context->getDevice());
			VkDevice logicDevice = device->getLogicDevice();
			VkPhysicalDevice physicalDevice = device->getPhysicalDevice();
			// Get physical device surface properties and formats
			VkSurfaceCapabilitiesKHR surfCaps;
			VK_CHECK_RESULT(_fpGetPhysicalDeviceSurfaceCapabilitiesKHR(physicalDevice, vkSurface, &surfCaps));

			// Get available present modes
			uint32_t presentModeCount;
			VK_CHECK_RESULT(_fpGetPhysicalDeviceSurfacePresentModesKHR(physicalDevice, vkSurface, &presentModeCount, NULL));
			assert(presentModeCount > 0);

			std::vector<VkPresentModeKHR> presentModes(presentModeCount);
			VK_CHECK_RESULT(_fpGetPhysicalDeviceSurfacePresentModesKHR(physicalDevice, vkSurface, &presentModeCount, presentModes.data()));

			VkExtent2D swapchainExtent{};
			// If width (and height) equals the special value 0xFFFFFFFF, the size of the surface will be set by the swapchain
			if (surfCaps.currentExtent.width == std::numeric_limits<uint32_t>::max())
			{
				// If the surface size is undefined, the size is set to
				// the size of the images requested.
				swapchainExtent.width = context->getContextInfo().width;
				swapchainExtent.height = context->getContextInfo().height;
			}
			else
			{
				// If the surface size is defined, the swap chain size must match
				swapchainExtent = surfCaps.currentExtent;
				context->getContextInfo().width = surfCaps.currentExtent.width;
				context->getContextInfo().height = surfCaps.currentExtent.height;
			}
			_width = swapchainExtent.width;
			_height = swapchainExtent.height;

			// Select a present mode for the swapchain

			// The VK_PRESENT_MODE_FIFO_KHR mode must always be present as per spec
			// This mode waits for the vertical blank ("v-sync")
			VkPresentModeKHR swapchainPresentMode{ VK_PRESENT_MODE_FIFO_KHR };

			// If v-sync is not requested, try to find a mailbox mode
			// It's the lowest latency non-tearing present mode available
			if (!context->getContextInfo().vsync)
			{
				for (size_t i = 0; i < presentModeCount; i++)
				{
					if (presentModes[i] == VK_PRESENT_MODE_MAILBOX_KHR)
					{
						swapchainPresentMode = VK_PRESENT_MODE_MAILBOX_KHR;
						break;
					}
					if (presentModes[i] == VK_PRESENT_MODE_IMMEDIATE_KHR)
					{
						swapchainPresentMode = VK_PRESENT_MODE_IMMEDIATE_KHR;
					}
				}
			}

			// Determine the number of images
			uint32_t desiredNumberOfSwapchainImages = surfCaps.minImageCount + 1;
			if ((surfCaps.maxImageCount > 0) && (desiredNumberOfSwapchainImages > surfCaps.maxImageCount))
			{
				desiredNumberOfSwapchainImages = surfCaps.maxImageCount;
			}

			// Find the transformation of the surface
			VkSurfaceTransformFlagsKHR preTransform;
			if (surfCaps.supportedTransforms & VK_SURFACE_TRANSFORM_IDENTITY_BIT_KHR)
			{
				// We prefer a non-rotated transform
				preTransform = VK_SURFACE_TRANSFORM_IDENTITY_BIT_KHR;
			}
			else
			{
				preTransform = surfCaps.currentTransform;
			}

			// Find a supported composite alpha format (not all devices support alpha opaque)
			VkCompositeAlphaFlagBitsKHR compositeAlpha{ VK_COMPOSITE_ALPHA_OPAQUE_BIT_KHR };
			// Simply select the first composite alpha format available
			std::vector<VkCompositeAlphaFlagBitsKHR> compositeAlphaFlags = {
				VK_COMPOSITE_ALPHA_OPAQUE_BIT_KHR,
				VK_COMPOSITE_ALPHA_PRE_MULTIPLIED_BIT_KHR,
				VK_COMPOSITE_ALPHA_POST_MULTIPLIED_BIT_KHR,
				VK_COMPOSITE_ALPHA_INHERIT_BIT_KHR,
			};
			for (auto& compositeAlphaFlag : compositeAlphaFlags) {
				if (surfCaps.supportedCompositeAlpha & compositeAlphaFlag) {
					compositeAlpha = compositeAlphaFlag;
					break;
				};
			}

			VkSwapchainCreateInfoKHR swapchainCI = {};
			swapchainCI.sType = VK_STRUCTURE_TYPE_SWAPCHAIN_CREATE_INFO_KHR;
			swapchainCI.surface = vkSurface;
			swapchainCI.minImageCount = desiredNumberOfSwapchainImages;
			swapchainCI.imageFormat = static_cast<VkFormat>(_format);
			swapchainCI.imageColorSpace = static_cast<VkColorSpaceKHR>(_colorSpace);
			swapchainCI.imageExtent = { swapchainExtent.width, swapchainExtent.height };
			swapchainCI.imageUsage = VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT;
			swapchainCI.preTransform = (VkSurfaceTransformFlagBitsKHR)preTransform;
			swapchainCI.imageArrayLayers = 1;
			swapchainCI.imageSharingMode = VK_SHARING_MODE_EXCLUSIVE;
			swapchainCI.queueFamilyIndexCount = 0;
			swapchainCI.presentMode = swapchainPresentMode;
			// Setting oldSwapChain to the saved handle of the previous swapchain aids in resource reuse and makes sure that we can still present already acquired images
			swapchainCI.oldSwapchain = oldSwapchain;
			// Setting clipped to VK_TRUE allows the implementation to discard rendering outside of the surface area
			swapchainCI.clipped = VK_TRUE;
			swapchainCI.compositeAlpha = compositeAlpha;

			// Enable transfer source on swap chain images if supported
			if (surfCaps.supportedUsageFlags & VK_IMAGE_USAGE_TRANSFER_SRC_BIT) {
				swapchainCI.imageUsage |= VK_IMAGE_USAGE_TRANSFER_SRC_BIT;
			}

			// Enable transfer destination on swap chain images if supported
			if (surfCaps.supportedUsageFlags & VK_IMAGE_USAGE_TRANSFER_DST_BIT) {
				swapchainCI.imageUsage |= VK_IMAGE_USAGE_TRANSFER_DST_BIT;
			}

			VK_CHECK_RESULT(_fpCreateSwapchainKHR(logicDevice, &swapchainCI, nullptr, &_vkSwapChain));

			if (oldSwapchain != VK_NULL_HANDLE) {
				_destroyTextures();
				_fpDestroySwapchainKHR(logicDevice, oldSwapchain, nullptr);
			}
			uint32_t imgCont = 0;
			VK_CHECK_RESULT(_fpGetSwapchainImagesKHR(logicDevice, _vkSwapChain, &imgCont, nullptr));
			std::vector<VkImage> images{};
			images.resize(imgCont);
			VK_CHECK_RESULT(_fpGetSwapchainImagesKHR(logicDevice, _vkSwapChain, &imgCont, images.data()));
			TextureInfo texInfo{};
			for (auto img : images) {
				texInfo.format = _format;
				texInfo.width = swapchainExtent.width;
				texInfo.height = swapchainExtent.height;
				auto tex = device->createTexture(texInfo, img);
				std::dynamic_pointer_cast<VKTexture>(tex)->_isSwapchain = true;
				_textures.emplace_back(tex);
			}
			VkFenceCreateInfo fenceInfo{};
			fenceInfo.sType = VK_STRUCTURE_TYPE_FENCE_CREATE_INFO;
			fenceInfo.flags = VK_FENCE_CREATE_SIGNALED_BIT;
			_waitFences.resize(_textures.size());
			for (auto& fence : _waitFences) {
				VK_CHECK_RESULT(vkCreateFence(logicDevice, &fenceInfo, nullptr, &fence));
			}
		}
		void VKSwapchain::destroy()
		{
			Swapchain::destroy();
			_waitFences.clear();
			_currTextureIdx = 0;
			_currFrameIdx = 0;
		}
		void VKSwapchain::acquireFrame()
		{
			auto context = Context::GetContext();
			auto device = std::dynamic_pointer_cast<VKDevice>(context->getDevice());
			VkDevice logicDevice = device->getLogicDevice();
			vkWaitForFences(logicDevice, 1, &_waitFences[_currFrameIdx], VK_TRUE, UINT64_MAX);
			VkResult result = _fpAcquireNextImageKHR(logicDevice, _vkSwapChain, UINT64_MAX, _semaphores.presentComplete, VK_NULL_HANDLE, &_currTextureIdx);

			if (result == VK_ERROR_OUT_OF_DATE_KHR) {
				return;
			}
			else if (result != VK_SUCCESS && result != VK_SUBOPTIMAL_KHR) {
				throw std::runtime_error("failed to acquire swap chain image!");
			}
			vkResetFences(logicDevice, 1, &_waitFences[_currFrameIdx]);
		}
		// TODO: present command buffer list
		void VKSwapchain::presentFrame(std::shared_ptr<CommandBuffer> cmdBuff)
		{
			auto context = Context::GetContext();
			auto device = std::dynamic_pointer_cast<VKDevice>(context->getDevice());
			VkDevice logicDevice = device->getLogicDevice();
			VkSubmitInfo submitInfo{};
			submitInfo.sType = VK_STRUCTURE_TYPE_SUBMIT_INFO;

			VkSemaphore waitSemaphores[] = { _semaphores.presentComplete };
			VkPipelineStageFlags waitStages[] = { VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT };
			submitInfo.waitSemaphoreCount = 1;
			submitInfo.pWaitSemaphores = waitSemaphores;
			submitInfo.pWaitDstStageMask = waitStages;

			submitInfo.commandBufferCount = 1;
			auto vkCmdBuff = std::dynamic_pointer_cast<VKCommandBuffer>(cmdBuff);
			submitInfo.pCommandBuffers = &vkCmdBuff->getVkCmdBuffer();

			VkSemaphore signalSemaphores[] = { _semaphores.renderComplete };
			submitInfo.signalSemaphoreCount = 1;
			submitInfo.pSignalSemaphores = signalSemaphores;

			if (vkQueueSubmit(device->getGraphicQueue(), 1, &submitInfo, _waitFences[_currFrameIdx]) != VK_SUCCESS) {
				throw std::runtime_error("failed to submit draw command buffer!");
			}

			VkPresentInfoKHR presentInfo{};
			presentInfo.sType = VK_STRUCTURE_TYPE_PRESENT_INFO_KHR;

			presentInfo.waitSemaphoreCount = 1;
			presentInfo.pWaitSemaphores = signalSemaphores;

			VkSwapchainKHR swapChains[] = { _vkSwapChain };
			presentInfo.swapchainCount = 1;
			presentInfo.pSwapchains = swapChains;

			presentInfo.pImageIndices = &_currTextureIdx;

			VkResult result = vkQueuePresentKHR(device->getGraphicQueue(), &presentInfo);

			if (result == VK_ERROR_OUT_OF_DATE_KHR || result == VK_SUBOPTIMAL_KHR) {
			}
			else if (result != VK_SUCCESS) {
				throw std::runtime_error("failed to present swap chain image!");
			}

			_currFrameIdx = (_currFrameIdx + 1) % static_cast<int32_t>(_textures.size());
		}
	}
}
