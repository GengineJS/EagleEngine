/*
 *  RenderResource.h
 *
 *  Copyright (c) 2022 by XiangKui Zheng
 *  This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */

#pragma once
#include <unordered_map>
#include <pipeline/PipelineDef.h>
#include <functional>
#include <vector>
#include <pipeline/render-graph/GraphPass.h>
namespace eg {
	namespace pipeline {
		class RenderResource
		{
		public:
			RenderResource(ResourceType type);
			virtual ~RenderResource();
			// Which passes use the resource
			std::vector<std::shared_ptr<BaseGraphPass>> inputs{};
			// which passes output the resource
			std::vector< std::shared_ptr<BaseGraphPass>> outputs{};
			void setType(ResourceType type);
			inline const std::string& getName() const { return _name; }
			inline ResourceType getResourceType() const { return _type; }
			void addSlot(const SlotDesc& slot);
			inline const auto& getSlots() const { return _slots; }
		protected:
			ResourceType _type{ ResourceType::UNKNOWN };
			std::string _name{""};
			std::vector<SlotDesc> _slots{};
		};

		class RenderTexture : public RenderResource {
		public:
			RenderTexture(ResourceType type = ResourceType::TEXTURE);
			~RenderTexture() override;
			void apply(const std::string& name, graphics::Format format, uint32_t width, uint32_t height);
			inline graphics::Format getFormat() const { return _format; }
			inline uint32_t getWidth() const { return _width; }
			inline uint32_t getHeight() const { return _height; }
		protected:
			graphics::Format _format{ graphics::Format::B8G8R8A8_UNORM };
			uint32_t _width{0};
			uint32_t _height{0};
		};

		class RenderBuffer : public RenderResource {
		public:
			RenderBuffer();
			~RenderBuffer() override;
			void apply(const std::string& name, const graphics::BufferInfo& info);
			inline const graphics::BufferInfo& getBufferInfo() const { return _info; }
		protected:
			graphics::BufferInfo _info;
		};
	}
}