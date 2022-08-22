/*
 *  VKTexture.h
 *
 *  Copyright (c) 2022 by XiangKui Zheng
 *  This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */

#pragma once
#include <graphics/Texture.h>
#include <vulkan/vulkan.h>
#include <ktx.h>
#include <ktxvulkan.h>
namespace eg {
	namespace graphics {
		class  VKTexture : public Texture
		{
			friend class VKSwapchain;
		public:
			VKTexture() = default;
			VKTexture(const TextureInfo& info);
			void create() override;
			void create(VkImage image);
			inline const VkImage& getVkImage() { return _image; }
			inline const VkImageView& getVkImageView() { return _imageView; }
			inline const VkDeviceMemory& getMemory() { return _mem; }
			~VKTexture() override;
			void loadTexture(const std::string& filePath) override;
		protected:
			VkImage _image{ VK_NULL_HANDLE };
			VkImageView _imageView{ VK_NULL_HANDLE };
			VkDeviceMemory _mem{ VK_NULL_HANDLE };
			bool _isSwapchain{ false };
		};
	}
}
