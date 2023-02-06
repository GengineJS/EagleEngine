/*
 *  DescriptorSet.h
 *
 *  Copyright (c) 2022 by XiangKui Zheng
 *  This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */

#pragma once
#include <graphics/DescriptorSetLayout.h>
#include <graphics/DescriptorPool.h>
#include <vector>
#include <graphics/Buffer.h>
#include <graphics/Texture.h>
#include <unordered_map>
#include <assert.h>
#include <graphics/PipelineLayout.h>

namespace eg {
	namespace graphics {
		struct DescriptorSetInfo {
			DescriptorPool* descPool{ nullptr };
			uint32_t descSetCount{ 1 };
			std::vector<DescriptorSetLayout*> setLayouts{};
		};

		class DescriptorSet {
		public:
			DescriptorSet() = default;
			DescriptorSet(const DescriptorSetInfo& info);
			virtual ~DescriptorSet();
			virtual void create() = 0;
			inline const DescriptorSetInfo& getDescriptorSetInfo() const { return _info; }
			Buffer* getBuffer(uint32_t binding) const;
			Texture* getTexture(uint32_t binding) const;
			PipelineLayout* getPipelineLayout() const;
			virtual void destroy() = 0;

			void setBuffers(const std::unordered_map<uint32_t, Buffer*>&);
			void setTextures(const std::unordered_map<uint32_t, Texture*>&);
			void bindBuffer(uint32_t binding, Buffer*);
			void bindTexture(uint32_t binding, Texture*);
			void remove(uint32_t binding);
			void clear();
			virtual void flush();
		protected:
			DescriptorSetInfo _info{};
			std::unique_ptr<DescriptorPool> _descPool{ nullptr };
			std::vector<std::unique_ptr<DescriptorSetLayout>> _descLayouts{};
			std::unique_ptr<PipelineLayout> _pipLayout{ nullptr };
			struct WriteDescriptorSet
			{
				uint32_t       dstBinding;
				uint32_t       dstArrayElement{ 0 };
				uint32_t       descCount{ 1 };
				DescriptorType descType;
				Texture*	pTex{nullptr};
				Buffer*	   pBuff{ nullptr };
				BufferView*    pBuffView{ nullptr };
			};
			std::vector<WriteDescriptorSet> _descWrites{};
			// binding and buffer
			std::unordered_map<uint32_t, Buffer*> _buffers{};
			std::unordered_map<uint32_t, Texture*> _textures{};
			// TODO
			std::unordered_map<uint32_t, BufferView*> _buffViews{};
		};
	}
}