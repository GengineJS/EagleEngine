/*
 *  GraphExcutor.h
 *
 *  Copyright (c) 2022 by XiangKui Zheng
 *  This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */

#pragma once
#include <pipeline/render-graph/GraphContext.h>
namespace eg {
	namespace pipeline {
		class GraphExcutor
		{
		public:
			GraphExcutor();
			virtual ~GraphExcutor();
			void execute(const std::unique_ptr<GraphContext>& context);
		protected:
			bool _isExe{ false };
			GraphContext* _context{ nullptr };

		};
	}
}
