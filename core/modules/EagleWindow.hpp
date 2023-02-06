/*
 *  EagleModule.h
 *
 *  Copyright (c) 2022 by XiangKui Zheng
 *  This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */
#pragma once
#ifdef _WIN32
#pragma comment(linker, "/subsystem:windows")
#include <windows.h>
#include <fcntl.h>
#include <io.h>
#include <ShellScalingAPI.h>
#elif defined(USE_PLATFORM_ANDROID_KHR)
#include <android/native_activity.h>
#include <android/asset_manager.h>
#include <android_native_app_glue.h>
#include <sys/system_properties.h>
#endif
#include <modules/EagleModule.hpp>
#include <engine/EagleEngine.h>
#include <utils/Delegate.hpp>

namespace eg {
	struct WindowHandler
	{
		// true if application has focused, false if moved to background
		bool focused = false;
#if defined(_WIN32)
		HWND window;
		HINSTANCE windowInstance;
#elif defined(USE_PLATFORM_ANDROID_KHR)
		struct TouchPos {
			int32_t x;
			int32_t y;
		} touchPos;
		bool touchDown = false;
		double touchTimer = 0.0;
		int64_t lastTapTime = 0;
		android_app* androidApp{ nullptr };
#elif (defined(USE_PLATFORM_IOS_MVK) || defined(USE_PLATFORM_MACOS_MVK))
		void* view;
#if defined(USE_PLATFORM_IOS_MVK)
		uint32_t screenWidth = 0;
		uint32_t screenHeight = 0;
#endif
#endif
	};
	class EagleWindow: public EagleModule::Register<EagleWindow>
	{
		inline static const bool EagleWindow::IsRegister = Registar(ModuleStage::START);
		friend class InputSystem;
	public:
		EagleWindow() = default;
		~EagleWindow() = default;
		void execute(ModuleStage) override;
		bool peekMessage();
#if defined(_WIN32)
		static LRESULT CALLBACK WndProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
		void handleMessages(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
		HWND setup(HINSTANCE hinstance);
#elif defined(USE_PLATFORM_ANDROID_KHR)
#elif (defined(USE_PLATFORM_IOS_MVK) || defined(USE_PLATFORM_MACOS_MVK))
#elif (defined(USE_PLATFORM_IOS_MVK) || defined(USE_PLATFORM_MACOS_MVK))
#else
#endif
		inline const auto& getHandler() const { return _winHandler; }
		
		inline auto& onClose() { return _onClose; }
		inline auto& onSize() { return _onSize; }
		inline auto& onPaint() { return _onPaint; }
		inline auto& onReady() { return _onReady; }
		inline auto isMinimize() { return _isMinimize; }
	protected:
		inline auto& onKey() { return _onKey; }
		inline auto& onMouse() { return _onMouse; }
		inline auto& onMouseWheel() { return _onMouseWheel; }
		inline auto& onMouseMove() { return _onMouseMove; }
		uint32_t _destWidth{ 0 };
		uint32_t _destHeight{ 0 };
		WindowHandler _winHandler{};
		bool _isMinimize{ false };
		Delegate<void()> _onClose{};
		Delegate<void()> _onPaint{};
		Delegate<void(uint32_t w, uint32_t h)> _onSize{};
		Delegate<void(uint32_t code, bool isDown)> _onKey{};
		Delegate<void(uint32_t mouse, bool isDown, int32_t x, int32_t y)> _onMouse{};
		Delegate<void(short delta)> _onMouseWheel{};
		Delegate<void(int32_t x, int32_t y)> _onMouseMove{};
		Delegate<void(const WindowHandler&)> _onReady{};
	};
}
