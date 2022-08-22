/*
 *  Swapchain.h
 *
 *  Copyright (c) 2022 by XiangKui Zheng
 *  This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */

#pragma once
#include <graphics/GraphicsDef.h>
#include <graphics/Texture.h>
#include <graphics/Surface.h>

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
#include <vector>
#include <graphics/CommandBuffer.h>
namespace eg {
	namespace graphics {
		struct AppHandler
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
			android_app* androidApp{nullptr};
#elif (defined(USE_PLATFORM_IOS_MVK) || defined(USE_PLATFORM_MACOS_MVK))
			void* view;
#if defined(USE_PLATFORM_IOS_MVK)
            uint32_t screenWidth = 0;
            uint32_t screenHeight = 0;
#endif
#endif
		};


		class  Swapchain
		{
		public:
			Swapchain();
			virtual ~Swapchain();
			virtual void create(const AppHandler& handler) = 0;
			virtual void destroy();
			inline const std::vector<std::shared_ptr<Texture>> getTextures() const { return _textures; }
			inline std::shared_ptr<Surface> getSurface() const { return _surface; }
			inline const Format& getFormat() const { return _format; }
			inline const ColorSpace& getColorSpace() const { return _colorSpace; }
			inline const uint32_t getWidth() const { return _width; }
			inline const uint32_t getHeight() const { return _height; }
			inline const int32_t getFrameIdx() const { return _currFrameIdx; }
			virtual void acquireFrame() = 0;
			virtual void presentFrame(std::shared_ptr<CommandBuffer> cmdBuff) = 0;
		protected:
			virtual void _initSurface(const AppHandler& handler) = 0;
			Format _format{};
			ColorSpace _colorSpace{};
			std::vector<std::shared_ptr<Texture>> _textures{};
			uint32_t _width{ 0 };
			uint32_t _height{ 0 };
			int32_t _currFrameIdx{ 0 };
			std::shared_ptr<Surface> _surface{ nullptr };
			void _destroyTextures();
		};
	}
}
