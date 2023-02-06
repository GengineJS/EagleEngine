/*
 *  DescriptorSet.cpp
 *
 *  Copyright (c) 2022 by XiangKui Zheng
 *  This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */

#include <graphics/DescriptorSet.h>
#include <graphics/Context.h>
namespace eg {
	namespace graphics {
		DescriptorSet::DescriptorSet(const DescriptorSetInfo& info): _info(info)
		{
			_descPool.reset(info.descPool);
			for (auto layout: info.setLayouts) {
				_descLayouts.emplace_back(layout);
			}
		}
		DescriptorSet::~DescriptorSet()
		{
		}
		Buffer* DescriptorSet::getBuffer(uint32_t binding) const
		{
			if (_buffers.find(binding) == _buffers.end()) {
				return nullptr;
			}
			return _buffers.at(binding);
		}
		Texture* DescriptorSet::getTexture(uint32_t binding) const
		{
			if (_textures.find(binding) == _textures.end()) {
				return nullptr;
			}
			return _textures.at(binding);
		}
		PipelineLayout* DescriptorSet::getPipelineLayout() const
		{
			return _pipLayout.get();
		}
		void DescriptorSet::setBuffers(const std::unordered_map<uint32_t, Buffer*>& buffs)
		{
			_buffers = buffs;
		}
		void DescriptorSet::setTextures(const std::unordered_map<uint32_t, Texture*>& texs)
		{
			_textures = texs;
		}
		void DescriptorSet::bindBuffer(uint32_t binding, Buffer* buff)
		{
			assert(_buffers.find(binding) == _buffers.end() && _textures.find(binding) == _textures.end());
			_buffers.insert(std::make_pair(binding, buff));
		}
		void DescriptorSet::bindTexture(uint32_t binding, Texture* tex)
		{
			assert(_buffers.find(binding) == _buffers.end() && _textures.find(binding) == _textures.end());
			_textures.insert(std::make_pair(binding, tex));
		}
		void DescriptorSet::remove(uint32_t binding)
		{
			auto iterBuff = _buffers.find(binding);
			auto iterTex = _textures.find(binding);
			if (iterBuff != _buffers.end()) {
				_buffers.erase(iterBuff);
			}
			else if (iterTex != _textures.end()) {
				_textures.erase(iterTex);
			}
		}
		void DescriptorSet::clear()
		{
			_buffers.clear();
			_textures.clear();
		}
		void DescriptorSet::flush()
		{
			auto& device = Context::GetContext()->getDevice();
			std::vector<DescriptorPoolSize> poolSizes{};
			std::vector<DescriptorSetLayoutBinding> bindings{};
			_descWrites.clear();
			for (auto& kv: _buffers) {
				uint32_t bind = kv.first;
				auto buff = kv.second;
				if (!_info.descPool) {
					DescriptorPoolSize poolSize{};
					poolSize.type = buff->getBufferInfo().descType;
					poolSizes.emplace_back(std::move(poolSize));
				}
				if (_info.setLayouts.size() == 0) {
					DescriptorSetLayoutBinding layoutBind{};
					layoutBind.binding = bind;
					layoutBind.type = buff->getBufferInfo().descType;
					layoutBind.stage = buff->getBufferInfo().stage;
					bindings.emplace_back(std::move(layoutBind));
				}
				WriteDescriptorSet writeDesc{};
				writeDesc.descType = buff->getBufferInfo().descType;
				writeDesc.dstBinding = bind;
				writeDesc.pBuff = buff;
				_descWrites.emplace_back(std::move(writeDesc));
			}
			for (auto& kv: _textures) {
				uint32_t bind = kv.first;
				auto tex = kv.second;
				if (!_info.descPool) {
					DescriptorPoolSize poolSize{};
					poolSize.type = tex->getTextureInfo().descType;
					poolSizes.emplace_back(std::move(poolSize));
				}
				if (_info.setLayouts.size() == 0) {
					DescriptorSetLayoutBinding layoutBind{};
					layoutBind.binding = bind;
					layoutBind.type = tex->getTextureInfo().descType;
					layoutBind.stage = tex->getTextureInfo().stage;
					bindings.emplace_back(std::move(layoutBind));
				}
				WriteDescriptorSet writeDesc{};
				writeDesc.descType = tex->getTextureInfo().descType;
				writeDesc.dstBinding = bind;
				writeDesc.pTex = tex;
				_descWrites.emplace_back(std::move(writeDesc));
			}
			if (poolSizes.size() > 0) {
				DescriptorPoolInfo poolInfo{};
				poolInfo.pPoolSizes = std::move(poolSizes);
				_descPool = device->createDescriptorPool(poolInfo);
				_info.descPool = _descPool.get();
			}
			if (bindings.size() > 0) {
				DescriptorSetLayoutInfo layoutInfo{};
				layoutInfo.bindings = std::move(bindings);
				auto descLayout = device->createDescriptorSetLayout(layoutInfo);
				_info.setLayouts.emplace_back(descLayout.get());
				_descLayouts.emplace_back(std::move(descLayout));
			}
		}
	}
}
