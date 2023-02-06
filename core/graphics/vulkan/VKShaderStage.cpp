/*
 *  VKShaderStage.cpp
 *
 *  Copyright (c) 2022 by XiangKui Zheng
 *  This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */

#include <graphics/vulkan/VKShaderStage.h>
#include <graphics/vulkan/VKContext.h>
#include <graphics/vulkan/VKDevice.h>
#include <graphics/vulkan/VKTools.h>
namespace eg {
	namespace graphics {
		VKShaderStage::VKShaderStage(const std::vector<ShaderStageInfo>& infos) : ShaderStage(infos)
		{
			auto context = Context::GetContext();
			auto device = dynamic_cast<VKDevice*>(context->getDevice().get());
			VkShaderModuleCreateInfo moduleInfo{};
			moduleInfo.sType = VK_STRUCTURE_TYPE_SHADER_MODULE_CREATE_INFO;
			if (_vertexInfo.pCode) {
				moduleInfo.codeSize = _vertexInfo.size;
				moduleInfo.pCode = _vertexInfo.pCode;
				VK_CHECK_RESULT(vkCreateShaderModule(device->getLogicDevice(), &moduleInfo, nullptr, &_vkVertexModule));
				delete[] _vertexInfo.pCode;
				_createShaderStage(_vertexInfo, _vkVertexInfo, _vkVertexModule);
			}
			if (_geometryInfo.pCode) {
				moduleInfo.codeSize = _geometryInfo.size;
				moduleInfo.pCode = _geometryInfo.pCode;
				VK_CHECK_RESULT(vkCreateShaderModule(device->getLogicDevice(), &moduleInfo, nullptr, &_vkGeometryModule));
				delete[] _geometryInfo.pCode;
				_createShaderStage(_geometryInfo, _vkGeometryInfo, _vkGeometryModule);
			}
			if (_fragmentInfo.pCode) {
				moduleInfo.codeSize = _fragmentInfo.size;
				moduleInfo.pCode = _fragmentInfo.pCode;
				VK_CHECK_RESULT(vkCreateShaderModule(device->getLogicDevice(), &moduleInfo, nullptr, &_vkFragmentModule));
				delete[] _fragmentInfo.pCode;
				_createShaderStage(_fragmentInfo, _vkFragmentInfo, _vkFragmentModule);
			}
			if (_computeInfo.pCode) {
				moduleInfo.codeSize = _computeInfo.size;
				moduleInfo.pCode = _computeInfo.pCode;
				VK_CHECK_RESULT(vkCreateShaderModule(device->getLogicDevice(), &moduleInfo, nullptr, &_vkComputeModule));
				delete[] _computeInfo.pCode;
				_createShaderStage(_computeInfo, _vkComputeInfo, _vkComputeModule);
			}
			if (_tessellateControlInfo.pCode) {
				moduleInfo.codeSize = _tessellateControlInfo.size;
				moduleInfo.pCode = _tessellateControlInfo.pCode;
				VK_CHECK_RESULT(vkCreateShaderModule(device->getLogicDevice(), &moduleInfo, nullptr, &_vkTessellateControlModule));
				delete[] _tessellateControlInfo.pCode;
				_createShaderStage(_tessellateControlInfo, _vkTessellateControlInfo, _vkTessellateControlModule);
			}
			if (_tessellateEvaInfo.pCode) {
				moduleInfo.codeSize = _tessellateEvaInfo.size;
				moduleInfo.pCode = _tessellateEvaInfo.pCode;
				VK_CHECK_RESULT(vkCreateShaderModule(device->getLogicDevice(), &moduleInfo, nullptr, &_vkTessellateEvaModule));
				delete[] _tessellateEvaInfo.pCode;
				_createShaderStage(_tessellateEvaInfo, _vkTessellateEvaInfo, _vkTessellateEvaModule);
			}
		}

		VKShaderStage::~VKShaderStage()
		{
			auto context = Context::GetContext();
			auto device = dynamic_cast<VKDevice*>(context->getDevice().get());
			for (auto& stage: _stages) {
				vkDestroyShaderModule(device->getLogicDevice(), stage.module, nullptr);
			}
			_stages.clear();
		}

		void VKShaderStage::_createShaderStage(const ShaderStageInfo& info, VkPipelineShaderStageCreateInfo& stage, const VkShaderModule& module)
		{
			stage.sType = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO;
			stage.module = module;
			stage.pName = info.pName.c_str();
			stage.stage = static_cast<VkShaderStageFlagBits>(info.stage);
			_stages.emplace_back(stage);
		}
	}
}
