/*
 *  RenderVisitor.cpp
 *
 *  Copyright (c) 2022 by XiangKui Zheng
 *  This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */

#include <pipeline/render-graph/RenderVisitor.h>
#include <pipeline/render-graph/GraphContext.h>
namespace eg {
	namespace pipeline {
		RenderVisitor::RenderVisitor()
		{
		}
		RenderVisitor::~RenderVisitor()
		{
			// std::unordered_map<std::string, std::unique_ptr<RenderData>>().swap(_renderDatas);
		}
		void RenderVisitor::bindVertex(const std::vector<uint32_t>& indices, const std::vector<Vertex>& vertices, PrimitiveTopology topology, bool isPrimitiveRestart)
		{
			_genRenderData();
			_currRenderData->vertInfo.indexBuffer = indices;
			_currRenderData->vertInfo.vertexBuffer = vertices;
			_currRenderData->vertInfo.iaInfo.isPrimitiveRestart = isPrimitiveRestart;
			_currRenderData->vertInfo.iaInfo.topology = topology;
		}
		void RenderVisitor::bindBuffer(const SlotDesc& slot, Buffer* buff)
		{
			_genRenderData();
			if (_currRenderData->buffers.find(buff) != _currRenderData->buffers.end()) {
				return;
			}
			_currRenderData->buffers.insert(std::make_pair(buff, slot));
		}
		void RenderVisitor::bindTexture(const SlotDesc& slot, Texture* tex)
		{
			_genRenderData();
			if (_currRenderData->textures.find(tex) != _currRenderData->textures.end()) {
				return;
			}
			_currRenderData->textures.insert(std::make_pair(tex, slot));
		}
		void RenderVisitor::setShaderDesc(const ShaderDesc& desc)
		{
			_genRenderData();
			_currRenderData->shaderDesc = desc;
		}
		void RenderVisitor::setShaderDesc(const std::string& vert, const std::string& frag)
		{
			_genRenderData();
			ShaderDesc shader{};
			shader.add(vert, ShaderStageFlag::VERTEX);
			shader.add(frag, ShaderStageFlag::FRAGMENT);
			setShaderDesc(shader);
		}
		void RenderVisitor::setViewport(const Viewport& viewport, uint32_t firstViewport, uint32_t viewportCount)
		{
			_genRenderData();
			_cmdBuff->setViewport(viewport, firstViewport, viewportCount);
		}
		void RenderVisitor::setScissor(const Rect2D& scissor, uint32_t firstScissor, uint32_t scissorCount)
		{
			_genRenderData();
			_cmdBuff->setScissor(scissor, firstScissor, scissorCount);
		}
		void RenderVisitor::draw()
		{
			_genRenderData();
			assert(_currRenderData->shaderDesc.shaders.size() && _currRenderData->vertInfo.vertexBuffer.size());
			std::string shaderName{};
			for (auto v :_currRenderData->shaderDesc.shaders) {
				shaderName += v.first;
			}
			if (_draw(shaderName)) return;
			auto device = _context->getGfxContext()->getDevice().get();
			_currRenderData->vi = device->createVertexInput(_currRenderData->vertInfo);
			auto shader = device->createShader(_currRenderData->shaderDesc);
			_bindBufferDesc();
			_bindTextureDesc();
			std::vector<DescriptorSetLayout*> setLayouts{};
			for (auto& kv : _currRenderData->descs) {
				auto& desc = kv.second;
				desc->flush();
				setLayouts.emplace_back(desc->getDescriptorSetInfo().setLayouts[0]);
			}
			_currRenderData->layout = device->createPipelineLayout(setLayouts);
			GraphicsPipelineInfo graphicsInfo{};
			graphicsInfo.vertexInput = _currRenderData->vi.get();
			graphicsInfo.shader = shader.get();
			graphicsInfo.renderPass = _renderPass;
			graphicsInfo.pipelineLayout = _currRenderData->layout.get();
			_currRenderData->pipeline = device->createGraphicsPipeline(graphicsInfo);
			_renderDatas.insert({ shaderName, std::move(_currRenderData)});
			_draw(shaderName);
		}
		inline void RenderVisitor::_bindTextureDesc()
		{
			auto device = _context->getGfxContext()->getDevice().get();
			for (auto& v : _currRenderData->textures) {
				auto& slot = v.second;
				DescriptorSet* desc{ nullptr };
				if (_currRenderData->descs.find(slot.set) != _currRenderData->descs.end()) {
					desc = _currRenderData->descs.at(slot.set).get();
				}
				else {
					auto descPtr = device->createDescriptorSet();
					desc = descPtr.get();
					_currRenderData->descs.insert({ slot.set, std::move(descPtr)});
				}
				desc->bindTexture(slot.binding, v.first);
			}
		}
		void RenderVisitor::_bindBufferDesc()
		{
			auto device = _context->getGfxContext()->getDevice().get();
			for (auto& v : _currRenderData->buffers) {
				auto& slot = v.second;
				DescriptorSet* desc{ nullptr };
				if (_currRenderData->descs.find(slot.set) != _currRenderData->descs.end()) {
					desc = _currRenderData->descs.at(slot.set).get();
				}
				else {
					auto descPtr = device->createDescriptorSet();
					desc = descPtr.get();
					_currRenderData->descs.insert({ slot.set, std::move(descPtr) });
				}
				desc->bindBuffer(slot.binding, v.first);
			}
		}
		void RenderVisitor::_genRenderData()
		{
			if (!_currRenderData) _currRenderData.reset(new RenderData());
		}
		bool RenderVisitor::_draw(const std::string& id)
		{	
			auto shaderIter = _renderDatas.find(id);
			if (shaderIter != _renderDatas.end()) {
				auto& renderData = _renderDatas.at(id);
				_cmdBuff->bindPipeline(renderData->pipeline.get());
				for (auto& desc: renderData->descs) {
					_cmdBuff->bindDescriptorSets(desc.first, desc.second.get());
				}
				// TODO: dynamic vi
				_cmdBuff->draw(renderData->vi.get());
				return true;
			}
			return false;
		}
		void RenderVisitor::_setContext(const GraphContext* context, const CommandBuffer* cmdBuff,
			const RenderPass* pass, const Framebuffer* fb)
		{
			_context = const_cast<GraphContext*>(context);
			_cmdBuff = const_cast<CommandBuffer*>(cmdBuff);
			_renderPass = const_cast<RenderPass*>(pass);
			_framebuffer = const_cast<Framebuffer*>(fb);
		}
	}
}
