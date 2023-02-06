#include <EagleApplication.h>

// vertex data
const std::vector<Vertex> vertices{
	{ {  1.0f,  1.0f, 0.0f }, { 1.0f, 0.0f, 0.0f }, {1.f, 0.f} },
	{ {  0.0f, -1.0f, 0.0f }, { 0.0f, 0.0f, 1.0f }, {0.5f, 1.f} },
	{ { -1.0f,  1.0f, 0.0f }, { 0.0f, 1.0f, 0.0f }, {0.f, 0.f} },

};
//// index data
const std::vector<uint32_t> indices{
	0, 1, 2
};
struct {
	glm::mat4 projectionMatrix{glm::mat4(1.0f)};
	glm::mat4 modelMatrix{ glm::mat4(1.0f) };
	glm::mat4 viewMatrix{ glm::mat4(1.0f) };
} uboVS;
std::unique_ptr<Buffer> uboBuff{ nullptr };
//the app entry point
std::unique_ptr<EagleEngine> EagleSetup() {
	EngineInfo info{};
	info.applicationName = "Blur";
	info.applicationVersion = 1.12f;
	auto engine = std::make_unique<EagleEngine>(info);
	auto renderer = engine->getModule<EagleRenderer>();
	auto& pipeline = renderer->getRenderPipeline();
	auto& renderGraph = pipeline->getRenderGraph();
	engine->onAfterExecute().Add([&](ModuleStage stage) {
		if (stage == ModuleStage::START) {
			const auto& device = EagleRenderer::Get()->getContext()->getDevice();
			BufferInfo buff(sizeof(uboVS),
				0,
				BufferUsageFlag::UNIFORM,
				MemoryPropertyFlag::HOST_COHERENT | MemoryPropertyFlag::HOST_VISIBLE);
			uboBuff = device->createBuffer(buff);
			uboBuff->update(&uboVS);
		}
	});
	renderGraph->beginFrame();
	std::string texName = "TriangleTex";
	renderGraph->addColorTexture(texName, Format::B8G8R8A8_UNORM, info.width, info.height);
	auto sourcePass = renderGraph->addGraphPass("SourcePass", [&](RenderVisitor& visitor) {
		visitor.setShaderDesc(FileUtils::getShadersPath()
			+ "blur/triangle.vert.spv", FileUtils::getShadersPath()
			+ "blur/triangle.frag.spv");
		visitor.bindVertex(indices, vertices);
		visitor.bindBuffer({ 0, 0 }, uboBuff.get());
		visitor.draw();
		});
	sourcePass->addRenderTexture(texName, AccessState::WRITE);
	ShaderDesc shaderDesc{};
	shaderDesc.add(FileUtils::getShadersPath()
		+ "blur/blur.vert.spv", ShaderStageFlag::VERTEX);
	shaderDesc.add(FileUtils::getShadersPath()
		+ "blur/blur.frag.spv", ShaderStageFlag::FRAGMENT);
	renderGraph->addScreenPass("OnScreen", texName, shaderDesc, { 0, 1 });
	renderGraph->endFrame();
	return engine;
}
EAGLE_MAIN();