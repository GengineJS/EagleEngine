/*
 *  EagleRenderer.cpp
 *
 *  Copyright (c) 2022 by XiangKui Zheng
 *  This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */

#include "EagleApplication.h"
#include "EagleRenderer.h"
namespace eg {
	const std::string EagleRenderer::ON_START = "GAME_ON_START";
	const std::string EagleRenderer::ON_UPDATE = "GAME_ON_UPDATE";
	const std::string EagleRenderer::ON_RENDER_BEFORE = "GAME_ON_RENDER_BEFORE";
	const std::string EagleRenderer::ON_RENDER_AFTER = "GAME_ON_RENDER_AFTER";
	const std::string EagleRenderer::ON_WINDOW_RESIZE = "GAME_ON_WINDOW_RESIZE";
	EagleRenderer::EagleRenderer(const ContextInfo& info)
	{
		_context = Context::CreateContext(info);
	}

	EagleRenderer::~EagleRenderer()
	{
	}

	void EagleRenderer::onApplicationInit(std::shared_ptr<EagleApplication> app)
{
        auto handler = app->getHandler();
#if defined(USE_PLATFORM_ANDROID_KHR)
		
		_context->getContextInfo().width = ANativeWindow_getWidth(handler.androidApp->window);
		_context->getContextInfo().height = ANativeWindow_getHeight(handler.androidApp->window);
#elif defined(USE_PLATFORM_IOS_MVK)
        _context->getContextInfo().width = handler.screenWidth;
        _context->getContextInfo().height = handler.screenHeight;
#endif
		_context->onApplicationInit(app->getHandler());
	}

	void EagleRenderer::onApplicationExit(std::shared_ptr<EagleApplication> app) {

	}

	void EagleRenderer::onApplicationPause()
	{
	}

	void EagleRenderer::onApplicationFocus()
	{
	}

	void EagleRenderer::_active() {
        _pipeline->activate();
	}

	void EagleRenderer::onStart()
	{
        _active();
		emit(ON_START, this);

	}
	void EagleRenderer::onUpdate()
	{
		ContextInfo info = _context->getContextInfo();
		float ms = static_cast<float>(1000.f / info.fps);
		emit(ON_UPDATE, this);
	}
	void EagleRenderer::onRender()
	{
		emit(ON_RENDER_BEFORE, this);
		_pipeline->render();
		emit(ON_RENDER_AFTER, this);
		
	}
	void EagleRenderer::onResize(uint32_t w, uint32_t h)
	{
		_context->onResize(w, h);
		_pipeline->activate();
		emit(ON_WINDOW_RESIZE, this, w, h);
	}
}
