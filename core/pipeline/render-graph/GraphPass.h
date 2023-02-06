/*
 *  GraphPass.h
 *
 *  Copyright (c) 2022 by XiangKui Zheng
 *  This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */

#pragma once
#include <string>
#include <unordered_map>
#include <vector>
#include <pipeline/PipelineDef.h>
#include <functional>
#include <graphics/CommandBuffer.h>
#include <pipeline/render-graph/RenderVisitor.h>

namespace eg {
	namespace pipeline {
		class RenderGraph;
		class RenderResource;
		using namespace graphics;

		class BaseGraphPass
		{
		public:
			BaseGraphPass(const std::string& name, bool isOnScreen = false);
			virtual ~BaseGraphPass() = default;
			// which resources are used as input by the current pass
			std::vector<RenderResource*> inputs{};
			// which resources are output by the current pass
			std::vector<RenderResource*> outputs{};
			const bool isOnScreen() const { return _isOnScreen; }
			void addRenderBuffer(const std::string& ref, const SlotDesc& desc = {});
			void addRenderTexture(const std::string& ref, const SlotDesc& desc = {});
			inline const ShaderDesc& getShaderDesc() const { return _shader; }
			inline void setRenderFunc(const std::function<void(RenderVisitor&)>& callback);
			inline void setShaderDesc(const ShaderDesc& desc);
			inline void setName(const std::string& name) { _name = name; }
			void setPassView(const PassView&);
			inline void setRenderGraph(const RenderGraph* graph) { _graph = const_cast<RenderGraph*>(graph); }
			inline const RenderGraph* getRenderGraph() const { return _graph; }
			inline const std::string& getName() const { return _name; }
			inline void isValid(bool valid) { _isValid = valid; }
			inline bool getValid() const { return _isValid; }
			inline const std::vector<std::string>& getResources() const { return _resources; }
			inline const std::function<void(RenderVisitor&)>& getRenderFunc() const {
				return _func;
			}
			inline const PassView& getPassView() const { return _view; }
		protected:
			bool _isOnScreen{ false };
			std::vector<std::string> _resources;
			ShaderDesc _shader{};
			RenderGraph* _graph{ nullptr };
			bool _isValid{ false };
			std::string _name{""};
			std::function<void(RenderVisitor&)> _func;
			PassView _view{};
		};
		class GraphPass : public BaseGraphPass
		{
		public:
			GraphPass(const std::string& name, const RenderGraph* graph, const std::function<void(RenderVisitor&)>& callback);
			GraphPass(const std::string& name, const RenderGraph* graph, const ShaderDesc& shader, const std::function<void(RenderVisitor&)>& callback);
			~GraphPass() override;
			void addRenderTexture(const std::string& ref, AccessState access, const SlotDesc& desc = {});
		};
		class GraphScreenPass : public BaseGraphPass {
		public:
			GraphScreenPass(const std::string& name, const RenderGraph* graph, const std::function<void(RenderVisitor&)>& callback);
			GraphScreenPass(const std::string& name, const RenderGraph* graph, const ShaderDesc& shader, const std::function<void(RenderVisitor&)>& callback);
			~GraphScreenPass() override;
		};
	}
}