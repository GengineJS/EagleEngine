#include <pipeline/render-graph/RenderGraph.h>

namespace eg {
	namespace pipeline {
		RenderGraph::RenderGraph()
		{
		}
		RenderGraph::~RenderGraph()
		{
		}
		void RenderGraph::beginFrame()
		{
			reset();
			_isBegin = true;
		}
		void RenderGraph::endFrame()
		{
			_isEnd = true;
		}
		void RenderGraph::reset()
		{
			_isEnd = false;
			_isBegin = false;
			clearPasses();
		}
		std::shared_ptr<GraphPass> RenderGraph::addGraphPass(const std::string& name, const std::function<void(std::shared_ptr<RenderVisitor>)> func, const PassView& view)
		{
			assert(_passes.find(name) == _passes.end());
			auto pass = std::make_shared<GraphPass>(name, shared_from_this(), func);
			pass->setPassView(view);
			_passes.insert(std::make_pair(name, pass));
			return pass;
		}
		std::shared_ptr<GraphPass> RenderGraph::addGraphPass(const std::string& name, const ShaderDesc& shader, const std::function<void(std::shared_ptr<RenderVisitor>)> func, const PassView& view)
		{
			assert(_passes.find(name) == _passes.end());
			auto pass = std::make_shared<GraphPass>(name, shared_from_this(), shader, func);
			pass->setPassView(view);
			_passes.insert(std::make_pair(name, pass));
			return pass;
		}
		std::shared_ptr<GraphScreenPass> RenderGraph::addScreenPass(const std::string& name, const std::function<void(std::shared_ptr<RenderVisitor>)> func, const PassView& view)
		{
			assert(_screenPasses.find(name) == _screenPasses.end());
			auto pass = std::make_shared<GraphScreenPass>(name, shared_from_this(), func);
			pass->setPassView(view);
			_screenPasses.insert(std::make_pair(name, pass));
			return pass;
		}
		std::shared_ptr<GraphScreenPass> RenderGraph::addScreenPass(const std::string& name, const std::string& inputTex, const PassView& view)
		{
			assert(_screenPasses.find(name) == _screenPasses.end());
			auto pass = std::make_shared<GraphScreenPass>(name, shared_from_this(), nullptr);
			pass->setPassView(view);
			pass->addRenderTexture(inputTex);
			_screenPasses.insert(std::make_pair(name, pass));
			return pass;
		}
		std::shared_ptr<GraphScreenPass> RenderGraph::addScreenPass(const std::string& name, const std::string& inputTex, const ShaderDesc& shader, const SlotDesc& slot)
		{
			assert(_screenPasses.find(name) == _screenPasses.end());
			auto pass = std::make_shared<GraphScreenPass>(name, shared_from_this(), shader, nullptr);
			pass->addRenderTexture(inputTex, slot);
			_screenPasses.insert(std::make_pair(name, pass));
			return pass;
		}
		std::shared_ptr<GraphPass> RenderGraph::getGraphPass(const std::string& name) const
		{
			if (_passes.find(name) == _passes.end()) {
				return nullptr;
			}
			return _passes.at(name);
		}
		std::shared_ptr<GraphScreenPass> RenderGraph::getScreenPass(const std::string& name) const
		{
			if (_screenPasses.find(name) == _screenPasses.end()) {
				return nullptr;
			}
			return _screenPasses.at(name);
		}
		std::shared_ptr<RenderResource> RenderGraph::getResource(const std::string& name) const
		{
			if (_resources.find(name) == _resources.end()) {
				return nullptr;
			}
			return _resources.at(name);
		}
		void RenderGraph::_applyRenderTexture(const std::string& name, graphics::Format format, uint32_t width, uint32_t height, ResourceType type)
		{	assert(_resources.find(name) == _resources.end());
			auto rTex = std::make_shared<RenderTexture>(type);
			rTex->apply(name, format, width, height);
			_resources.insert(std::make_pair(name, rTex));
		}
		std::shared_ptr<RenderGraph> RenderGraph::addDepthStencil(const std::string& name, uint32_t width, uint32_t height, graphics::Format format)
		{
			_applyRenderTexture(name, format, width, height, ResourceType::DEPTH_STENCIL);
			return shared_from_this();
		}
		std::shared_ptr<RenderGraph> RenderGraph::addColorTexture(const std::string& name, graphics::Format format, uint32_t width, uint32_t height)
		{
			_applyRenderTexture(name, format, width, height, ResourceType::TEXTURE);
			return shared_from_this();
		}
		std::shared_ptr<RenderGraph> RenderGraph::addInputBuffer(const std::string& name, const graphics::BufferInfo& buff)
		{
			assert(_resources.find(name) == _resources.end());
			auto rBuff = std::make_shared<RenderBuffer>();
			rBuff->apply(name, buff);
			_resources.insert(std::make_pair(name, rBuff));
			return shared_from_this();
		}
		void RenderGraph::visitorPass(std::function<bool(std::shared_ptr<GraphPass>)> passCallback)
		{
			for (auto& val: getPasses()) {
				const std::string& name = val.first;
				auto currPass = val.second;
				if (passCallback(currPass)) return;
			}
		}
		void RenderGraph::visitorResource(std::function<bool(std::shared_ptr<class RenderTexture>)> resCallback, std::function<bool(std::shared_ptr<class RenderBuffer>)> buffCallback)
		{
			for (auto& val : getResources()) {
				const std::string& name = val.first;
				auto currRes = val.second;
				switch (currRes->getResourceType())
				{
				case ResourceType::DEPTH_STENCIL:
				case ResourceType::TEXTURE:
					if (resCallback(std::dynamic_pointer_cast<RenderTexture>(currRes))) {
						return;
					}
					break;
				case ResourceType::BUFFER:
					if (buffCallback(std::dynamic_pointer_cast<RenderBuffer>(currRes))) {
						return;
					}
					break;
				default:
					break;
				}
			}
		}
	}
}