#include <graphics/vulkan/VKGraphicsPipeline.h>
#include <graphics/vulkan/VKContext.h>
#include <graphics/vulkan/VKDevice.h>
#include <graphics/vulkan/VKShaderStage.h>
#include <assert.h>
#include <graphics/vulkan/VKVertexInput.h>
#include <graphics/vulkan/VKPipelineLayout.h>
#include <graphics/vulkan/VKRenderPass.h>
#include <graphics/vulkan/VKSwapchain.h>
namespace eg {
	namespace graphics {
		VKGraphicsPipeline::VKGraphicsPipeline(const GraphicsPipelineInfo& info): GraphicsPipeline(info)
		{
			auto context = Context::GetContext();
			auto device = std::dynamic_pointer_cast<VKDevice>(context->getDevice());
			auto swapchain = std::dynamic_pointer_cast<VKSwapchain>(context->getSwapchain());
			assert(_info.shader && _info.vertexInput && _info.pipelineLayout && _info.renderPass);
			auto shader = std::dynamic_pointer_cast<VKShaderStage>(_info.shader);
			auto vi = std::dynamic_pointer_cast<VKVertexInput>(_info.vertexInput);
			auto pipLayout = std::dynamic_pointer_cast<VKPipelineLayout>(_info.pipelineLayout);
			auto renderPass = std::dynamic_pointer_cast<VKRenderPass>(_info.renderPass);
			VkGraphicsPipelineCreateInfo pipelineInfo{};
			pipelineInfo.sType = VK_STRUCTURE_TYPE_GRAPHICS_PIPELINE_CREATE_INFO;
			pipelineInfo.basePipelineIndex = _info.basePipelineIdx;
			if (_info.basePipeline) {
				pipelineInfo.basePipelineHandle = std::dynamic_pointer_cast<VKGraphicsPipeline>(_info.basePipeline)->getVkPipeline();
			}
			pipelineInfo.layout = pipLayout->getVkPipelineLayout();
			
			pipelineInfo.renderPass = renderPass->getVkRenderPass();
			pipelineInfo.pStages = shader->getVkShaderStages().data();
			pipelineInfo.stageCount = static_cast<uint32_t>(shader->getVkShaderStages().size());
			pipelineInfo.subpass = _info.subpass;
			VkPipelineColorBlendAttachmentState colorBlendState{};
			colorBlendState.colorWriteMask = VK_COLOR_COMPONENT_R_BIT | VK_COLOR_COMPONENT_G_BIT | VK_COLOR_COMPONENT_B_BIT | VK_COLOR_COMPONENT_A_BIT;
			colorBlendState.blendEnable = VK_FALSE;
			colorBlendState.srcColorBlendFactor = VK_BLEND_FACTOR_ONE; // Optional
			colorBlendState.dstColorBlendFactor = VK_BLEND_FACTOR_ZERO; // Optional
			colorBlendState.colorBlendOp = VK_BLEND_OP_ADD; // Optional
			colorBlendState.srcAlphaBlendFactor = VK_BLEND_FACTOR_ONE; // Optional
			colorBlendState.dstAlphaBlendFactor = VK_BLEND_FACTOR_ZERO; // Optional
			colorBlendState.alphaBlendOp = VK_BLEND_OP_ADD; // Optional
			VkPipelineColorBlendStateCreateInfo colorBlendStateInfo{};
			colorBlendStateInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_COLOR_BLEND_STATE_CREATE_INFO;
			colorBlendStateInfo.logicOpEnable = VK_FALSE;
			colorBlendStateInfo.logicOp = VK_LOGIC_OP_COPY;
			colorBlendStateInfo.attachmentCount = 1;
			colorBlendStateInfo.pAttachments = &colorBlendState;
			colorBlendStateInfo.blendConstants[0] = 0.0f;
			colorBlendStateInfo.blendConstants[1] = 0.0f;
			colorBlendStateInfo.blendConstants[2] = 0.0f;
			colorBlendStateInfo.blendConstants[3] = 0.0f;
			if (_info.colorBlend.isLogicOp) {
				colorBlendStateInfo.logicOpEnable = VK_TRUE;
				colorBlendStateInfo.logicOp = static_cast<VkLogicOp>(_info.colorBlend.logicOp);
				colorBlendStateInfo.attachmentCount = static_cast<uint32_t>(_info.colorBlend.attachments.size());
				std::vector<VkPipelineColorBlendAttachmentState> blendStates{};
				for (auto& blendState: _info.colorBlend.attachments) {
					VkPipelineColorBlendAttachmentState colorBlendState{};
					colorBlendState.colorWriteMask = static_cast<VkColorComponentFlags>(blendState.colorWriteMask);
					colorBlendState.blendEnable = blendState.blendEnable;
					colorBlendState.srcColorBlendFactor = static_cast<VkBlendFactor>(blendState.srcColorBlend);
					colorBlendState.dstColorBlendFactor = static_cast<VkBlendFactor>(blendState.dstColorBlend);
					colorBlendState.colorBlendOp = static_cast<VkBlendOp>(blendState.colorBlendOp);
					colorBlendState.srcAlphaBlendFactor = static_cast<VkBlendFactor>(blendState.srcAlphaBlend);
					colorBlendState.dstAlphaBlendFactor = static_cast<VkBlendFactor>(blendState.dstAlphaBlend);
					colorBlendState.alphaBlendOp = static_cast<VkBlendOp>(blendState.alphaBlendOp);
					blendStates.emplace_back(std::move(colorBlendState));
				}
				colorBlendStateInfo.pAttachments = blendStates.data();

				colorBlendStateInfo.blendConstants[0] = _info.colorBlend.blendConstants[0];
				colorBlendStateInfo.blendConstants[1] = _info.colorBlend.blendConstants[1];
				colorBlendStateInfo.blendConstants[2] = _info.colorBlend.blendConstants[2];
				colorBlendStateInfo.blendConstants[3] = _info.colorBlend.blendConstants[3];
			}
			pipelineInfo.pColorBlendState = &colorBlendStateInfo;
			VkPipelineDepthStencilStateCreateInfo depthStencilStateInfo{};
			depthStencilStateInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_DEPTH_STENCIL_STATE_CREATE_INFO;
			depthStencilStateInfo.depthTestEnable = _info.depthStencil.isDepthTest;
			depthStencilStateInfo.depthWriteEnable = _info.depthStencil.isDepthWrite;
			depthStencilStateInfo.depthCompareOp = static_cast<VkCompareOp>(_info.depthStencil.depthCompare);
			depthStencilStateInfo.depthBoundsTestEnable = _info.depthStencil.isDepthBoundsTest;
			depthStencilStateInfo.stencilTestEnable = _info.depthStencil.isStencilTest;
			if (_info.depthStencil.isStencilTest) {
				VkStencilOpState backStencilState{};
				backStencilState.compareMask = _info.depthStencil.back.compareMask;
				backStencilState.compareOp = static_cast<VkCompareOp>(_info.depthStencil.back.compareOp);
				backStencilState.depthFailOp = static_cast<VkStencilOp>(_info.depthStencil.back.depthFailOp);
				backStencilState.failOp = static_cast<VkStencilOp>(_info.depthStencil.back.failOp);
				backStencilState.passOp = static_cast<VkStencilOp>(_info.depthStencil.back.passOp);
				backStencilState.reference = _info.depthStencil.back.reference;
				backStencilState.writeMask = _info.depthStencil.back.writeMask;
				depthStencilStateInfo.back = std::move(backStencilState);

				VkStencilOpState frontStencilState{};
				frontStencilState.compareMask = _info.depthStencil.front.compareMask;
				frontStencilState.compareOp = static_cast<VkCompareOp>(_info.depthStencil.front.compareOp);
				frontStencilState.depthFailOp = static_cast<VkStencilOp>(_info.depthStencil.front.depthFailOp);
				frontStencilState.failOp = static_cast<VkStencilOp>(_info.depthStencil.front.failOp);
				frontStencilState.passOp = static_cast<VkStencilOp>(_info.depthStencil.front.passOp);
				frontStencilState.reference = _info.depthStencil.front.reference;
				frontStencilState.writeMask = _info.depthStencil.front.writeMask;
				depthStencilStateInfo.front = std::move(frontStencilState);
			}
			if (_info.depthStencil.isDepthBoundsTest) {
				depthStencilStateInfo.minDepthBounds = _info.depthStencil.minDepthBounds;
				depthStencilStateInfo.maxDepthBounds = _info.depthStencil.maxDepthBounds;
			}
			pipelineInfo.pDepthStencilState = &depthStencilStateInfo;
			std::vector<VkDynamicState> dynamicStates{};
			for (auto& dynamic: _info.dynamicInfo.dynamicStates) {
				dynamicStates.emplace_back(static_cast<VkDynamicState>(dynamic));
			}
			VkPipelineDynamicStateCreateInfo dynamicInfo{};
			dynamicInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_DYNAMIC_STATE_CREATE_INFO;
			dynamicInfo.dynamicStateCount = static_cast<uint32_t>(dynamicStates.size());
			dynamicInfo.pDynamicStates = dynamicStates.data();
			pipelineInfo.pDynamicState = &dynamicInfo;
			VkPipelineMultisampleStateCreateInfo multiSampleStateInfo{};
			multiSampleStateInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_MULTISAMPLE_STATE_CREATE_INFO;
			multiSampleStateInfo.sampleShadingEnable = _info.multiSample.isSampleShading;
			multiSampleStateInfo.rasterizationSamples = static_cast<VkSampleCountFlagBits>(_info.multiSample.rasterizationSamples);
			multiSampleStateInfo.minSampleShading = _info.multiSample.minSampleShading;
			multiSampleStateInfo.pSampleMask = _info.multiSample.sampleMask.get();
			multiSampleStateInfo.alphaToCoverageEnable = _info.multiSample.isAlphaToCoverage;
			multiSampleStateInfo.alphaToOneEnable = _info.multiSample.isAlphaToOne;
			pipelineInfo.pMultisampleState = &multiSampleStateInfo;
			
			VkPipelineRasterizationStateCreateInfo rasterizationStateInfo{};
			rasterizationStateInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_RASTERIZATION_STATE_CREATE_INFO;
			rasterizationStateInfo.depthClampEnable = _info.raster.isDepthClamp;
			rasterizationStateInfo.rasterizerDiscardEnable = _info.raster.isDiscard;
			rasterizationStateInfo.polygonMode = static_cast<VkPolygonMode>(_info.raster.polygonMode);
			rasterizationStateInfo.lineWidth = _info.raster.lineWidth;
			rasterizationStateInfo.cullMode = static_cast<VkCullModeFlags>(_info.raster.cullMode);
			rasterizationStateInfo.frontFace = static_cast<VkFrontFace>(_info.raster.frontFace);
			rasterizationStateInfo.depthBiasEnable = _info.raster.isDepthBias;
			rasterizationStateInfo.depthBiasConstantFactor = _info.raster.depthBiasConstant;
			rasterizationStateInfo.depthBiasClamp = _info.raster.depthBiasClamp;
			rasterizationStateInfo.depthBiasSlopeFactor = _info.raster.depthBiasSlopeFactor;
			pipelineInfo.pRasterizationState = &rasterizationStateInfo;
			
			
			pipelineInfo.pVertexInputState = &vi->getVkVertexInputStateInfo();
			auto vkIa = std::dynamic_pointer_cast<VKInputAssembler>(vi->getInputAssembler());
			pipelineInfo.pInputAssemblyState = &vkIa->getVkInputAssemblyState();

			VkViewport viewport{};
			viewport.x = 0;
			viewport.y = 0;
			viewport.width = static_cast<float>(swapchain->getWidth());
			viewport.height = static_cast<float>(swapchain->getHeight());
			viewport.minDepth = 0;
			viewport.maxDepth = 1;
			VkRect2D scissor{};
			scissor.offset = { 0, 0 };
			scissor.extent = { swapchain->getWidth(), swapchain->getHeight() };
			VkPipelineViewportStateCreateInfo viewportStateInfo{};
			viewportStateInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_VIEWPORT_STATE_CREATE_INFO;
			viewportStateInfo.viewportCount = 1;
			viewportStateInfo.pViewports = &viewport;
			viewportStateInfo.scissorCount = 1;
			viewportStateInfo.pScissors = &scissor;
			pipelineInfo.pViewportState = &viewportStateInfo;
			if (!_pipelineCache) {
				_createPipelineCache();
			}
			VK_CHECK_RESULT(vkCreateGraphicsPipelines(device->getLogicDevice(), _pipelineCache, 1, &pipelineInfo, nullptr, &_pipeline));
		}
		VKGraphicsPipeline::~VKGraphicsPipeline()
		{
			destroy();
		}
		void VKGraphicsPipeline::_createPipelineCache() {
			auto context = Context::GetContext();
			auto device = std::dynamic_pointer_cast<VKDevice>(context->getDevice());
			VkPipelineCacheCreateInfo pipelineCacheInfo{};
			pipelineCacheInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_CACHE_CREATE_INFO;
			vkCreatePipelineCache(device->getLogicDevice(), &pipelineCacheInfo, nullptr, &_pipelineCache);
		}
		void VKGraphicsPipeline::destroy()
		{
			auto context = Context::GetContext();
			auto device = std::dynamic_pointer_cast<VKDevice>(context->getDevice());
			vkDestroyPipeline(device->getLogicDevice(), _pipeline, nullptr);
			vkDestroyPipelineCache(device->getLogicDevice(), _pipelineCache, nullptr);
		}
	}
}
