/*
 *  VKGraphicsPipeline.h
 *
 *  Copyright (c) 2022 by XiangKui Zheng
 *  This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */

#pragma once

#include <graphics/GraphicsPipeline.h>
#include <vulkan/vulkan.h>
namespace eg {
	namespace graphics {
		class VKGraphicsPipeline : public GraphicsPipeline
		{
		public:
			VKGraphicsPipeline() = default;
			VKGraphicsPipeline(const GraphicsPipelineInfo& info);
			~VKGraphicsPipeline() override;
			void destroy() override;
			const VkPipeline& getVkPipeline() const { return _pipeline; }
		private:
			VkPipeline _pipeline;
			// Pipeline cache object
			VkPipelineCache _pipelineCache{VK_NULL_HANDLE};
			void _createPipelineCache();
		};
	}
}
