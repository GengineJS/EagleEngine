#include <graphics/vulkan/VKDescriptorSet.h>
#include <graphics/vulkan/VKContext.h>
#include <graphics/vulkan/VKDevice.h>
#include <vulkan/vulkan.h>
#include <graphics/vulkan/VKDescriptorSetLayout.h>
#include <assert.h>
#include <graphics/vulkan/VKDescriptorPool.h>
#include <graphics/vulkan/VKTools.h>
#include <graphics/vulkan/VKBuffer.h>
#include <graphics/vulkan/VKTexture.h>
#include <graphics/vulkan/VKSampler.h>
namespace eg {
	namespace graphics {
		VKDescriptorSet::VKDescriptorSet(const DescriptorSetInfo& info): DescriptorSet(info) {
			create();
		}
		VKDescriptorSet::~VKDescriptorSet()
		{
			destroy();
		}

		void VKDescriptorSet::destroy()
		{
			auto context = Context::GetContext();
			auto device = std::dynamic_pointer_cast<VKDevice>(context->getDevice());
			auto descPool = std::dynamic_pointer_cast<VKDescriptorPool>(_info.descPool);
			vkFreeDescriptorSets(device->getLogicDevice(), descPool->getVkDescriptorPool(), static_cast<uint32_t>(_descriptorSets.size()), _descriptorSets.data());
		}

		void VKDescriptorSet::create()
		{
			auto context = Context::GetContext();
			auto device = std::dynamic_pointer_cast<VKDevice>(context->getDevice());
			VkDescriptorSetAllocateInfo allocInfo{};
			allocInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_ALLOCATE_INFO;
			std::vector<VkDescriptorSetLayout> layouts{};
			for (auto descLayout: _info.setLayouts) {
				auto  setLayout = std::dynamic_pointer_cast<VKDescriptorSetLayout>(descLayout);
				layouts.emplace_back(setLayout->getVkDescriptorSetLayout());
			}
			allocInfo.pSetLayouts = layouts.data();
			allocInfo.descriptorSetCount = _info.descSetCount;
			assert(_info.descPool != nullptr);
			allocInfo.descriptorPool = std::dynamic_pointer_cast<VKDescriptorPool>(_info.descPool)->getVkDescriptorPool();
			_descriptorSets.resize(_info.descSetCount);
			VK_CHECK_RESULT(vkAllocateDescriptorSets(device->getLogicDevice(), &allocInfo, _descriptorSets.data()));
			PipelineLayoutInfo pipInfo{};
			pipInfo.pSetLayouts = _info.setLayouts;
			_pipLayout = device->createPipelineLayout(pipInfo);
		}

		void VKDescriptorSet::flush()
		{
			DescriptorSet::flush();
			if (_descriptorSets.size() == 0) create();
			for (auto &desc: _descriptorSets) {
				std::vector<VkWriteDescriptorSet> descriptorWrites(_descWrites.size());
				uint32_t idx = 0;
				for (auto& descWrite : _descWrites) {
					descriptorWrites.at(idx).sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
					descriptorWrites.at(idx).dstSet = desc;
					descriptorWrites.at(idx).dstBinding = descWrite.dstBinding;
					descriptorWrites.at(idx).dstArrayElement = descWrite.dstArrayElement;
					descriptorWrites.at(idx).descriptorCount = descWrite.descCount;
					descriptorWrites.at(idx).descriptorType = static_cast<VkDescriptorType>(descWrite.descType);
					if (descWrite.pBuff) {
						VkDescriptorBufferInfo buffInfo{};
						auto buf = std::dynamic_pointer_cast<VKBuffer>(descWrite.pBuff);
						buffInfo.buffer = buf->getVkBuffer();
						buffInfo.offset = descWrite.pBuff->getBufferInfo().offset;
						buffInfo.range = descWrite.pBuff->getBufferInfo().size;
						descriptorWrites.at(idx).pBufferInfo = &buffInfo;
					}
					else if (descWrite.pTex) {
						VkDescriptorImageInfo imageInfo{};
						auto tex = std::dynamic_pointer_cast<VKTexture>(descWrite.pTex);
						imageInfo.imageLayout = VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL;
						imageInfo.imageView = tex->getVkImageView();
						auto sampler = std::dynamic_pointer_cast<VKSampler>(descWrite.pTex->getSampler());
						imageInfo.sampler = sampler->getVkSampler();
						descriptorWrites.at(idx).pImageInfo = &imageInfo;
					}
					idx++;
				}
				auto context = Context::GetContext();
				auto device = std::dynamic_pointer_cast<VKDevice>(context->getDevice());
				vkUpdateDescriptorSets(device->getLogicDevice(), static_cast<uint32_t>(descriptorWrites.size()), descriptorWrites.data(), 0, nullptr);
			}
		}
	}
}
