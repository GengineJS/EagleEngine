/*
 *  RenderVisitor.h
 *
 *  Copyright (c) 2022 by XiangKui Zheng
 *  This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */

#pragma once
#include <graphics/Context.h>
#include <pipeline/PipelineDef.h>
#include <unordered_map>
namespace eg {
	namespace pipeline {
		class GraphContext;
		class RenderVisitor
		{
			friend class DeviceGraphPass;
		public:
			RenderVisitor();
			~RenderVisitor();
			void bindVertex(const std::vector<uint32_t>& indices,
				const std::vector<graphics::Vertex>& vertices,
				graphics::PrimitiveTopology topology = graphics::PrimitiveTopology::TRIANGLE_LIST,
				bool isPrimitiveRestart = false);
			void bindBuffer(const SlotDesc& slot, std::shared_ptr<graphics::Buffer>);
			void bindTexture(const SlotDesc& slot, std::shared_ptr<graphics::Texture>);
			void setShaderDesc(const graphics::ShaderDesc& desc);
			void setShaderDesc(const std::string& vert, const std::string& frag);
			void setViewport(const graphics::Viewport& viewport, uint32_t firstViewport = 0, uint32_t viewportCount = 1);
			void setScissor(const graphics::Rect2D& scissor, uint32_t firstScissor = 0, uint32_t scissorCount = 1);
			void draw();
			inline std::shared_ptr<GraphContext> getGraphContext() const { return _context; }
			inline std::shared_ptr<graphics::CommandBuffer> getCommandBuffer() const { return _cmdBuff; }
			inline std::shared_ptr<graphics::RenderPass> getRenderPass() const { return _renderPass; }
			inline std::shared_ptr<graphics::Framebuffer> getFramebuffer() const { return _framebuffer; }
		private:
			std::shared_ptr<GraphContext> _context{ nullptr };
			std::shared_ptr<graphics::CommandBuffer> _cmdBuff{ nullptr };
			std::shared_ptr<graphics::RenderPass> _renderPass{ nullptr };
			std::shared_ptr<graphics::Framebuffer> _framebuffer{ nullptr };
			struct RenderData {
				graphics::ShaderDesc _shaderDesc{};
				graphics::VertexInputInfo _vertInfo{};
				std::unordered_map<std::shared_ptr<graphics::Buffer>, SlotDesc> _buffers{};
				std::unordered_map<std::shared_ptr<graphics::Texture>, SlotDesc> _textures{};

				// graphics pipeline
				std::shared_ptr<graphics::VertexInput> _vi{ nullptr };
				std::shared_ptr<graphics::GraphicsPipeline> _pipeline{ nullptr };
				// set, desc
				std::unordered_map<uint32_t, std::shared_ptr<graphics::DescriptorSet>> _descs{};
				std::shared_ptr<graphics::PipelineLayout> _layout{ nullptr };
			} _currRenderData{};
			std::unordered_map<std::string, RenderData> _renderDatas{};
			void _bindTextureDesc();
			void _bindBufferDesc();
			bool _draw(const std::string& id);
			void _setContext(std::shared_ptr<GraphContext>, std::shared_ptr<graphics::CommandBuffer>,
				std::shared_ptr<graphics::RenderPass>, std::shared_ptr<graphics::Framebuffer>);
		};
	}
}
