/*
 *  Device.h
 *
 *  Copyright (c) 2022 by XiangKui Zheng
 *  This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */

#pragma once
#include "graphics/GraphicsDef.h"
#include "graphics/Texture.h"
#include <graphics/Swapchain.h>
#include <graphics/CommandBuffer.h>
#include <vector>
#include <graphics/Buffer.h>
#include <graphics/ShaderStage.h>
#include <graphics/RenderPass.h>
#include <graphics/VertexInput.h>
#include <graphics/PipelineLayout.h>
#include <graphics/Sampler.h>
#include <graphics/DescriptorSetLayout.h>
#include <graphics/DescriptorPool.h>
#include <graphics/DescriptorSet.h>
#include <graphics/GraphicsPipeline.h>
namespace eg {
	namespace graphics {
		struct QueueFamilyIndices
		{
			uint32_t graphics{ UINT32_MAX };
			uint32_t compute{ UINT32_MAX };
			uint32_t transfer{ UINT32_MAX };
			uint32_t present{ UINT32_MAX };
		};

		class Device {
		public:
			Device() = default;
			virtual ~Device();
			virtual bool initialize() = 0;
			virtual void destroy() = 0;
			virtual std::shared_ptr<GraphicsPipeline> createGraphicsPipeline(const GraphicsPipelineInfo& info) = 0;
			virtual std::shared_ptr<Swapchain> createSwapchain() = 0;
			virtual std::shared_ptr<CommandBuffer> createCommandBuffer(const CommandBufferInfo& info) = 0;
			virtual std::shared_ptr<Texture> createTexture(const TextureInfo& info) = 0;
			virtual std::shared_ptr<Texture> loadTexture(const std::string& filePath) = 0;
			virtual std::shared_ptr<Buffer> createBuffer(const BufferInfo& info) = 0;
			virtual std::shared_ptr<BufferView> createBufferView(const BufferViewInfo& info) = 0;
			virtual std::shared_ptr<ShaderStage> createShader(const ShaderDesc& info) = 0;
			virtual std::shared_ptr<Framebuffer> createFramebuffer(const FramebufferInfo& info) = 0;
			virtual std::shared_ptr<RenderPass> createRenderPass(const RenderPassInfo& info) = 0;
			virtual std::shared_ptr<VertexInput> createVertexInput(const VertexInputInfo& info) = 0;
			virtual std::shared_ptr<InputAssembler> createInputAssembler(const InputAssemblerInfo& info) = 0;
			virtual std::shared_ptr<Sampler> createSampler(const SamplerInfo& info) = 0;
			virtual std::shared_ptr<DescriptorSetLayout> createDescriptorSetLayout(const DescriptorSetLayoutInfo& info) = 0;
			virtual std::shared_ptr<DescriptorPool> createDescriptorPool(const DescriptorPoolInfo& info) = 0;
			virtual std::shared_ptr<DescriptorSet> createDescriptorSet() = 0;
			virtual std::shared_ptr<DescriptorSet> createDescriptorSet(const DescriptorSetInfo& info) = 0;
			virtual std::shared_ptr<DescriptorSet> createDescriptorSet(const std::unordered_map<uint32_t, std::shared_ptr<Buffer>>&) = 0;
			virtual std::shared_ptr<DescriptorSet> createDescriptorSet(const std::unordered_map<uint32_t, std::shared_ptr<Texture>>&) = 0;
			virtual std::shared_ptr<DescriptorSet> createDescriptorSet(const std::unordered_map<uint32_t, std::shared_ptr<Buffer>>&, const std::unordered_map<uint32_t, std::shared_ptr<Texture>>&) = 0;
			virtual std::shared_ptr<PipelineLayout> createPipelineLayout(const PipelineLayoutInfo& info) = 0;
			virtual std::shared_ptr<PipelineLayout> createPipelineLayout(const std::vector<std::shared_ptr<DescriptorSetLayout>>& pSetLayouts) = 0;

			inline const QueueFamilyIndices& getQueueFamilyIndices() const { return _queueFamilyIndices; }
			inline void setPresentQueueIdx(uint32_t idx) { _queueFamilyIndices.present = idx; }

			inline const std::vector<std::string>& getEnableExtensions() const { return _enableExtensions; }
			inline const std::vector<std::string>& getSupportExtensions() const { return _supportExtensions; }
			inline bool extensionSupported(const std::string& extension) const { return (std::find(_supportExtensions.begin(), _supportExtensions.end(), extension) != _supportExtensions.end()); };
		protected:
			// Device extensions
			std::vector<std::string> _enableExtensions{};
			std::vector<std::string> _supportExtensions{};
			QueueFamilyIndices _queueFamilyIndices{};
		};
	}
}
