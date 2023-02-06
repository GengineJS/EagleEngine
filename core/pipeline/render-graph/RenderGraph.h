/*
 *  RenderGraph.h
 *
 *  Copyright (c) 2022 by XiangKui Zheng
 *  This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */

#pragma once
#include <unordered_map>
#include <pipeline/render-graph/GraphPass.h>
#include <pipeline/render-graph/RenderResource.h>
#include <functional>
#include <graphics/CommandBuffer.h>

namespace eg {
	namespace pipeline {
		class RenderResource;
		class RenderGraph
		{
		public:
			RenderGraph();
			virtual ~RenderGraph();
			void beginFrame();
			void endFrame();
			void reset();
			inline bool isPrepare() const { return _isBegin && _isEnd; }
			GraphPass* addGraphPass(const std::string& name, const std::function<void(RenderVisitor&)> func, const PassView& view = {});
			GraphPass* addGraphPass(const std::string& name, const ShaderDesc& shader, const std::function<void(RenderVisitor&)> func, const PassView& view = {});
			GraphScreenPass* addScreenPass(const std::string& name, const std::function<void(RenderVisitor&)> func, const PassView& view = {});
			GraphScreenPass* addScreenPass(const std::string& name, const std::string& inputTex, const PassView& view = {});
			GraphScreenPass* addScreenPass(const std::string& name, const std::string& inputTex, const ShaderDesc& shader, const SlotDesc& slot = {});

			RenderGraph* addDepthStencil(const std::string& name, uint32_t width, uint32_t height, graphics::Format format = graphics::Format::D32_SFLOAT_S8_UINT);
			RenderGraph* addColorTexture(const std::string& name, graphics::Format format, uint32_t width, uint32_t height);
			RenderGraph* addInputBuffer(const std::string& name, const graphics::BufferInfo&);
			void visitorPass(std::function<bool(const GraphPass*)> passCallback);
			void visitorResource(std::function<bool(const class RenderTexture*)> resCallback, std::function<bool(const class RenderBuffer*)> buffCallback);
			
			GraphPass* getGraphPass(const std::string& name) const;
			GraphScreenPass* getScreenPass(const std::string& name) const;
			RenderResource* getResource(const std::string& name) const;
			inline auto& getPasses() const { return _passes; }
			inline auto& getScreenPasses() const { return _screenPasses; }
			inline const auto& getResources() const { return _resources; }
			
			inline void clearResources() { _resources.clear(); }
			inline void clearPasses() { _passes.clear(); }
			
		protected:
			bool _isBegin{ false };
			bool _isEnd{ false };
			std::unordered_map<std::string, std::unique_ptr<GraphPass>> _passes{};
			std::unordered_map<std::string, std::unique_ptr<GraphScreenPass>>  _screenPasses;
			std::unordered_map<std::string, std::unique_ptr<RenderResource>> _resources{};
			void _applyRenderTexture(const std::string& name, graphics::Format format, uint32_t width, uint32_t height, ResourceType type);
		};
	}
}