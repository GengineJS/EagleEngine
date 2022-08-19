#pragma once
#include <pipeline/render-graph/GraphContext.h>
namespace eg {
	namespace pipeline {
		class GraphCompiler
		{
		public:
			GraphCompiler();
			virtual ~GraphCompiler();
			void compie(std::shared_ptr<GraphContext> context);
		protected:
			std::shared_ptr<GraphContext> _context{ nullptr };
			void _linkPasses(std::shared_ptr<BaseGraphPass> pass);
		};
	}
}