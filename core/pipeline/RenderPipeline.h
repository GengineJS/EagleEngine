/*
 *  RenderPipeline.h
 *
 *  Copyright (c) 2022 by XiangKui Zheng
 *  This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */

#pragma once
#include <pipeline/render-graph/RenderGraph.h>
#include <pipeline/render-graph/GraphCompiler.h>
#include <pipeline/render-graph/GraphExcutor.h>
#include <pipeline/render-graph/GraphContext.h>
namespace eg {
	namespace pipeline {
		class RenderPipeline
		{
		public:
			RenderPipeline();
			virtual ~RenderPipeline();
			void activate();
			void render();
			inline const std::unique_ptr<RenderGraph>& getRenderGraph() const { return _renderGraph; }
			inline const std::unique_ptr<GraphContext>& getGraphContext() const { return _context; }
		protected:
			std::unique_ptr<RenderGraph> _renderGraph = std::make_unique<RenderGraph>();
			std::unique_ptr<GraphContext> _context{ nullptr };
			std::unique_ptr<GraphCompiler> _compiler = std::make_unique<GraphCompiler>();
			std::unique_ptr<GraphExcutor> _excutor = std::make_unique<GraphExcutor>();
		};
	}
}