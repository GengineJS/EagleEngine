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
			_context = std::make_shared<GraphContext>(_renderGraph);
			_compiler->compie(_context);
		}
		void RenderPipeline::render()
		{
			_excutor->execute(_context);
		}
	}
}
