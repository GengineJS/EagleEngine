#pragma once

#include <graphics/ShaderStage.h>
#include <vulkan/vulkan.h>
namespace eg {
	namespace graphics {
		class VKShaderStage : public ShaderStage
		{
		public:
			VKShaderStage() = default;
			VKShaderStage(const std::vector<ShaderStageInfo>& infos);
			~VKShaderStage() override;
			inline const std::vector<VkPipelineShaderStageCreateInfo>& getVkShaderStages() const { return _stages; }
			inline const VkPipelineShaderStageCreateInfo& getVkVertexInfo() const { return _vkVertexInfo; }
			inline const VkPipelineShaderStageCreateInfo& getVkGeometryInfo() const { return _vkGeometryInfo; }
			inline const VkPipelineShaderStageCreateInfo& getVkTessellatControlInfo() const { return _vkTessellateControlInfo; }
			inline const VkPipelineShaderStageCreateInfo& getVkTessellatEvaInfo() const { return _vkTessellateEvaInfo; }
			inline const VkPipelineShaderStageCreateInfo& getVkComputeInfo() const { return _vkComputeInfo; }
			inline const VkPipelineShaderStageCreateInfo& getVkFragmentInfo() const { return _vkFragmentInfo; }
		protected:
			void _createShaderStage(const ShaderStageInfo& info, VkPipelineShaderStageCreateInfo& stage, const VkShaderModule& module);
			VkPipelineShaderStageCreateInfo _vkVertexInfo{};
			VkShaderModule _vkVertexModule;
			VkPipelineShaderStageCreateInfo _vkGeometryInfo{};
			VkShaderModule _vkGeometryModule;
			VkPipelineShaderStageCreateInfo _vkFragmentInfo{};
			VkShaderModule _vkFragmentModule;
			VkPipelineShaderStageCreateInfo _vkComputeInfo{};
			VkShaderModule _vkComputeModule;
			VkPipelineShaderStageCreateInfo _vkTessellateControlInfo{};
			VkShaderModule _vkTessellateControlModule;
			VkPipelineShaderStageCreateInfo _vkTessellateEvaInfo{};
			VkShaderModule _vkTessellateEvaModule;
			std::vector<VkPipelineShaderStageCreateInfo> _stages;
		};
	}
}
