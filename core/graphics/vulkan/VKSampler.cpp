/*
 *  VKSampler.cpp
 *
 *  Copyright (c) 2022 by XiangKui Zheng
 *  This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */

#include <graphics/vulkan/VKSampler.h>
#include <graphics/vulkan/VKDevice.h>
#include <graphics/vulkan/VKContext.h>
#include <graphics/vulkan/VKTools.h>
namespace eg {
	namespace graphics {
		VKSampler::VKSampler(const SamplerInfo& info)
		{
			auto context = Context::GetContext();
			auto device = std::dynamic_pointer_cast<VKDevice>(context->getDevice());
			VkSamplerCreateInfo samplerInfo{};
			samplerInfo.sType = VK_STRUCTURE_TYPE_SAMPLER_CREATE_INFO;
			samplerInfo.addressModeU = static_cast<VkSamplerAddressMode>(info.addressModeU);
			samplerInfo.addressModeV = static_cast<VkSamplerAddressMode>(info.addressModeV);
			samplerInfo.addressModeW = static_cast<VkSamplerAddressMode>(info.addressModeW);
			_anisotropyEnable = samplerInfo.anisotropyEnable = false;
			samplerInfo.maxAnisotropy = 1.0f;
			samplerInfo.borderColor = static_cast<VkBorderColor>(info.borderColor);
			samplerInfo.compareEnable = info.compareEnable;
			samplerInfo.compareOp = static_cast<VkCompareOp>(info.compareOp);
			samplerInfo.flags = static_cast<VkSamplerCreateFlags>(info.flag);
			samplerInfo.magFilter = static_cast<VkFilter>(info.magFilter);
			if (device->getDeviceFeatures().samplerAnisotropy) {
				samplerInfo.maxAnisotropy = device->getDeviceProps().limits.maxSamplerAnisotropy;
				_anisotropyEnable = samplerInfo.anisotropyEnable = true;
			}
			samplerInfo.maxLod = info.maxLod;
			samplerInfo.minFilter = static_cast<VkFilter>(info.minFilter);
			samplerInfo.mipLodBias = info.mipLodBias;
			samplerInfo.mipmapMode = static_cast<VkSamplerMipmapMode>(info.mipmapMode);
			samplerInfo.unnormalizedCoordinates = info.unnormalizedCoordinates;

			VK_CHECK_RESULT(vkCreateSampler(device->getLogicDevice(), &samplerInfo, nullptr, &_sampler));
		}
		VKSampler::~VKSampler()
		{
			auto context = std::dynamic_pointer_cast<VKContext>(Context::GetContext());
			auto device = std::dynamic_pointer_cast<VKDevice>(context->getDevice());
			vkDestroySampler(device->getLogicDevice(), _sampler, nullptr);
		}
	}
}
