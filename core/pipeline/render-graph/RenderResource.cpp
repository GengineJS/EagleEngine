#include <pipeline/render-graph/RenderResource.h>
#include <assert.h>
namespace eg {
	namespace pipeline {
		RenderResource::RenderResource(ResourceType type)
		{
			_type = type;
		}
		RenderResource::~RenderResource()
		{
		}
		void RenderResource::setType(ResourceType type)
		{
			_type = type;
		}
		void RenderResource::addSlot(const SlotDesc& slot)
		{
			for (auto curSlot:_slots) {
				assert(!(curSlot.set == slot.set && curSlot.binding == slot.binding));
			}
			_slots.emplace_back(slot);
		}
		RenderTexture::RenderTexture(ResourceType type): RenderResource(type)
		{
		}
		RenderTexture::~RenderTexture()
		{
		}
		void RenderTexture::apply(const std::string& name, graphics::Format format, uint32_t width, uint32_t height)
		{
			_name = name;
			_format = format;
			_width = width;
			_height = height;
		}
		RenderBuffer::RenderBuffer(): RenderResource(ResourceType::BUFFER)
		{
		}
		RenderBuffer::~RenderBuffer()
		{
		}
		void RenderBuffer::apply(const std::string& name, const graphics::BufferInfo& info)
		{
			_name = name;
			_info = info;
		}
	}
}