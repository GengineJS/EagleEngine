#pragma once
#include <pipeline/render-graph/GraphContext.h>
namespace eg {
	namespace pipeline {
		class GraphExcutor
		{
		public:
			GraphExcutor();
			virtual ~GraphExcutor();
			void execute(std::shared_ptr<GraphContext> context);
		protected:
			bool _isExe{ false };
			std::shared_ptr<GraphContext> _context{ nullptr };

		};
	}
}
