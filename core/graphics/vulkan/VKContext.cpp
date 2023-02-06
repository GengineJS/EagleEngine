/*
 *  VKContext.cpp
 *
 *  Copyright (c) 2022 by XiangKui Zheng
 *  This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */

#include <graphics/Context.h>
#include <graphics/vulkan/VKContext.h>
#include <iostream>
#include <graphics/vulkan/VKDebug.h>
#include <graphics/vulkan/VKTools.h>
#include <graphics/vulkan/VKDevice.h>
#include <algorithm>
#include <engine/EagleEngine.h>
#if defined(USE_PLATFORM_ANDROID_KHR)
	#include <graphics/vulkan/VKAndroid.h>
#endif
namespace eg {
	namespace graphics {
		VKContext::~VKContext()
		{
			// You need to release swapchain and commandBuffer in the subclass, otherwise the release will fail
			_commandBuffers.clear();
			_swapchain.reset();
			
#if defined(USE_PLATFORM_ANDROID_KHR)
			android::freeVulkanLibrary();
#endif
			// vkDestroyInstance(_instance, nullptr);
		}
		const std::unique_ptr<Device>& VKContext::createDevice()
		{
			_device = std::make_unique<VKDevice>();
			_device->initialize();
			return _device;
		}

		void VKContext::resize(uint32_t w, uint32_t h)
		{
			vkDeviceWaitIdle(dynamic_cast<VKDevice*>(_device.get())->getLogicDevice());
			Context::resize(w, h);
		}

		VKContext::VKContext() : Context() {}

		void VKContext::initialize()
		{
			const auto& info = EagleEngine::Get()->getEngineInfo();
			VkApplicationInfo appInfo{};
			appInfo.sType = VK_STRUCTURE_TYPE_APPLICATION_INFO;
            appInfo.pNext = nullptr;
			appInfo.apiVersion = VK_API_VERSION_1_0;
			appInfo.applicationVersion = MAKE_FLOAT_VERSION(info.applicationVersion);
			appInfo.pApplicationName = info.applicationName.c_str();
			appInfo.engineVersion = MAKE_FLOAT_VERSION(ENGINE_VERSION);
			appInfo.pEngineName = ENGINE_NAME;

			_enableExtensions.emplace_back(VK_KHR_SURFACE_EXTENSION_NAME);

			// Enable surface extensions depending on os
#if defined(USE_PLATFORM_WIN32)
			_enableExtensions.emplace_back(VK_KHR_WIN32_SURFACE_EXTENSION_NAME);
#elif defined(USE_PLATFORM_ANDROID_KHR)
			_enableExtensions.emplace_back(VK_KHR_ANDROID_SURFACE_EXTENSION_NAME);
			// Vulkan library is loaded dynamically on Android
			bool libLoaded = android::loadVulkanLibrary();
			assert(libLoaded);
#elif defined(USE_PLATFORM_IOS_MVK)
			_enableExtensions.emplace_back(VK_MVK_IOS_SURFACE_EXTENSION_NAME);
#elif defined(USE_PLATFORM_MACOS_MVK)
			_enableExtensions.emplace_back(VK_MVK_MACOS_SURFACE_EXTENSION_NAME);
#endif
			uint32_t extCount{ 0 };
			vkEnumerateInstanceExtensionProperties(nullptr, &extCount, nullptr);
			if (extCount > 0) {
				std::vector<VkExtensionProperties> extensions(extCount);
				if (vkEnumerateInstanceExtensionProperties(nullptr, &extCount, &extensions.front()) == VK_SUCCESS)
				{
					for (VkExtensionProperties extension : extensions)
					{
						_supportExtensions.emplace_back(extension.extensionName);
					}
				}
			}
			// Enabled requested instance extensions
			auto& enabledExtensions = info.enabledExtensions;
			if (enabledExtensions.size() > 0) {
				for (auto& ext : enabledExtensions)
				{
					// Output message if requested extension is not available
					if (std::find(_supportExtensions.begin(), _supportExtensions.end(), ext) != _supportExtensions.end() &&
						std::find(_enableExtensions.begin(), _enableExtensions.end(), ext) == _enableExtensions.end())
					{
						_enableExtensions.emplace_back(ext);
					}
				}
			}
			VkInstanceCreateInfo instanceInfo{};
			instanceInfo.sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO;
			instanceInfo.pApplicationInfo = &appInfo;
			instanceInfo.pNext = nullptr;
            instanceInfo.flags = 0;
            instanceInfo.enabledExtensionCount = 0;
            instanceInfo.ppEnabledExtensionNames = nullptr;
            instanceInfo.enabledLayerCount = 0;
            instanceInfo.ppEnabledLayerNames = nullptr;
			std::vector<const char*> exts(_enableExtensions.size());
			if (_enableExtensions.size() > 0)
			{
				if (_isValidation)
				{
					std::string debugExt{ VK_EXT_DEBUG_UTILS_EXTENSION_NAME };
					if (std::find(_enableExtensions.begin(), _enableExtensions.end(), debugExt) == _enableExtensions.end())
						_enableExtensions.push_back(debugExt);
				}
				instanceInfo.enabledExtensionCount = (uint32_t)_enableExtensions.size();
				std::transform(std::begin(_enableExtensions), std::end(_enableExtensions), std::begin(exts), [&](const std::string& name) { return name.c_str(); });
				instanceInfo.ppEnabledExtensionNames = exts.data();
			}

			// The VK_LAYER_KHRONOS_validation contains all current validation functionality.
			// Note that on Android this layer requires at least NDK r20
			const char* validationLayerName = "VK_LAYER_KHRONOS_validation";
			if (_isValidation)
			{
				// Check if this layer is available at instance level
				uint32_t instanceLayerCount;
				vkEnumerateInstanceLayerProperties(&instanceLayerCount, nullptr);
				std::vector<VkLayerProperties> instanceLayerProperties(instanceLayerCount);
				vkEnumerateInstanceLayerProperties(&instanceLayerCount, instanceLayerProperties.data());
				bool validationLayerPresent = false;
				for (auto& layer : instanceLayerProperties) {
					if (strcmp(layer.layerName, validationLayerName) == 0) {
						validationLayerPresent = true;
						break;
					}
				}
				if (validationLayerPresent) {
					instanceInfo.ppEnabledLayerNames = &validationLayerName;
					instanceInfo.enabledLayerCount = 1;
				}
				else {
					std::cerr << "Validation layer VK_LAYER_KHRONOS_validation not present, validation is disabled";
				}
			}
			VkResult res{ vkCreateInstance(&instanceInfo, nullptr, &_instance) };
			if (res != VK_SUCCESS) {
				exitFatal("Could not create Vulkan instance : \n" + errorString(res), res);
			}
#if defined(USE_PLATFORM_ANDROID_KHR)
			android::loadVulkanFunctions(_instance);
#endif
			if (_isValidation) {
				VkDebugReportFlagsEXT debugReportFlags = VK_DEBUG_REPORT_ERROR_BIT_EXT | VK_DEBUG_REPORT_WARNING_BIT_EXT;
				debug::setupDebugging(_instance, debugReportFlags, VK_NULL_HANDLE);
			}
		}
	}
}
