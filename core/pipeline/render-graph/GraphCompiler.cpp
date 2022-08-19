#include <pipeline/render-graph/GraphCompiler.h>
namespace eg {
	namespace pipeline {
		GraphCompiler::GraphCompiler()
		{
		}
		GraphCompiler::~GraphCompiler()
		{
		}
		void GraphCompiler::compie(std::shared_ptr<GraphContext> context)
		{
			// clear resources and set valid to false
			if (_context) {
                _context->resetPasses();
				_context->clearResources();
			}
			_context = context;
			auto renderGraph = context->getRenderGraph();
			auto screenPasses = renderGraph->getScreenPasses();
			assert(screenPasses.size());
			for (auto kv: screenPasses) {
				auto graphPass = kv.second;
				_linkPasses(graphPass);
			}
		}
		void GraphCompiler::_linkPasses(std::shared_ptr<BaseGraphPass> pass)
		{
			for (auto resource : pass->inputs) {
				for (auto curPass: resource->outputs) {
					// The same resource may be used multiple times by the same pass
					if (!curPass->getValid()) {
						_linkPasses(curPass);
					}
				 }
			}
			if (!pass->getValid()) {
				pass->isValid(true);
				_context->addPass(std::make_shared<DeviceGraphPass>(pass, _context));
			}
		}
	}
}
