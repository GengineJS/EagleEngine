/*
 *  GraphCompiler.h
 *
 *  Copyright (c) 2022 by XiangKui Zheng
 *  This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */

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