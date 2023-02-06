/*
 *  GraphPass.cpp
 *
 *  Copyright (c) 2022 by XiangKui Zheng
 *  This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */

#include <pipeline/render-graph/GraphPass.h>
#include <pipeline/render-graph/RenderGraph.h>

namespace eg {
	namespace pipeline {
		// GraphPass
		GraphPass::GraphPass(const std::string& name, const RenderGraph* graph, const std::function<void(RenderVisitor&)>& callback): BaseGraphPass(name)
		{
			setRenderGraph(graph);
			setRenderFunc(callback);
		}
		GraphPass::GraphPass(const std::string& name, const RenderGraph* graph, const ShaderDesc& shader, const std::function<void(RenderVisitor&)>& callback): BaseGraphPass(name)
		{
			setRenderGraph(graph);
			setShaderDesc(shader);
			setRenderFunc(callback);
		}
		GraphPass::~GraphPass()
		{
		}
		// BaseGraphPass
		void BaseGraphPass::setPassView(const PassView& view)
		{
			_view = view;
		}
		void GraphPass::addRenderTexture(const std::string& ref, AccessState access, const SlotDesc& desc)
		{	
			auto res = _graph->getResource(ref);
			assert(res);
			switch (access)
			{
			case AccessState::READ:
				if (res->getResourceType() == ResourceType::BUFFER) {
					addRenderBuffer(ref, desc);
				}
				else {
					BaseGraphPass::addRenderTexture(ref, desc);
				}
				break;
			case AccessState::READ_WRITE:
				BaseGraphPass::addRenderTexture(ref, desc);
			case AccessState::WRITE:
				res->outputs.emplace_back(this);
				outputs.emplace_back(res);
				break;
			default:
				break;
			}
		}
		void BaseGraphPass::addRenderBuffer(const std::string& buff, const SlotDesc& desc)
		{
			auto res = _graph->getResource(buff);
			assert(res);
			res->addSlot(desc);
			auto iter = std::find(std::begin(_resources), std::end(_resources), buff);
			if (iter == std::end(_resources))
			{
				res->inputs.emplace_back(this);
				inputs.emplace_back(res);
				_resources.emplace_back(buff);
			}
		}
		void BaseGraphPass::addRenderTexture(const std::string& ref, const SlotDesc& desc)
		{
			auto res = _graph->getResource(ref);
			assert(res);
			res->addSlot(desc);
			auto iter = std::find(std::begin(_resources), std::end(_resources), ref);
			if (iter == std::end(_resources))
			{
				res->inputs.emplace_back(this);
				inputs.emplace_back(res);
				_resources.emplace_back(ref);
			}
		}
		inline void BaseGraphPass::setRenderFunc(const std::function<void(RenderVisitor&)>& callback)
		{
			_func = callback;
		}
		inline void BaseGraphPass::setShaderDesc(const ShaderDesc& desc)
		{
			_shader = desc;
		}
		BaseGraphPass::BaseGraphPass(const std::string& name, bool isOnScreen)
		{
			_name = name;
			_isOnScreen = isOnScreen;
		}
		GraphScreenPass::GraphScreenPass(const std::string& name, const RenderGraph* graph, const std::function<void(RenderVisitor&)>& callback): BaseGraphPass(name, true)
		{
			setRenderGraph(graph);
			setRenderFunc(callback);
		}
		GraphScreenPass::GraphScreenPass(const std::string& name, const RenderGraph* graph, const ShaderDesc& shader, const std::function<void(RenderVisitor&)>& callback): BaseGraphPass(name, true)
		{
			setRenderGraph(graph);
			setShaderDesc(shader);
			setRenderFunc(callback);
		}
		GraphScreenPass::~GraphScreenPass()
		{
		}
	}
}