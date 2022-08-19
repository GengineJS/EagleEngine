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
			std::shared_ptr<GraphicsPipeline> createGraphicsPipeline(const GraphicsPipelineInfo& info) override;
			std::shared_ptr<Swapchain> createSwapchain() override;
			std::shared_ptr<CommandBuffer> createCommandBuffer(const CommandBufferInfo& info) override;
			std::shared_ptr<Texture> createTexture(const TextureInfo& info) override;
			std::shared_ptr<Texture> createTexture(const TextureInfo& info, VkImage img);
			std::shared_ptr<Texture> loadTexture(const std::string& filePath) override;
			std::shared_ptr<Buffer> createBuffer(const BufferInfo& info) override;
			std::shared_ptr<BufferView> createBufferView(const BufferViewInfo& info) override;
			std::shared_ptr<ShaderStage> createShader(const ShaderDesc& info) override;
			std::shared_ptr<Framebuffer> createFramebuffer(const FramebufferInfo& info) override;
			std::shared_ptr<RenderPass> createRenderPass(const RenderPassInfo& info) override;
			std::shared_ptr<VertexInput> createVertexInput(const VertexInputInfo& info) override;
			std::shared_ptr<InputAssembler> createInputAssembler(const InputAssemblerInfo& info) override;
			std::shared_ptr<Sampler> createSampler(const SamplerInfo& info) override;
			std::shared_ptr<DescriptorSetLayout> createDescriptorSetLayout(const DescriptorSetLayoutInfo& info) override;
			std::shared_ptr<DescriptorPool> createDescriptorPool(const DescriptorPoolInfo& info) override;
			std::shared_ptr<DescriptorSet> createDescriptorSet() override;
			std::shared_ptr<DescriptorSet> createDescriptorSet(const std::unordered_map<uint32_t, std::shared_ptr<Buffer>>&) override;
			std::shared_ptr<DescriptorSet> createDescriptorSet(const std::unordered_map<uint32_t, std::shared_ptr<Texture>>&) override;
			std::shared_ptr<DescriptorSet> createDescriptorSet(const DescriptorSetInfo& info) override;
			std::shared_ptr<DescriptorSet> createDescriptorSet(const std::unordered_map<uint32_t, std::shared_ptr<Buffer>>&, const std::unordered_map<uint32_t, std::shared_ptr<Texture>>&) override;
			std::shared_ptr<PipelineLayout> createPipelineLayout(const PipelineLayoutInfo& info) override;
			std::shared_ptr<PipelineLayout> createPipelineLayout(const std::vector<std::shared_ptr<DescriptorSetLayout>>& pSetLayouts) override;

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
			std::shared_ptr<void> _deviceCreatepNextChain{ nullptr };
			// Set to true when the debug marker extension is detected*
			bool _enableDebugMarkers{ false };
			// Handle to the device graphics queue that command buffers are submitted to
			VkQueue _graphicQueue;
			void _createLogicDevice();
		};
	}
}
