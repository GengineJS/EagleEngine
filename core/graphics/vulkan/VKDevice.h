/*
 *  VKDevice.h
 *
 *  Copyright (c) 2022 by XiangKui Zheng
 *  This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */

#pragma once
#include "vulkan/vulkan.h"
#include "graphics/Device.h"
namespace eg {
	namespace graphics {
		class VKDevice : public Device
		{
		public:
			VKDevice();
			~VKDevice() override;
			bool initialize() override;
			void destroy() override;
			std::unique_ptr<GraphicsPipeline> createGraphicsPipeline(const GraphicsPipelineInfo& info) override;
			std::unique_ptr<Swapchain> createSwapchain() override;
			std::unique_ptr<CommandBuffer> createCommandBuffer(const CommandBufferInfo& info) override;
			std::unique_ptr<Texture> createTexture(const TextureInfo& info) override;
			std::unique_ptr<Texture> createTexture(const TextureInfo& info, VkImage img);
			std::unique_ptr<Texture> loadTexture(const std::string& filePath) override;
			std::unique_ptr<Buffer> createBuffer(const BufferInfo& info) override;
			std::unique_ptr<BufferView> createBufferView(const BufferViewInfo& info) override;
			std::unique_ptr<ShaderStage> createShader(const ShaderDesc& info) override;
			std::unique_ptr<Framebuffer> createFramebuffer(const FramebufferInfo& info) override;
			std::unique_ptr<RenderPass> createRenderPass(const RenderPassInfo& info) override;
			std::unique_ptr<VertexInput> createVertexInput(const VertexInputInfo& info) override;
			std::unique_ptr<InputAssembler> createInputAssembler(const InputAssemblerInfo& info) override;
			std::unique_ptr<Sampler> createSampler(const SamplerInfo& info) override;
			std::unique_ptr<DescriptorSetLayout> createDescriptorSetLayout(const DescriptorSetLayoutInfo& info) override;
			std::unique_ptr<DescriptorPool> createDescriptorPool(const DescriptorPoolInfo& info) override;
			std::unique_ptr<DescriptorSet> createDescriptorSet() override;
			std::unique_ptr<DescriptorSet> createDescriptorSet(const std::unordered_map<uint32_t, Buffer*>&) override;
			std::unique_ptr<DescriptorSet> createDescriptorSet(const std::unordered_map<uint32_t, Texture*>&) override;
			std::unique_ptr<DescriptorSet> createDescriptorSet(const DescriptorSetInfo& info) override;
			std::unique_ptr<DescriptorSet> createDescriptorSet(const std::unordered_map<uint32_t, Buffer*>&, const std::unordered_map<uint32_t, Texture*>&) override;
			std::unique_ptr<PipelineLayout> createPipelineLayout(const PipelineLayoutInfo& info) override;
			std::unique_ptr<PipelineLayout> createPipelineLayout(const std::vector<DescriptorSetLayout*>& pSetLayouts) override;

			inline const VkPhysicalDevice& getPhysicalDevice()const { return _physicalDevice; }
			inline const VkDevice& getLogicDevice()const { return _logicDevice; }
			inline const VkPhysicalDeviceProperties& getDeviceProps() const { return _deviceProperties; }
			inline const VkPhysicalDeviceFeatures& getDeviceFeatures() const { return _deviceFeatures; }
			inline const VkPhysicalDeviceMemoryProperties& getDeviceMemoryProps() const { return _deviceMemoryProperties; }
			inline const VkQueue& getGraphicQueue() const { return _graphicQueue; }

			inline bool isDebugMarkers() const { return _enableDebugMarkers; }
			uint32_t getQueueFamilyIndex(VkQueueFlagBits queueFlags) const;
			uint32_t getMemoryTypeIndex(uint32_t typeFilter, VkMemoryPropertyFlags memoryPropFlags) const;
		protected:
			VkPhysicalDevice _physicalDevice;
			VkDevice _logicDevice;
			VkPhysicalDeviceProperties _deviceProperties;
			VkPhysicalDeviceFeatures _deviceFeatures;
			// Set of physical device features to be enabled for this
			VkPhysicalDeviceFeatures _enabledFeatures{};
			VkPhysicalDeviceMemoryProperties _deviceMemoryProperties;
			std::vector<VkQueueFamilyProperties> _queueFamilyProperties;
			void* _deviceCreatepNextChain{ nullptr };
			// Set to true when the debug marker extension is detected*
			bool _enableDebugMarkers{ false };
			// Handle to the device graphics queue that command buffers are submitted to
			VkQueue _graphicQueue;
			void _createLogicDevice();
		};
	}
}
