/*
 *  GraphContext.cpp
 *
 *  Copyright (c) 2022 by XiangKui Zheng
 *  This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */

#include <pipeline/render-graph/GraphContext.h>
#include <graphics/Tools.h>
namespace eg {
	namespace pipeline {
		GraphContext::GraphContext(const std::unique_ptr<RenderGraph>& renderGraph)
		{

			_gfxContext = graphics::Context::GetContext();
			setRenderGraph(renderGraph);
		}
		GraphContext::~GraphContext()
		{
			clearPasses();
			clearResources();
		}
		void GraphContext::setRenderGraph(const std::unique_ptr<RenderGraph>& renderGraph)
		{
			_renderGraph = renderGraph.get();
		}
		RenderGraph* GraphContext::getRenderGraph() const
		{
			return _renderGraph;
		}
		void GraphContext::addPass(std::unique_ptr<DeviceGraphPass> &&pass)
		{
			_passes.emplace_back(std::move(pass));
		}
		void GraphContext::addResource(std::unique_ptr<DeviceResource> &&res)
		{
			_resources.insert({ res->getReource()->getName(), std::move(res) });
		}
		void GraphContext::clearPasses()
		{
			_passes.clear();
		}
		void GraphContext::clearResources()
		{
			_resources.clear();
		}
		DeviceResource::DeviceResource(const RenderResource* res, const GraphContext* context)
		{
			_context = const_cast<GraphContext*>(context);
			setResource(res);
		}
		DeviceResource::~DeviceResource()
		{
		}
		void DeviceResource::setResource(const RenderResource* res)
		{
			_resorce = const_cast<RenderResource*>(res);
		}
		DeviceTexture::DeviceTexture(const RenderTexture* res, const GraphContext* context): DeviceResource(res, context)
		{	
			graphics::TextureUsageFlag usage = res->getResourceType() == ResourceType::TEXTURE ?
				graphics::TextureUsageFlag::COLOR_ATTACHMENT : graphics::TextureUsageFlag::DEPTH_STENCIL_ATTACHMENT;
			graphics::TextureInfo info(res->getWidth(), res->getHeight(), res->getFormat(),
				usage | graphics::TextureUsageFlag::SAMPLED | TextureUsageFlag::TRANSFER_SRC);
			_tex = _context->getGfxContext()->getDevice()->createTexture(info);
		}
		DeviceTexture::~DeviceTexture()
		{
		}
		DeviceBuffer::DeviceBuffer(const RenderBuffer* res, const GraphContext* context): DeviceResource(res, context)
		{
		}
		DeviceBuffer::~DeviceBuffer()
		{
		}

		std::unique_ptr<graphics::RenderPass> DeviceGraphPass::_createRenderPass(const PassView& view, const std::vector<graphics::Format>& colors, graphics::Format dStencil)
		{	
			auto colorTexLayout = _graphPass->isOnScreen() ? graphics::TextureLayout::PRESENT_SRC_KHR : graphics::TextureLayout::SHADER_READ_ONLY_OPTIMAL;
			std::vector<graphics::ColorAttachment> attachments(colors.size());
			uint32_t idx = 0;
			for (auto attachment: attachments) {
				attachment.format = colors.at(idx);
				attachment.samples = view.samples;
				attachment.finalLayout = colorTexLayout;
				if (view.clearFlag != ClearFlag::ALL && view.clearFlag != ClearFlag::COLOR) {
					attachment.loadOp = graphics::AttachmentLoadOp::LOAD;
					if (_graphPass->isOnScreen()) attachment.initLayout = graphics::TextureLayout::PRESENT_SRC_KHR;
				}
				idx++;
			}
			graphics::DepthStencilAttachment depthAttachment{};
			depthAttachment.format = dStencil;
			depthAttachment.samples = view.samples;
			if (view.clearFlag != ClearFlag::ALL && view.clearFlag != ClearFlag::DEPTH_STENCIL) {
				if (view.clearFlag == ClearFlag::COLOR || view.clearFlag == ClearFlag::STENCIL) {
					depthAttachment.loadOp = graphics::AttachmentLoadOp::LOAD;
					depthAttachment.initLayout = graphics::TextureLayout::DEPTH_READ_ONLY_STENCIL_ATTACHMENT_OPTIMAL;
				}
				if (view.clearFlag == ClearFlag::DEPTH) {
					depthAttachment.stencilLoadOp = graphics::AttachmentLoadOp::LOAD;
					depthAttachment.stencilStoreOp = graphics::AttachmentStoreOp::STORE;
					depthAttachment.initLayout = graphics::TextureLayout::DEPTH_READ_ONLY_STENCIL_ATTACHMENT_OPTIMAL;
				}
			}
			graphics::RenderPassInfo passInfo(attachments, depthAttachment);
			auto renderPass = _context->getGfxContext()->getDevice()->createRenderPass(passInfo);
			return renderPass;
		}

		// screen vertex data
		const std::vector<Vertex> vertices{
			{ {  1.0f,  1.0f, 0.0f }, { 0.0f, 0.0f, 0.0f }, {1.f, 1.f} },
			{ { 1.0f,  -1.0f, 0.0f }, { 0.0f, 0.0f, 0.0f }, {1.f, 0.f} },
			{ {  -1.0f, -1.0f, 0.0f }, { 0.0f, 0.0f, 0.0f }, {0.f, 0.f} },
			{ {  -1.0f, 1.0f, 0.0f }, { 0.0f, 0.0f, 0.0f }, {0.f, 1.f} }
		};
		// screen index data
		const std::vector<uint32_t> indices{
			0, 1, 2, 0, 2, 3
		};

		void DeviceGraphPass::_createScreenQuad()
		{
			
			VertexInputInfo viInfo{};
			viInfo.iaInfo.topology = PrimitiveTopology::TRIANGLE_LIST;
			viInfo.indexBuffer = indices;
			viInfo.vertexBuffer = vertices;
			_vertInput = _context->getGfxContext()->getDevice()->createVertexInput(viInfo);
		}
		void DeviceGraphPass::_createDescriptor()
		{
			for (auto& input: _graphPass->inputs) {
				auto& curRes = _context->getDeviceResources().at(input->getName());
				for (auto& slot: input->getSlots()) {
					DescriptorSet* ds{ nullptr };
					if (_descSets.find(slot.set) != _descSets.end()) {
						ds = _descSets.at(slot.set).get();
					}
					else {
						auto desc = _context->getGfxContext()->getDevice()->createDescriptorSet();
						ds = desc.get();
						_descSets.insert({ slot.set, std::move(desc) });
					}
					if (input->getResourceType() != ResourceType::BUFFER) {
						ds->bindTexture(slot.binding, dynamic_cast<DeviceTexture*>(curRes.get())->getTexture());
					}
					// TODO: buffer and external texture
					else {

					}
				}
			}
			std::vector<DescriptorSetLayout*> setLayouts{};
			for (auto& kv: _descSets) {
				auto& desc = kv.second;
				desc->flush();
				setLayouts.emplace_back(desc->getDescriptorSetInfo().setLayouts[0]);
			}
			_pipelineLayout = _context->getGfxContext()->getDevice()->createPipelineLayout(setLayouts);
		}
		void DeviceGraphPass::_createPipeline()
		{
			GraphicsPipelineInfo graphicsInfo{};
			graphicsInfo.vertexInput = _vertInput.get();
			graphicsInfo.shader = _shader.get();
			graphicsInfo.renderPass = _renderPass.get();
			graphicsInfo.pipelineLayout = _pipelineLayout.get();
			_pipeline = Context::GetContext()->getDevice()->createGraphicsPipeline(graphicsInfo);
		}
		DeviceGraphPass::DeviceGraphPass(const BaseGraphPass* graphPass, const GraphContext* context)
		{
			auto &shaderDesc = graphPass->getShaderDesc();
			// can put it in endFrame func check
			if (graphPass->isOnScreen() && shaderDesc.shaders.size() == 0 && graphPass->inputs.size() > 1) {
				assert(false);
			}
			_graphPass = const_cast<BaseGraphPass*>(graphPass);
			_context = const_cast<GraphContext*>(context);
			auto& swapchain = context->getGfxContext()->getSwapchain();
			auto& dRes = context->getDeviceResources();
			// must not be screenPass
			if (graphPass->outputs.size() > 0) {
				auto pass = dynamic_cast<GraphPass*>(_graphPass);
				std::vector<Texture*> cols{};
				std::vector<Format> colFormats{};
				Texture* depth{ nullptr };
				uint32_t width = swapchain->getWidth();
				uint32_t height = swapchain->getHeight();
				uint32_t idx = 0;
				for (auto outRes: graphPass->outputs) {
					auto curTex = dynamic_cast<RenderTexture*>(outRes);
					// Determines whether the current texture is the same size as the previous texture
					if (idx != 0) {
						assert(curTex->getWidth() == width && curTex->getHeight() == height);
					}
					DeviceTexture* deviceTex{ nullptr };
					if (dRes.find(curTex->getName()) != dRes.end()) {
						deviceTex = dynamic_cast<DeviceTexture*>(dRes.at(curTex->getName()).get());
					}
					else {
						auto devTex = std::make_unique<DeviceTexture>(curTex, context);
						deviceTex = devTex.get();
						_context->addResource(std::move(devTex));
					}
					if (curTex->getWidth() != width || curTex->getHeight() != height) {
						width = curTex->getWidth();
						height = curTex->getHeight();
					}
					if (curTex->getResourceType() == ResourceType::TEXTURE) {
						cols.emplace_back(deviceTex->getTexture());
						colFormats.emplace_back(curTex->getFormat());
					}
					else {
						depth = deviceTex->getTexture();
					}
					idx++;
				}
				if (!cols.size()) {
					graphics::TextureInfo texInfo(width,
						height, swapchain->getFormat(),
						TextureUsageFlag::COLOR_ATTACHMENT);
					_tempColTex = _context->getGfxContext()->getDevice()->createTexture(texInfo);
					cols.emplace_back(_tempColTex.get());
				}
				if (!depth) {
					 
                    auto dFormat = getSupportedDepthFormat(graphics::Format::D24_UNORM_S8_UINT);
					graphics::TextureInfo depthTexInfo(width,
						height,
						dFormat,
						graphics::TextureUsageFlag::DEPTH_STENCIL_ATTACHMENT);
					_tempDepTexs.emplace_back(_context->getGfxContext()->getDevice()->createTexture(depthTexInfo));
					depth = _tempDepTexs[0].get();
				}
				cols.emplace_back(depth);
				_renderPass = _createRenderPass(pass->getPassView(), colFormats, depth->getTextureInfo().format);
				graphics::FramebufferInfo fboInfo(width,
					height,
					_renderPass.get(), std::move(cols));
				_framebuffers.emplace_back(_context->getGfxContext()->getDevice()->createFramebuffer(fboInfo));
			}
			
			for (auto& curRes: _graphPass->inputs) {
				if (dRes.find(curRes->getName()) != dRes.end()) {
					if (curRes->getResourceType() != ResourceType::BUFFER) {
						_inputTexs.emplace_back(dynamic_cast<DeviceTexture*>(dRes.at(curRes->getName()).get()));
					}
				}
				// TODO: external buffer and texture
				else {

				}
			}
			
			if (graphPass->isOnScreen()) {
				auto screenPass = dynamic_cast<GraphScreenPass*>(const_cast<BaseGraphPass*>(graphPass));
				auto &view = screenPass->getPassView();
                auto dFormat = getSupportedDepthFormat(graphics::Format::D24_UNORM_S8_UINT);
				_renderPass = _createRenderPass(view, { swapchain->getFormat() }, dFormat);
				graphics::TextureInfo depthTexInfo(swapchain->getWidth(),
					swapchain->getHeight(),
					dFormat,
					graphics::TextureUsageFlag::DEPTH_STENCIL_ATTACHMENT);
				auto& colors = swapchain->getTextures();
				for (uint32_t i = 0; i < colors.size(); i++) {
					_tempDepTexs.emplace_back(_context->getGfxContext()->getDevice()->createTexture(depthTexInfo));
					auto attachs = { colors.at(i).get(), _tempDepTexs[i].get() };
					graphics::FramebufferInfo fboInfo(swapchain->getWidth(),
						swapchain->getHeight(),
						_renderPass.get(), attachs);
					_framebuffers.emplace_back(_context->getGfxContext()->getDevice()->createFramebuffer(fboInfo));
				}
			}
			if (!shaderDesc.shaders.empty()) {
				_shader = _context->getGfxContext()->getDevice()->createShader(shaderDesc);
				_createScreenQuad();
				_createDescriptor();
				_createPipeline();
			}
		}
		DeviceGraphPass::~DeviceGraphPass()
		{
			_graphPass->isValid(false);
			/*std::unordered_map<uint32_t, std::unique_ptr<DescriptorSet>>().swap(_descSets);
			std::vector<DeviceTexture*>().swap(_inputTexs);
			std::vector<std::unique_ptr<graphics::Framebuffer>>().swap(_framebuffers);
			std::vector<std::unique_ptr<Texture>>().swap(_tempDepTexs);*/
		}
		void DeviceGraphPass::execute(uint32_t cmdBuffId)
		{
			auto cmdBuff = _context->getGfxContext()->getCommandBuffers()[cmdBuffId].get();
			auto currFbo = _framebuffers[0].get();
			auto isScreen = _graphPass->isOnScreen();
			if (isScreen) {
				currFbo = _framebuffers[cmdBuffId].get();
			}
			auto width = currFbo->getFramebufferInfo().width;
			auto height = currFbo->getFramebufferInfo().height;
			
			graphics::RenderPassBeginInfo passInfo{};
			passInfo.clearColor = _graphPass->getPassView().clearVal.color;
			passInfo.clearDepthStencil = _graphPass->getPassView().clearVal.depthStencil;
			passInfo.framebuffer = currFbo;
			passInfo.renderPass = _renderPass.get();
			passInfo.renderArea = { 0, 0, width, height };
			_renderVisitor->_setContext(_context, cmdBuff, _renderPass.get(), currFbo);
			cmdBuff->beginRenderPass(passInfo);
			if (_shader) {
				for (auto& kv : _descSets) {
					cmdBuff->bindDescriptorSets(kv.first, kv.second.get());
				}
				cmdBuff->bindPipeline(_pipeline.get());
				cmdBuff->draw(_vertInput.get());
			}
			else if(isScreen && !_inputTexs.empty())
			{
				cmdBuff->copyTexture(_inputTexs[0]->getTexture(), currFbo->getFramebufferInfo().pAttachments[0]);
			}
			auto executeFunc = _graphPass->getRenderFunc();
			if(executeFunc) executeFunc(*_renderVisitor.get());
			cmdBuff->endRenderPass();
			

		}
	}
}
