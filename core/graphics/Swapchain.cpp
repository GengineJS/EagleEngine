/*
 *  Swapchain.cpp
 *
 *  Copyright (c) 2022 by XiangKui Zheng
 *  This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */

#include <graphics/Swapchain.h>
namespace eg {
	namespace graphics {
		Swapchain::Swapchain()
		{
		}

		void Swapchain::destroy() {
			_surface = nullptr;
			_destroyTextures();
		}

		Swapchain::~Swapchain()
		{
			destroy();
		}

		void Swapchain::_destroyTextures()
		{
			_textures.clear();
		}
	}
}
