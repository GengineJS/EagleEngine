/*
 *  RenderGraph.cpp
 *
 *  Copyright (c) 2022 by XiangKui Zheng
 *  This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */

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
		GraphPass* RenderGraph::addGraphPass(const std::string& name, const std::function<void(RenderVisitor&)> func, const PassView& view)
		{
			assert(_passes.find(name) == _passes.end());
			auto pass = std::make_unique<GraphPass>(name, this, func);
			pass->setPassView(view);
			auto ptr = pass.get();
			_passes.insert(std::make_pair(name, std::move(pass)));
			return ptr;
		}
		GraphPass* RenderGraph::addGraphPass(const std::string& name, const ShaderDesc& shader, const std::function<void(RenderVisitor&)> func, const PassView& view)
		{
			assert(_passes.find(name) == _passes.end());
			auto pass = std::make_unique<GraphPass>(name, this, shader, func);
			pass->setPassView(view);
			auto ptr = pass.get();
			_passes.insert(std::make_pair(name, std::move(pass)));
			return ptr;
		}
		GraphScreenPass* RenderGraph::addScreenPass(const std::string& name, const std::function<void(RenderVisitor&)> func, const PassView& view)
		{
			assert(_screenPasses.find(name) == _screenPasses.end());
			auto pass = std::make_unique<GraphScreenPass>(name, this, func);
			pass->setPassView(view);
			auto ptr = pass.get();
			_screenPasses.insert(std::make_pair(name, std::move(pass)));
			return ptr;
		}
		GraphScreenPass* RenderGraph::addScreenPass(const std::string& name, const std::string& inputTex, const PassView& view)
		{
			assert(_screenPasses.find(name) == _screenPasses.end());
			auto pass = std::make_unique<GraphScreenPass>(name, this, nullptr);
			pass->setPassView(view);
			pass->addRenderTexture(inputTex);
			auto ptr = pass.get();
			_screenPasses.insert(std::make_pair(name, std::move(pass)));
			return ptr;
		}
		GraphScreenPass* RenderGraph::addScreenPass(const std::string& name, const std::string& inputTex, const ShaderDesc& shader, const SlotDesc& slot)
		{
			assert(_screenPasses.find(name) == _screenPasses.end());
			auto pass = std::make_unique<GraphScreenPass>(name, this, shader, nullptr);
			pass->addRenderTexture(inputTex, slot);
			auto ptr = pass.get();
			_screenPasses.insert(std::make_pair(name, std::move(pass)));
			return ptr;
		}
		GraphPass* RenderGraph::getGraphPass(const std::string& name) const
		{
			if (_passes.find(name) == _passes.end()) {
				return nullptr;
			}
			return _passes.at(name).get();
		}
		GraphScreenPass* RenderGraph::getScreenPass(const std::string& name) const
		{
			if (_screenPasses.find(name) == _screenPasses.end()) {
				return nullptr;
			}
			return _screenPasses.at(name).get();
		}
		RenderResource* RenderGraph::getResource(const std::string& name) const
		{
			if (_resources.find(name) == _resources.end()) {
				return nullptr;
			}
			return _resources.at(name).get();
		}
		void RenderGraph::_applyRenderTexture(const std::string& name, graphics::Format format, uint32_t width, uint32_t height, ResourceType type)
		{	assert(_resources.find(name) == _resources.end());
			auto rTex = std::make_unique<RenderTexture>(type);
			rTex->apply(name, format, width, height);
			_resources.insert(std::make_pair(name, std::move(rTex)));
		}
		RenderGraph* RenderGraph::addDepthStencil(const std::string& name, uint32_t width, uint32_t height, graphics::Format format)
		{
			_applyRenderTexture(name, format, width, height, ResourceType::DEPTH_STENCIL);
			return this;
		}
		RenderGraph* RenderGraph::addColorTexture(const std::string& name, graphics::Format format, uint32_t width, uint32_t height)
		{
			_applyRenderTexture(name, format, width, height, ResourceType::TEXTURE);
			return this;
		}
		RenderGraph* RenderGraph::addInputBuffer(const std::string& name, const graphics::BufferInfo& buff)
		{
			assert(_resources.find(name) == _resources.end());
			auto rBuff = std::make_unique<RenderBuffer>();
			rBuff->apply(name, buff);
			_resources.insert(std::make_pair(name, std::move(rBuff)));
			return this;
		}
		void RenderGraph::visitorPass(std::function<bool(const GraphPass*)> passCallback)
		{
			for (auto& val: getPasses()) {
				const std::string& name = val.first;
				auto& currPass = val.second;
				if (passCallback(currPass.get())) return;
			}
		}
		void RenderGraph::visitorResource(std::function<bool(const RenderTexture*)> resCallback, std::function<bool(const RenderBuffer*)> buffCallback)
		{
			for (auto& val : getResources()) {
				const std::string& name = val.first;
				auto& currRes = val.second;
				switch (currRes->getResourceType())
				{
				case ResourceType::DEPTH_STENCIL:
				case ResourceType::TEXTURE:
					if (resCallback(dynamic_cast<RenderTexture*>(currRes.get()))) {
						return;
					}
					break;
				case ResourceType::BUFFER:
					if (buffCallback(dynamic_cast<RenderBuffer*>(currRes.get()))) {
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