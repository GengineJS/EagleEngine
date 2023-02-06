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
#include <modules/EagleWindow.hpp>
namespace eg {
	namespace graphics {
		class  Swapchain
		{
		public:
			Swapchain();
			virtual ~Swapchain();
			virtual void create(const WindowHandler& handler) = 0;
			virtual void destroy();
			inline const std::vector<std::unique_ptr<Texture>>& getTextures() const { return textures; }
			inline const std::unique_ptr<Surface>& getSurface() const { return _surface; }
			inline const Format& getFormat() const { return _format; }
			inline const ColorSpace& getColorSpace() const { return _colorSpace; }
			inline const uint32_t getWidth() const { return _width; }
			inline const uint32_t getHeight() const { return _height; }
			inline const int32_t getFrameIdx() const { return _currFrameIdx; }
			virtual void acquireFrame() = 0;
			virtual void presentFrame(const std::unique_ptr<CommandBuffer>& cmdBuff) = 0;
		protected:
			virtual void _initSurface(const WindowHandler& handler) = 0;
			Format _format{};
			ColorSpace _colorSpace{};
			std::vector<std::unique_ptr<Texture>> textures{};
			uint32_t _width{ 0 };
			uint32_t _height{ 0 };
			int32_t _currFrameIdx{ 0 };
			std::unique_ptr<Surface> _surface{ nullptr };
			void _destroyTextures();
		};
	}
}
