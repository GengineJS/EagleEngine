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
	using namespace graphics;
	namespace pipeline {
		class GraphContext;
		class RenderData {
		public:
			~RenderData() = default;
			ShaderDesc shaderDesc{};
			VertexInputInfo vertInfo{};
			std::unordered_map<Buffer*, SlotDesc> buffers{};
			std::unordered_map<Texture*, SlotDesc> textures{};

			// graphics pipeline
			std::unique_ptr<VertexInput> vi{ nullptr };
			std::unique_ptr<GraphicsPipeline> pipeline{ nullptr };
			// set, desc
			std::unordered_map<uint32_t, std::unique_ptr<DescriptorSet>> descs{};
			std::unique_ptr<PipelineLayout> layout{ nullptr };
		};
		class RenderVisitor
		{
			friend class DeviceGraphPass;
		public:
			RenderVisitor();
			~RenderVisitor();
			void bindVertex(const std::vector<uint32_t>& indices,
				const std::vector<Vertex>& vertices,
				PrimitiveTopology topology = PrimitiveTopology::TRIANGLE_LIST,
				bool isPrimitiveRestart = false);
			void bindBuffer(const SlotDesc& slot, Buffer* buff);
			void bindBuffer(const SlotDesc& slot, std::unique_ptr<Buffer>&& buff);
			void bindTexture(const SlotDesc& slot, Texture*);
			void setShaderDesc(const ShaderDesc& desc);
			void setShaderDesc(const std::string& vert, const std::string& frag);
			void setViewport(const Viewport& viewport, uint32_t firstViewport = 0, uint32_t viewportCount = 1);
			void setScissor(const Rect2D& scissor, uint32_t firstScissor = 0, uint32_t scissorCount = 1);
			void draw();
			inline const GraphContext* getGraphContext() const { return _context; }
			inline const CommandBuffer* getCommandBuffer() const { return _cmdBuff; }
			inline const RenderPass* getRenderPass() const { return _renderPass; }
			inline const Framebuffer* getFramebuffer() const { return _framebuffer; }
		private:
			GraphContext* _context{ nullptr };
			CommandBuffer* _cmdBuff{ nullptr };
			RenderPass* _renderPass{ nullptr };
			Framebuffer* _framebuffer{ nullptr };
			std::unique_ptr<RenderData> _currRenderData{std::make_unique<RenderData>()};
			std::unordered_map<std::string, std::unique_ptr<RenderData>> _renderDatas{};
			void _bindTextureDesc();
			void _bindBufferDesc();
			void _genRenderData();
			bool _draw(const std::string& id);
			void _setContext(const GraphContext*, const CommandBuffer*,
				const RenderPass*, const Framebuffer*);
		};
	}
}
