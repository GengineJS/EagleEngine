/*
 *  VKDevice.cpp
 *
 *  Copyright (c) 2022 by XiangKui Zheng
 *  This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */

#include "graphics/vulkan/VKDevice.h"
#include <graphics/vulkan/VKContext.h>
#include <graphics/vulkan/VKTools.h>
#include <algorithm>
#include <valarray>
#include <graphics/vulkan/VKSwapchain.h>
#include <graphics/vulkan/VKCommandBuffer.h>
#include <graphics/vulkan/VKTexture.h>
#include <graphics/vulkan/VKBuffer.h>
#include <graphics/vulkan/VKSampler.h>
#include <graphics/vulkan/VKShaderStage.h>
#include <graphics/vulkan/VKFramebuffer.h>
#include <graphics/vulkan/VKRenderPass.h>
#include <graphics/vulkan/VKVertexInput.h>
#include <graphics/vulkan/VKDescriptorSetLayout.h>
#include <graphics/vulkan/VKDescriptorPool.h>
#include <graphics/vulkan/VKDescriptorSet.h>
#include <graphics/vulkan/VKPipelineLayout.h>
#include <graphics/vulkan/VKGraphicsPipeline.h>
#include <utils/FileUtils.h>
#include <engine/EagleEngine.h>
namespace eg {
	namespace graphics {
		VKDevice::VKDevice()
		{
		}

		VKDevice::~VKDevice()
		{
			vkDestroyDevice(_logicDevice, nullptr);
		}

		bool VKDevice::initialize()
		{
			auto vkContext = dynamic_cast<VKContext*>(Context::GetContext());
			uint32_t gpuCount = 0;
			VK_CHECK_RESULT(vkEnumeratePhysicalDevices(vkContext->getVkInstance(), &gpuCount, nullptr));
			if (gpuCount == 0) {
				exitFatal("No device with Vulkan support found", -1);
			}
			std::vector<VkPhysicalDevice> physicalDevices(gpuCount);
			VkResult err = vkEnumeratePhysicalDevices(vkContext->getVkInstance(), &gpuCount, physicalDevices.data());
			if (err) {
				exitFatal("Could not enumerate physical devices : \n" + errorString(err), err);
				return false;
			}
			uint32_t selectedDevice = 0;
			_physicalDevice = physicalDevices[selectedDevice];

			vkGetPhysicalDeviceProperties(_physicalDevice, &_deviceProperties);
			vkGetPhysicalDeviceFeatures(_physicalDevice, &_deviceFeatures);
			vkGetPhysicalDeviceMemoryProperties(_physicalDevice, &_deviceMemoryProperties);
			uint32_t queueFamilyCount;
			vkGetPhysicalDeviceQueueFamilyProperties(_physicalDevice, &queueFamilyCount, nullptr);
			assert(queueFamilyCount > 0);
			_queueFamilyProperties.resize(queueFamilyCount);
			vkGetPhysicalDeviceQueueFamilyProperties(_physicalDevice, &queueFamilyCount, _queueFamilyProperties.data());
			uint32_t extCount = 0;
			vkEnumerateDeviceExtensionProperties(_physicalDevice, nullptr, &extCount, nullptr);
			if (extCount > 0)
			{
				std::vector<VkExtensionProperties> extensions(extCount);
				if (vkEnumerateDeviceExtensionProperties(_physicalDevice, nullptr, &extCount, &extensions.front()) == VK_SUCCESS)
				{
					for (auto& ext : extensions)
					{
						_supportExtensions.push_back(ext.extensionName);
					}
				}
			}
			auto& enabledExtensions = EagleEngine::Get()->getEngineInfo().enabledExtensions;
			if (enabledExtensions.size() > 0) {
				for (auto& ext : enabledExtensions)
				{
					if (std::find(_supportExtensions.begin(), _supportExtensions.end(), ext) != _supportExtensions.end() &&
						std::find(_enableExtensions.begin(), _enableExtensions.end(), ext) == _enableExtensions.end())
					{
						_enableExtensions.emplace_back(ext.c_str());
					}
				}
			}
			_createLogicDevice();
			return true;
		}

		void VKDevice::destroy()
		{
		}

		std::unique_ptr<GraphicsPipeline> VKDevice::createGraphicsPipeline(const GraphicsPipelineInfo& info)
		{
			return std::make_unique<VKGraphicsPipeline>(info);
		}

		std::unique_ptr<Swapchain> VKDevice::createSwapchain()
		{
			return std::make_unique<VKSwapchain>();
		}

		std::unique_ptr<CommandBuffer> VKDevice::createCommandBuffer(const CommandBufferInfo& info)
		{
			return std::make_unique<VKCommandBuffer>(info);
		}

		std::unique_ptr<Texture> VKDevice::createTexture(const TextureInfo& info)
		{
			auto tex = std::make_unique<VKTexture>(info);
			tex->create();
			return tex;
		}

		std::unique_ptr<Texture> VKDevice::createTexture(const TextureInfo& info, VkImage img)
		{
			auto tex = std::make_unique<VKTexture>(info);
			tex->create(img);
			return tex;
		}

		std::unique_ptr<Texture> VKDevice::loadTexture(const std::string& filePath)
		{
			auto tex = std::make_unique<VKTexture>();
			tex->loadTexture(filePath);
			return tex;
		}

		std::unique_ptr<Buffer> VKDevice::createBuffer(const BufferInfo& info)
		{
			return std::make_unique<VKBuffer>(info);
		}

		std::unique_ptr<BufferView> VKDevice::createBufferView(const BufferViewInfo& info)
		{
			return std::make_unique<VKBufferView>(info);
		}

		std::unique_ptr<ShaderStage> VKDevice::createShader(const ShaderDesc& info)
		{
			std::vector<ShaderStageInfo> infos{};
			for (auto curShader: info.shaders) {
				infos.emplace_back(utils::FileUtils::loadSPIRVShader(curShader.first, curShader.second));
			}
			return std::make_unique<VKShaderStage>(infos);
		}

		std::unique_ptr<Framebuffer> VKDevice::createFramebuffer(const FramebufferInfo& info)
		{
			return std::make_unique<VKFramebuffer>(info);
		}

		std::unique_ptr<RenderPass> VKDevice::createRenderPass(const RenderPassInfo& info)
		{
			return std::make_unique<VKRenderPass>(info);
		}

		std::unique_ptr<VertexInput> VKDevice::createVertexInput(const VertexInputInfo& info)
		{
			return std::make_unique<VKVertexInput>(info);
		}

		std::unique_ptr<InputAssembler> VKDevice::createInputAssembler(const InputAssemblerInfo& info)
		{
			return std::make_unique<VKInputAssembler>(info);
		}

		std::unique_ptr<Sampler> VKDevice::createSampler(const SamplerInfo& info)
		{
			return std::make_unique<VKSampler>(info);
		}

		std::unique_ptr<DescriptorSetLayout> VKDevice::createDescriptorSetLayout(const DescriptorSetLayoutInfo& info)
		{
			return std::make_unique<VKDescriptorSetLayout>(info);
		}

		std::unique_ptr<DescriptorPool> VKDevice::createDescriptorPool(const DescriptorPoolInfo& info)
		{
			return std::make_unique<VKDescriptorPool>(info);
		}

		std::unique_ptr<DescriptorSet> VKDevice::createDescriptorSet()
		{
			auto res = std::make_unique<VKDescriptorSet>();
			return res;
		}

		std::unique_ptr<DescriptorSet> VKDevice::createDescriptorSet(const std::unordered_map<uint32_t, Buffer*>& buffers) {
			auto res = std::make_unique<VKDescriptorSet>();
			res->setBuffers(buffers);
			res->flush();
			return res;
		}

		std::unique_ptr<DescriptorSet> VKDevice::createDescriptorSet(const std::unordered_map<uint32_t, Texture*>& textures)
		{
			auto res = std::make_unique<VKDescriptorSet>();
			res->setTextures(textures);
			res->flush();
			return res;
		}

		std::unique_ptr<DescriptorSet> VKDevice::createDescriptorSet(const DescriptorSetInfo& info)
		{
			return std::make_unique<VKDescriptorSet>(info);
		}

		std::unique_ptr<DescriptorSet> VKDevice::createDescriptorSet(const std::unordered_map<uint32_t, Buffer*>& buffs, const std::unordered_map<uint32_t, Texture*>& texs)
		{
			auto res = std::make_unique<VKDescriptorSet>();
			res->setBuffers(buffs);
			res->setTextures(texs);
			res->flush();
			return res;
		}

		std::unique_ptr<PipelineLayout> VKDevice::createPipelineLayout(const PipelineLayoutInfo& info)
		{
			return  std::make_unique<VKPipelineLayout>(info);
		}

		std::unique_ptr<PipelineLayout> VKDevice::createPipelineLayout(const std::vector<DescriptorSetLayout*>& pSetLayouts)
		{
			PipelineLayoutInfo info{};
			info.pSetLayouts = pSetLayouts;
			return createPipelineLayout(info);
		}

		uint32_t VKDevice::getQueueFamilyIndex(VkQueueFlagBits queueFlags) const
		{
			if (queueFlags & VK_QUEUE_COMPUTE_BIT)
			{
				for (uint32_t i = 0; i < static_cast<uint32_t>(_queueFamilyProperties.size()); i++)
				{
					if ((_queueFamilyProperties[i].queueFlags & queueFlags) && ((_queueFamilyProperties[i].queueFlags & VK_QUEUE_GRAPHICS_BIT) == 0))
					{
						return i;
					}
				}
			}
			if (queueFlags & VK_QUEUE_TRANSFER_BIT)
			{
				for (uint32_t i = 0; i < static_cast<uint32_t>(_queueFamilyProperties.size()); i++)
				{
					if ((_queueFamilyProperties[i].queueFlags & queueFlags) && ((_queueFamilyProperties[i].queueFlags & VK_QUEUE_GRAPHICS_BIT) == 0) && ((_queueFamilyProperties[i].queueFlags & VK_QUEUE_COMPUTE_BIT) == 0))
					{
						return i;
					}
				}
			}
			for (uint32_t i = 0; i < static_cast<uint32_t>(_queueFamilyProperties.size()); i++)
			{
				if (_queueFamilyProperties[i].queueFlags & queueFlags)
				{
					return i;
				}
			}

			throw std::runtime_error("Could not find a matching queue family index");
		}

		uint32_t VKDevice::getMemoryTypeIndex(uint32_t typeFilter, VkMemoryPropertyFlags memoryPropFlags) const
		{
			for (uint32_t i = 0; i < _deviceMemoryProperties.memoryTypeCount; i++) {
				if ((typeFilter & (1 << i)) && (_deviceMemoryProperties.memoryTypes[i].propertyFlags & memoryPropFlags) == memoryPropFlags) {
					return i;
				}
			}
			throw std::runtime_error("failed to find suitable memory type!");
		}

		void VKDevice::_createLogicDevice()
		{
			const auto& info = EagleEngine::Get()->getEngineInfo();
			bool useSwapChain = info.useSwapchain;

			VkQueueFlags requestedQueueTypes = static_cast<VkQueueFlags>(QueueFlag::GRAPHICS | QueueFlag::COMPUTE);
			std::vector<VkDeviceQueueCreateInfo> queueCreateInfos{};

			const float defaultQueuePriority(0.0f);
			if (requestedQueueTypes & VK_QUEUE_GRAPHICS_BIT)
			{
				_queueFamilyIndices.graphics = getQueueFamilyIndex(VK_QUEUE_GRAPHICS_BIT);
				VkDeviceQueueCreateInfo queueInfo{};
				queueInfo.sType = VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO;
				queueInfo.queueFamilyIndex = _queueFamilyIndices.graphics;
				queueInfo.queueCount = 1;
				queueInfo.pQueuePriorities = &defaultQueuePriority;
				queueCreateInfos.push_back(queueInfo);
			}
			else
			{
				_queueFamilyIndices.graphics = 0;
			}
			if (requestedQueueTypes & VK_QUEUE_COMPUTE_BIT)
			{
				_queueFamilyIndices.compute = getQueueFamilyIndex(VK_QUEUE_COMPUTE_BIT);
				if (_queueFamilyIndices.compute != _queueFamilyIndices.graphics)
				{
					VkDeviceQueueCreateInfo queueInfo{};
					queueInfo.sType = VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO;
					queueInfo.queueFamilyIndex = _queueFamilyIndices.compute;
					queueInfo.queueCount = 1;
					queueInfo.pQueuePriorities = &defaultQueuePriority;
					queueCreateInfos.push_back(queueInfo);
				}
			}
			else
			{
				_queueFamilyIndices.compute = _queueFamilyIndices.graphics;
			}

			if (requestedQueueTypes & VK_QUEUE_TRANSFER_BIT)
			{
				_queueFamilyIndices.transfer = getQueueFamilyIndex(VK_QUEUE_TRANSFER_BIT);
				if ((_queueFamilyIndices.transfer != _queueFamilyIndices.graphics) && (_queueFamilyIndices.transfer != _queueFamilyIndices.compute))
				{
					VkDeviceQueueCreateInfo queueInfo{};
					queueInfo.sType = VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO;
					queueInfo.queueFamilyIndex = _queueFamilyIndices.transfer;
					queueInfo.queueCount = 1;
					queueInfo.pQueuePriorities = &defaultQueuePriority;
					queueCreateInfos.push_back(queueInfo);
				}
			}
			else
			{
				_queueFamilyIndices.transfer = _queueFamilyIndices.graphics;
			}

			std::vector<std::string>& deviceExtensions = _enableExtensions;
			if (useSwapChain)
			{
				std::string swapchainExt = VK_KHR_SWAPCHAIN_EXTENSION_NAME;
				if (std::find(_enableExtensions.begin(), _enableExtensions.end(), swapchainExt) == _enableExtensions.end())
					deviceExtensions.push_back(swapchainExt);
			}

			VkDeviceCreateInfo deviceCreateInfo = {};
			deviceCreateInfo.sType = VK_STRUCTURE_TYPE_DEVICE_CREATE_INFO;
			deviceCreateInfo.queueCreateInfoCount = static_cast<uint32_t>(queueCreateInfos.size());;
			deviceCreateInfo.pQueueCreateInfos = queueCreateInfos.data();
			deviceCreateInfo.pEnabledFeatures = &_enabledFeatures;

			VkPhysicalDeviceFeatures2 physicalDeviceFeatures2{};
			if (_deviceCreatepNextChain) {
				physicalDeviceFeatures2.sType = VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_FEATURES_2;
				physicalDeviceFeatures2.features = _enabledFeatures;
				physicalDeviceFeatures2.pNext = _deviceCreatepNextChain;
				deviceCreateInfo.pEnabledFeatures = nullptr;
				deviceCreateInfo.pNext = &physicalDeviceFeatures2;
			}

			std::string debugMarker{ VK_EXT_DEBUG_MARKER_EXTENSION_NAME };
			if (extensionSupported(debugMarker))
			{
				if (std::find(_enableExtensions.begin(), _enableExtensions.end(), debugMarker) == _enableExtensions.end())
					deviceExtensions.emplace_back(debugMarker.c_str());
				_enableDebugMarkers = true;
			}
			std::vector<const char*> exts(_enableExtensions.size(), nullptr);
			if (deviceExtensions.size() > 0)
			{
				for (auto& enabledExtension : deviceExtensions)
				{
					if (!extensionSupported(enabledExtension)) {
						std::cerr << "Enabled device extension \"" << enabledExtension << "\" is not present at device level\n";
					}
				}

				deviceCreateInfo.enabledExtensionCount = (uint32_t)deviceExtensions.size();
				std::transform(std::begin(_enableExtensions), std::end(_enableExtensions), std::begin(exts), [&](const std::string& name) { return name.c_str(); });
				deviceCreateInfo.ppEnabledExtensionNames = exts.data();
			}

			VkResult result = vkCreateDevice(_physicalDevice, &deviceCreateInfo, nullptr, &_logicDevice);
			if (result != VK_SUCCESS)
			{
				exitFatal("Failed to create logical device", -1);
			}
			vkGetDeviceQueue(_logicDevice, _queueFamilyIndices.graphics, 0, &_graphicQueue);
		}
	}
}
