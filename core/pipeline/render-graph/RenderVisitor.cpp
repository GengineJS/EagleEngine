#include <pipeline/render-graph/RenderVisitor.h>
#include <pipeline/render-graph/GraphContext.h>
namespace eg {
	namespace pipeline {
		RenderVisitor::RenderVisitor()
		{
		}
		RenderVisitor::~RenderVisitor()
		{
		}
		void RenderVisitor::bindVertex(const std::vector<uint32_t>& indices, const std::vector<graphics::Vertex>& vertices, graphics::PrimitiveTopology topology, bool isPrimitiveRestart)
		{
			graphics::VertexInputInfo vertInfo{};
			vertInfo.indexBuffer = indices;
			vertInfo.vertexBuffer = vertices;
			vertInfo.iaInfo.isPrimitiveRestart = isPrimitiveRestart;
			vertInfo.iaInfo.topology = topology;
			_currRenderData._vertInfo = vertInfo;
		}
		void RenderVisitor::bindBuffer(const SlotDesc& slot, std::shared_ptr<graphics::Buffer> buff)
		{
			if (_currRenderData._buffers.find(buff) != _currRenderData._buffers.end()) {
				return;
			}
			_currRenderData._buffers.insert(std::make_pair(buff, slot));
		}
		void RenderVisitor::bindTexture(const SlotDesc& slot, std::shared_ptr<graphics::Texture> tex)
		{
			if (_currRenderData._textures.find(tex) != _currRenderData._textures.end()) {
				return;
			}
			_currRenderData._textures.insert(std::make_pair(tex, slot));
		}
		void RenderVisitor::setShaderDesc(const graphics::ShaderDesc& desc)
		{
			_currRenderData._shaderDesc = desc;
		}
		void RenderVisitor::setShaderDesc(const std::string& vert, const std::string& frag)
		{
			graphics::ShaderDesc shader{};
			shader.add(vert, graphics::ShaderStageFlag::VERTEX);
			shader.add(frag, graphics::ShaderStageFlag::FRAGMENT);
			setShaderDesc(shader);
		}
		void RenderVisitor::setViewport(const graphics::Viewport& viewport, uint32_t firstViewport, uint32_t viewportCount)
		{
			_cmdBuff->setViewport(viewport, firstViewport, viewportCount);
		}
		void RenderVisitor::setScissor(const graphics::Rect2D& scissor, uint32_t firstScissor, uint32_t scissorCount)
		{
			_cmdBuff->setScissor(scissor, firstScissor, scissorCount);
		}
		void RenderVisitor::draw()
		{
			assert(_currRenderData._shaderDesc.shaders.size() && _currRenderData._vertInfo.vertexBuffer.size());
			std::string shaderName{};
			for (auto v :_currRenderData._shaderDesc.shaders) {
				shaderName += v.first;
			}
			if (_draw(shaderName)) return;
			auto device = _context->getGfxContext()->getDevice();
			_currRenderData._vi = device->createVertexInput(_currRenderData._vertInfo);
			auto shader = device->createShader(_currRenderData._shaderDesc);
			_bindBufferDesc();
			_bindTextureDesc();
			std::vector<std::shared_ptr<DescriptorSetLayout>> setLayouts{};
			for (auto kv : _currRenderData._descs) {
				auto desc = kv.second;
				desc->flush();
				setLayouts.emplace_back(desc->getDescriptorSetInfo().setLayouts[0]);
			}
			_currRenderData._layout = device->createPipelineLayout(setLayouts);
			graphics::GraphicsPipelineInfo graphicsInfo{};
			graphicsInfo.vertexInput = _currRenderData._vi;
			graphicsInfo.shader = shader;
			graphicsInfo.renderPass = _renderPass;
			graphicsInfo.pipelineLayout = _currRenderData._layout;
			_currRenderData._pipeline = device->createGraphicsPipeline(graphicsInfo);
			_renderDatas.insert({ shaderName, _currRenderData});
			_draw(shaderName);
		}
		inline void RenderVisitor::_bindTextureDesc()
		{
			auto device = _context->getGfxContext()->getDevice();
			for (auto v : _currRenderData._textures) {
				auto slot = v.second;
				std::shared_ptr<graphics::DescriptorSet> desc{ nullptr };
				if (_currRenderData._descs.find(slot.set) != _currRenderData._descs.end()) {
					desc = _currRenderData._descs.at(slot.set);
				}
				else {
					desc = device->createDescriptorSet();
					_currRenderData._descs.insert({ slot.set, desc });
				}
				desc->bindTexture(slot.binding, v.first);
			}
		}
		void RenderVisitor::_bindBufferDesc()
		{
			auto device = _context->getGfxContext()->getDevice();
			for (auto v : _currRenderData._buffers) {
				auto slot = v.second;
				std::shared_ptr<graphics::DescriptorSet> desc{ nullptr };
				if (_currRenderData._descs.find(slot.set) != _currRenderData._descs.end()) {
					desc = _currRenderData._descs.at(slot.set);
				}
				else {
					desc = device->createDescriptorSet();
					_currRenderData._descs.insert({ slot.set, desc });
				}
				desc->bindBuffer(slot.binding, v.first);
			}
		}
		bool RenderVisitor::_draw(const std::string& id)
		{	
			auto shaderIter = _renderDatas.find(id);
			if (shaderIter != _renderDatas.end()) {
				auto renderData = _renderDatas.at(id);
				_cmdBuff->bindPipeline(renderData._pipeline);
				for (auto desc: renderData._descs) {
					_cmdBuff->bindDescriptorSets(desc.first, desc.second);
				}
				// TODO: dynamic vi
				_cmdBuff->draw(renderData._vi);
				return true;
			}
			return false;
		}
		void RenderVisitor::_setContext(std::shared_ptr<GraphContext> context, std::shared_ptr<graphics::CommandBuffer> cmdBuff,
			std::shared_ptr<graphics::RenderPass> pass, std::shared_ptr<graphics::Framebuffer> fb)
		{
			_context = context;
			_cmdBuff = cmdBuff;
			_renderPass = pass;
			_framebuffer = fb;
		}
	}
}
