/*
 *  Surface.h
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
namespace eg {
	namespace graphics {
		struct SurfaceInfo
		{
#if defined(USE_PLATFORM_WIN32)
			HINSTANCE hinstance;
			HWND hwnd;
#elif defined(USE_PLATFORM_ANDROID_KHR)
			ANativeWindow* window;
#elif (defined(USE_PLATFORM_IOS_MVK) || defined(USE_PLATFORM_MACOS_MVK))
			const void* pView;
#endif
		};


		class Surface
		{
		public:
			Surface() = default;
			Surface(const SurfaceInfo& info);
			virtual ~Surface();
			inline const SurfaceInfo& getSurfaceInfo() const { return _info; }
		protected:
			SurfaceInfo _info;
		};
	}
}
