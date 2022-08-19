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
