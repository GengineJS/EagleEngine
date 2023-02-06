/*
 *  GraphExcutor.cpp
 *
 *  Copyright (c) 2022 by XiangKui Zheng
 *  This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */

#include <pipeline/render-graph/GraphExcutor.h>

namespace eg {
	namespace pipeline {
		GraphExcutor::GraphExcutor()
		{
		}
		GraphExcutor::~GraphExcutor()
		{
		}
		void GraphExcutor::execute(const std::unique_ptr<GraphContext>& context)
		{
			auto gfxContext = context->getGfxContext();
			auto& cmdBuffs = gfxContext->getCommandBuffers();
			auto& swapchain = gfxContext->getSwapchain();
			auto& passes = context->getRenderPasses();
			swapchain->acquireFrame();
			auto idx = swapchain->getFrameIdx();
			cmdBuffs[idx]->beginCommandBuffer();
			for (auto& pass : passes) {
				if (pass->getGraphPass()->getValid()) {
					pass->execute(idx);
				}
			}
			cmdBuffs[idx]->endCommandBuffer();
			swapchain->presentFrame(cmdBuffs[idx]);
		}
	}
}
