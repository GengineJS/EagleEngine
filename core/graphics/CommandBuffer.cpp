/*
 *  CommandBuffer.cpp
 *
 *  Copyright (c) 2022 by XiangKui Zheng
 *  This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */

#include <graphics/CommandBuffer.h>
#include <utils/Utils.h>
#include <assert.h>

namespace eg {
	namespace graphics {
		CommandBuffer::CommandBuffer(const CommandBufferInfo& info) : _info(info) {}

		void CommandBuffer::destroy()
		{
			reset();
		}

		void CommandBuffer::beginCommandBuffer()
		{
			reset();
		}
		void CommandBuffer::beginRenderPass(const RenderPassBeginInfo& info) {
			_isSetDynViewport = false;
			_isSetDynScissor = false;
			_currPass = info.renderPass;
			_renderArea = info.renderArea;
			_currFramebuffer = info.framebuffer;
		}
		void CommandBuffer::setViewport(const Viewport& viewport, uint32_t firstViewport, uint32_t viewportCount)
		{
			_isSetDynViewport = true;
		}
		CommandBuffer::~CommandBuffer()
		{
			destroy();
		}

		void CommandBuffer::reset() {
		}
		void CommandBuffer::setScissor(const Rect2D& scissor, uint32_t firstScissor, uint32_t scissorCount)
		{
			_isSetDynScissor = true;
		}
		void CommandBuffer::draw(const std::shared_ptr<VertexInput> vi)
		{
			auto dynViewport = std::find(_dynamics.begin(), _dynamics.end(), DynamicState::VIEWPORT);
			if (dynViewport != _dynamics.end() && !_isSetDynViewport) {
				setViewport({static_cast<float>(_renderArea.x), static_cast<float>(_renderArea.y), static_cast<float>(_renderArea.w), static_cast<float>(_renderArea.h)});
			}
			auto dynScissor = std::find(_dynamics.begin(), _dynamics.end(), DynamicState::SCISSOR);
			if (dynScissor != _dynamics.end() && !_isSetDynScissor) {
				setScissor(_renderArea);
			}
		}
	}
}
