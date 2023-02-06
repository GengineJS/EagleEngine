/*
 *  RenderPipeline.cpp
 *
 *  Copyright (c) 2022 by XiangKui Zheng
 *  This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */

#include <pipeline/RenderPipeline.h>

namespace eg {
	namespace pipeline {
		RenderPipeline::RenderPipeline()
		{
		}
		RenderPipeline::~RenderPipeline()
		{
		}
		void RenderPipeline::activate()
		{
			assert(_renderGraph->isPrepare());
			if (_context) {
				_context.reset();
			}
			_context = std::make_unique<GraphContext>(_renderGraph);
			_compiler->compie(_context);
		}
		void RenderPipeline::render()
		{
			_excutor->execute(_context);
		}
	}
}
