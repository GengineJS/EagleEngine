#include <pipeline/render-graph/GraphContext.h>
#include <graphics/Tools.h>
namespace eg {
	namespace pipeline {
		GraphContext::GraphContext(std::shared_ptr<RenderGraph> renderGraph)
		{
			_gfxContext = graphics::Context::GetContext();
			_device = _gfxContext-> getDevice();
			setRenderGraph(renderGraph);
		}
		GraphContext::~GraphContext()
		{
			resetPasses();
			clearResources();
		}
		void GraphContext::setRenderGraph(std::shared_ptr<RenderGraph> renderGraph)
		{
			_renderGraph = renderGraph;
		}
		std::shared_ptr<RenderGraph> GraphContext::getRenderGraph() const
		{
			return _renderGraph;
		}
		void GraphContext::addPass(std::shared_ptr<DeviceGraphPass> pass)
		{
			_passes.emplace_back(pass);
		}
		void GraphContext::addResource(std::shared_ptr<DeviceResource> res)
		{
			_resources.insert({ res->getReource()->getName(), res });
		}
		void GraphContext::resetPasses()
		{
			for(auto& pass: _passes) {
				pass->getGraphPass()->isValid(false);
			}
		}
		void GraphContext::clearResources()
		{
			_resources.clear();
		}
		DeviceResource::DeviceResource(std::shared_ptr<RenderResource> res, std::shared_ptr<GraphContext> context)
		{
			_context = context;
			setResource(res);
		}
		DeviceResource::~DeviceResource()
		{
		}
		void DeviceResource::setResource(std::shared_ptr<RenderResource> res)
		{
			_resorce = res;
		}
		DeviceTexture::DeviceTexture(std::shared_ptr<RenderTexture> res, std::shared_ptr<GraphContext> context): DeviceResource(res, context)
		{	
			graphics::TextureUsageFlag usage = res->getResourceType() == ResourceType::TEXTURE ?
				graphics::TextureUsageFlag::COLOR_ATTACHMENT : graphics::TextureUsageFlag::DEPTH_STENCIL_ATTACHMENT;
			graphics::TextureInfo info(res->getWidth(), res->getHeight(), res->getFormat(),
				usage | graphics::TextureUsageFlag::SAMPLED | TextureUsageFlag::TRANSFER_SRC);
			_tex = _context->getDevice()->createTexture(info);
		}
		DeviceTexture::~DeviceTexture()
		{
		}
		DeviceBuffer::DeviceBuffer(std::shared_ptr<RenderBuffer> res, std::shared_ptr<GraphContext> context): DeviceResource(res, context)
		{
		}
		DeviceBuffer::~DeviceBuffer()
		{
		}

		std::shared_ptr<graphics::RenderPass> DeviceGraphPass::_createRenderPass(const PassView& view, const std::vector<graphics::Format>& colors, graphics::Format dStencil)
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
			auto renderPass = _context->getDevice()->createRenderPass(passInfo);
			return renderPass;
		}
		void DeviceGraphPass::_createScreenQuad()
		{
			// vertex data
			const std::vector<Vertex> vertices{
				{ {  1.0f,  1.0f, 0.0f }, { 0.0f, 0.0f, 0.0f }, {1.f, 1.f} },
				{ { 1.0f,  -1.0f, 0.0f }, { 0.0f, 0.0f, 0.0f }, {1.f, 0.f} },
				{ {  -1.0f, -1.0f, 0.0f }, { 0.0f, 0.0f, 0.0f }, {0.f, 0.f} },
				{ {  -1.0f, 1.0f, 0.0f }, { 0.0f, 0.0f, 0.0f }, {0.f, 1.f} }
			};
			// index data
			const std::vector<uint32_t> indices{
				0, 1, 2, 0, 2, 3
			};
			VertexInputInfo viInfo{};
			viInfo.iaInfo.topology = PrimitiveTopology::TRIANGLE_LIST;
			viInfo.indexBuffer = indices;
			viInfo.vertexBuffer = vertices;
			_vertInput = _context->getDevice()->createVertexInput(viInfo);
		}
		void DeviceGraphPass::_createDescriptor()
		{
			for (auto input: _graphPass->inputs) {
				auto curRes = _context->getDeviceResources().at(input->getName());
				for (auto& slot: input->getSlots()) {
					std::shared_ptr<DescriptorSet> ds{ nullptr };
					if (_descSets.find(slot.set) != _descSets.end()) {
						ds = _descSets.at(slot.set);
					}
					else {
						ds = _context->getDevice()->createDescriptorSet();
						_descSets.insert({ slot.set, ds });
					}
					if (input->getResourceType() != ResourceType::BUFFER) {
						ds->bindTexture(slot.binding, std::dynamic_pointer_cast<DeviceTexture>(curRes)->getTexture());
					}
					// TODO: buffer and external texture
					else {

					}
				}
			}
			std::vector<std::shared_ptr<DescriptorSetLayout>> setLayouts{};
			for (auto kv: _descSets) {
				auto desc = kv.second;
				desc->flush();
				setLayouts.emplace_back(desc->getDescriptorSetInfo().setLayouts[0]);
			}
			_pipelineLayout = _context->getDevice()->createPipelineLayout(setLayouts);
		}
		void DeviceGraphPass::_createPipeline()
		{
			GraphicsPipelineInfo graphicsInfo{};
			graphicsInfo.vertexInput = _vertInput;
			graphicsInfo.shader = _shader;
			graphicsInfo.renderPass = _renderPass;
			graphicsInfo.pipelineLayout = _pipelineLayout;
			_pipeline = Context::GetContext()->getDevice()->createGraphicsPipeline(graphicsInfo);
		}
		DeviceGraphPass::DeviceGraphPass(std::shared_ptr<BaseGraphPass> graphPass, std::shared_ptr<class GraphContext> context)
		{
			auto &shaderDesc = graphPass->getShaderDesc();
			// can put it in endFrame func check
			if (graphPass->isOnScreen() && shaderDesc.shaders.size() == 0 && graphPass->inputs.size() > 1) {
				assert(false);
			}
			_graphPass = graphPass;
			_context = context;
			auto swapchain = context->getGfxContext()->getSwapchain();
			auto dRes = context->getDeviceResources();
			// must not be screenPass
			if (graphPass->outputs.size() > 0) {
				auto pass = std::dynamic_pointer_cast<GraphPass>(graphPass);
				std::vector<std::shared_ptr<Texture>> cols{};
				std::vector<Format> colFormats{};
				std::shared_ptr<Texture> depth{ nullptr };
				uint32_t width = swapchain->getWidth();
				uint32_t height = swapchain->getHeight();
				uint32_t idx = 0;
				for (auto outRes: graphPass->outputs) {
					auto curTex = std::dynamic_pointer_cast<RenderTexture>(outRes);
					// Determines whether the current texture is the same size as the previous texture
					if (idx != 0) {
						assert(curTex->getWidth() == width && curTex->getHeight() == height);
					}
					std::shared_ptr<DeviceTexture> deviceTex{ nullptr };
					if (dRes.find(curTex->getName()) != dRes.end()) {
						deviceTex = std::dynamic_pointer_cast<DeviceTexture>(dRes.at(curTex->getName()));
					}
					else {
						deviceTex = std::make_shared<DeviceTexture>(curTex, context);
						context->addResource(deviceTex);
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
					cols.emplace_back(context->getDevice()->createTexture(texInfo));
				}
				if (!depth) {
                    auto dFormat = getSupportedDepthFormat(graphics::Format::D24_UNORM_S8_UINT);
					graphics::TextureInfo depthTexInfo(width,
						height,
						dFormat,
						graphics::TextureUsageFlag::DEPTH_STENCIL_ATTACHMENT);
					depth = context->getDevice()->createTexture(depthTexInfo);
				}
				cols.emplace_back(depth);
				_renderPass = _createRenderPass(pass->getPassView(), colFormats, depth->getTextureInfo().format);
				graphics::FramebufferInfo fboInfo(width,
					height,
					_renderPass, cols);
				_framebuffers.emplace_back(context->getDevice()->createFramebuffer(fboInfo));
			}
			
			for (auto curRes: _graphPass->inputs) {
				if (dRes.find(curRes->getName()) != dRes.end()) {
					if (curRes->getResourceType() != ResourceType::BUFFER) {
						_inputTexs.emplace_back(std::dynamic_pointer_cast<DeviceTexture>(dRes.at(curRes->getName())));
					}
				}
				// TODO: external buffer and texture
				else {

				}
			}
			
			if (graphPass->isOnScreen()) {
				auto screenPass = std::dynamic_pointer_cast<GraphScreenPass>(graphPass);
				auto &view = screenPass->getPassView();
                auto dFormat = getSupportedDepthFormat(graphics::Format::D24_UNORM_S8_UINT);
				_renderPass = _createRenderPass(view, { swapchain->getFormat() }, dFormat);
				graphics::TextureInfo depthTexInfo(swapchain->getWidth(),
					swapchain->getHeight(),
					dFormat,
					graphics::TextureUsageFlag::DEPTH_STENCIL_ATTACHMENT);
				auto& colors = swapchain->getTextures();
				for (uint32_t i = 0; i < colors.size(); i++) {
					graphics::FramebufferInfo fboInfo(swapchain->getWidth(),
						swapchain->getHeight(),
						_renderPass, {colors.at(i),
						context->getDevice()->createTexture(depthTexInfo)});
					_framebuffers.emplace_back(context->getDevice()->createFramebuffer(fboInfo));
				}
			}
			if (!shaderDesc.shaders.empty()) {
				_shader = context->getDevice()->createShader(shaderDesc);
				_createScreenQuad();
				_createDescriptor();
				_createPipeline();
			}
		}
		DeviceGraphPass::~DeviceGraphPass()
		{
			_graphPass->isValid(false);
		}
		void DeviceGraphPass::execute(uint32_t cmdBuffId)
		{
			auto cmdBuff = _context->getGfxContext()->getCommandBuffers()[cmdBuffId];
			auto currFbo = _framebuffers[0];
			auto isScreen = _graphPass->isOnScreen();
			if (isScreen) {
				currFbo = _framebuffers[cmdBuffId];
			}
			auto width = currFbo->getFramebufferInfo().width;
			auto height = currFbo->getFramebufferInfo().height;
			
			graphics::RenderPassBeginInfo passInfo{};
			passInfo.clearColor = _graphPass->getPassView().clearVal.color;
			passInfo.clearDepthStencil = _graphPass->getPassView().clearVal.depthStencil;
			passInfo.framebuffer = currFbo;
			passInfo.renderPass = _renderPass;
			passInfo.renderArea = { 0, 0, width, height };
			_renderVisitor->_setContext(_context, cmdBuff, _renderPass, currFbo);
			cmdBuff->beginRenderPass(passInfo);
			if (_shader) {
				for (auto kv : _descSets) {
					cmdBuff->bindDescriptorSets(kv.first, kv.second);
				}
				cmdBuff->bindPipeline(_pipeline);
				cmdBuff->draw(_vertInput);
			}
			else if(isScreen && !_inputTexs.empty())
			{
				cmdBuff->copyTexture(_inputTexs[0]->getTexture(), currFbo->getFramebufferInfo().pAttachments[0]);
			}
			auto executeFunc = _graphPass->getRenderFunc();
			if(executeFunc) executeFunc(_renderVisitor);
			cmdBuff->endRenderPass();
			

		}
	}
}
