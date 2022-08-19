#include <graphics/vulkan/VKTexture.h>
#include <graphics/vulkan/VKContext.h>
#include <graphics/vulkan/VKDevice.h>
#include <graphics/vulkan/VKTools.h>
#include <graphics/vulkan/VKBuffer.h>
#include <graphics/vulkan/VKCommandBuffer.h>
namespace eg {
	namespace graphics {
		VKTexture::VKTexture(const TextureInfo& info) :Texture(info) {}

		void VKTexture::create()
		{
			auto context = Context::GetContext();
			auto device = std::dynamic_pointer_cast<VKDevice>(context->getDevice());
			VkImageCreateInfo imgInfo{};
			imgInfo.sType = VK_STRUCTURE_TYPE_IMAGE_CREATE_INFO;
			imgInfo.extent = { _info.width, _info.height, _info.depth };
			imgInfo.arrayLayers = _info.layers;
			imgInfo.format = static_cast<VkFormat>(_info.format);
			VkImageType imgType{ static_cast<VkImageType>(_info.type) };
			switch (_info.type) {
			    case TextureType::TEX_1D:
			        imgType = VK_IMAGE_TYPE_1D;
                    break;
			    case TextureType::TEX_3D:
                    imgType = VK_IMAGE_TYPE_3D;
			        break;
			    case TextureType::TEX_2D:
                case TextureType::TEX_CUBE:
                case TextureType::CUBE_ARRAY:
                case TextureType::TEX2D_ARRAY:
                    imgType = VK_IMAGE_TYPE_2D;
                    break;
                case TextureType::TEX1D_ARRAY:
                    imgType = VK_IMAGE_TYPE_1D;
                    break;
			}
			imgInfo.imageType = imgType;
			imgInfo.initialLayout = static_cast<VkImageLayout>(_info.initialLayout);
			imgInfo.mipLevels = _info.levels;
			imgInfo.samples = static_cast<VkSampleCountFlagBits>(_info.samples);
			imgInfo.sharingMode = static_cast<VkSharingMode>(_info.sharing);
			imgInfo.tiling = static_cast<VkImageTiling>(_info.tiling);
			imgInfo.usage = static_cast<VkImageUsageFlags>(_info.usage);
			VK_CHECK_RESULT(vkCreateImage(device->getLogicDevice(), &imgInfo, nullptr, &_image));
			VkMemoryRequirements memRequirements;
			vkGetImageMemoryRequirements(device->getLogicDevice(), _image, &memRequirements);

			VkMemoryAllocateInfo allocInfo{};
			allocInfo.sType = VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO;
			_size = allocInfo.allocationSize = memRequirements.size;
			allocInfo.memoryTypeIndex = device->getMemoryTypeIndex(memRequirements.memoryTypeBits, static_cast<VkMemoryPropertyFlags>(_info.memProp));

			if (vkAllocateMemory(device->getLogicDevice(), &allocInfo, nullptr, &_mem) != VK_SUCCESS) {
				throw std::runtime_error("failed to allocate image memory!");
			}
			vkBindImageMemory(device->getLogicDevice(), _image, _mem, 0);
			create(_image);
		}

		void VKTexture::create(VkImage image)
		{
			if (!_image) _image = image;
			auto context = Context::GetContext();
			auto device = std::dynamic_pointer_cast<VKDevice>(context->getDevice());
			VkImageViewCreateInfo viewInfo{};
			viewInfo.sType = VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO;
			viewInfo.image = image;
			viewInfo.format = static_cast<VkFormat>(_info.format);
			viewInfo.viewType = static_cast<VkImageViewType>(_info.type);
			viewInfo.subresourceRange.aspectMask = static_cast<VkImageAspectFlags>(_info.subresourceRange.aspect);
			viewInfo.subresourceRange.baseArrayLayer = _info.subresourceRange.baseArrayLayer;
			viewInfo.subresourceRange.baseMipLevel = _info.subresourceRange.baseMipLevel;
			viewInfo.subresourceRange.layerCount = _info.subresourceRange.layerCount;
			viewInfo.subresourceRange.levelCount = _info.subresourceRange.levelCount;
			viewInfo.components.r = static_cast<VkComponentSwizzle>(_info.components.r);
			viewInfo.components.g = static_cast<VkComponentSwizzle>(_info.components.g);
			viewInfo.components.b = static_cast<VkComponentSwizzle>(_info.components.b);
			viewInfo.components.a = static_cast<VkComponentSwizzle>(_info.components.a);
			VK_CHECK_RESULT(vkCreateImageView(device->getLogicDevice(), &viewInfo, nullptr, &_imageView));
			createSampler();
		}

		VKTexture::~VKTexture() {
			auto context = Context::GetContext();
			auto device = std::dynamic_pointer_cast<VKDevice>(context->getDevice());
			auto logicDevice = device->getLogicDevice();

			// If the current image is all swapchain, it can not be released
			if(!_isSwapchain) vkDestroyImage(logicDevice, _image, nullptr);
			vkDestroyImageView(logicDevice, _imageView, nullptr);
			if (_mem) vkFreeMemory(logicDevice, _mem, nullptr);
		}
		void VKTexture::loadTexture(const std::string& filePath)
		{
			// We use the Khronos texture format (https://www.khronos.org/opengles/sdk/tools/KTX/file_format_spec/)
			std::string filename = filePath;
			// Texture data contains 4 channels (RGBA) with unnormalized 8-bit values, this is the most commonly supported format
			VkFormat format = VK_FORMAT_R8G8B8A8_UNORM;

			ktxResult result;
			ktxTexture* ktxTexture;
			auto context = Context::GetContext();
#if defined(__ANDROID__)
			auto androidApp = context->getAppHandler().androidApp;
			// Textures are stored inside the apk on Android (compressed)
			// So they need to be loaded via the asset manager
			AAsset* asset = AAssetManager_open(androidApp->activity->assetManager, filename.c_str(), AASSET_MODE_STREAMING);
			if (!asset) {
				exitFatal("Could not load texture from " + filename + "\n\nThe file may be part of the additional asset pack.\n\nRun \"download_assets.py\" in the repository root to download the latest version.", -1);
			}
			size_t size = AAsset_getLength(asset);
			assert(size > 0);

			ktx_uint8_t* textureData = new ktx_uint8_t[size];
			AAsset_read(asset, textureData, size);
			AAsset_close(asset);
			result = ktxTexture_CreateFromMemory(textureData, size, KTX_TEXTURE_CREATE_LOAD_IMAGE_DATA_BIT, &ktxTexture);
			delete[] textureData;
#else
			if (!fileExists(filename)) {
				exitFatal("Could not load texture from " + filename + "\n\nThe file may be part of the additional asset pack.\n\nRun \"download_assets.py\" in the repository root to download the latest version.", -1);
			}
			result = ktxTexture_CreateFromNamedFile(filename.c_str(), KTX_TEXTURE_CREATE_LOAD_IMAGE_DATA_BIT, &ktxTexture);
#endif
			assert(result == KTX_SUCCESS);

			// Get properties required for using and upload texture data from the ktx texture object
			_info.width = ktxTexture->baseWidth;
			_info.height = ktxTexture->baseHeight;
			_info.levels = ktxTexture->numLevels;
			ktx_uint8_t* ktxTextureData = ktxTexture_GetData(ktxTexture);
			ktx_size_t ktxTextureSize = ktxTexture_GetSize(ktxTexture);

			// We prefer using staging to copy the texture data to a device local optimal image
			VkBool32 useStaging = true;
			auto device = std::dynamic_pointer_cast<VKDevice>(context->getDevice());
			// Only use linear tiling if forced
			bool forceLinearTiling = false;
			if (forceLinearTiling) {
				// Don't use linear if format is not supported for (linear) shader sampling
				// Get device properties for the requested texture format
				VkFormatProperties formatProperties;
				vkGetPhysicalDeviceFormatProperties(device->getPhysicalDevice(), format, &formatProperties);
				useStaging = !(formatProperties.linearTilingFeatures & VK_FORMAT_FEATURE_SAMPLED_IMAGE_BIT);
			}
			_info.subresourceRange.levelCount = useStaging ? _info.levels : 1;
			if (useStaging) {
				BufferInfo buffInfo{};
				buffInfo.size = ktxTextureSize;
				buffInfo.usage = BufferUsageFlag::TRANSFER_SRC;
				buffInfo.sharing = SharingMode::EXCLUSIVE;
				buffInfo.memProp = MemoryPropertyFlag::HOST_VISIBLE | MemoryPropertyFlag::HOST_COHERENT;
				auto vkBuffer = std::dynamic_pointer_cast<VKBuffer>(device->createBuffer(buffInfo));
				vkBuffer->update(ktxTextureData, ktxTextureSize, 0);
				// Setup buffer copy regions for each mip level
				std::vector<VkBufferImageCopy> bufferCopyRegions;
				uint32_t offset = 0;

				for (uint32_t i = 0; i < _info.levels; i++) {
					// Calculate offset into staging buffer for the current mip level
					ktx_size_t offset;
					KTX_error_code ret = ktxTexture_GetImageOffset(ktxTexture, i, 0, 0, &offset);
					assert(ret == KTX_SUCCESS);
					// Setup a buffer image copy structure for the current mip level
					VkBufferImageCopy bufferCopyRegion = {};
					bufferCopyRegion.imageSubresource.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
					bufferCopyRegion.imageSubresource.mipLevel = i;
					bufferCopyRegion.imageSubresource.baseArrayLayer = 0;
					bufferCopyRegion.imageSubresource.layerCount = 1;
					bufferCopyRegion.imageExtent.width = ktxTexture->baseWidth >> i;
					bufferCopyRegion.imageExtent.height = ktxTexture->baseHeight >> i;
					bufferCopyRegion.imageExtent.depth = 1;
					bufferCopyRegion.bufferOffset = offset;
					bufferCopyRegions.push_back(bufferCopyRegion);
				}

				_info.type = TextureType::TEX_2D;
				_info.format = Format::R8G8B8A8_UNORM;
				_info.usage = TextureUsageFlag::TRANSFER_DST | TextureUsageFlag::SAMPLED;
				_info.memProp = MemoryPropertyFlag::DEVICE_LOCAL;
				create();

				CommandBufferInfo cmdInfo{};
				cmdInfo.queueIdx = device->getQueueFamilyIndices().graphics;
				std::shared_ptr<VKCommandBuffer> cmdBuff = std::make_shared<VKCommandBuffer>(cmdInfo);
				cmdBuff->beginSingleTimeCommand();

				// Image memory barriers for the texture image

				// The sub resource range describes the regions of the image that will be transitioned using the memory barriers below
				VkImageSubresourceRange subresourceRange = {};
				// Image only contains color data
				subresourceRange.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
				// Start at first mip level
				subresourceRange.baseMipLevel = 0;
				// We will transition on all mip levels
				subresourceRange.levelCount = _info.levels;
				// The 2D texture only has one layer
				subresourceRange.layerCount = 1;

				// Transition the texture image layout to transfer target, so we can safely copy our buffer data to it.
				VkImageMemoryBarrier imageMemoryBarrier = initializers::imageMemoryBarrier();;
				imageMemoryBarrier.image = _image;
				imageMemoryBarrier.subresourceRange = subresourceRange;
				imageMemoryBarrier.srcAccessMask = 0;
				imageMemoryBarrier.dstAccessMask = VK_ACCESS_TRANSFER_WRITE_BIT;
				imageMemoryBarrier.oldLayout = VK_IMAGE_LAYOUT_UNDEFINED;
				imageMemoryBarrier.newLayout = VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL;

				// Insert a memory dependency at the proper pipeline stages that will execute the image layout transition
				// Source pipeline stage is host write/read execution (VK_PIPELINE_STAGE_HOST_BIT)
				// Destination pipeline stage is copy command execution (VK_PIPELINE_STAGE_TRANSFER_BIT)
				vkCmdPipelineBarrier(
					cmdBuff->getVkCmdBuffer(),
					VK_PIPELINE_STAGE_HOST_BIT,
					VK_PIPELINE_STAGE_TRANSFER_BIT,
					0,
					0, nullptr,
					0, nullptr,
					1, &imageMemoryBarrier);

				// Copy mip levels from staging buffer
				vkCmdCopyBufferToImage(
					cmdBuff->getVkCmdBuffer(),
					vkBuffer->getVkBuffer(),
					_image,
					VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL,
					static_cast<uint32_t>(bufferCopyRegions.size()),
					bufferCopyRegions.data());

				// Once the data has been uploaded we transfer to the texture image to the shader read layout, so it can be sampled from
				imageMemoryBarrier.srcAccessMask = VK_ACCESS_TRANSFER_WRITE_BIT;
				imageMemoryBarrier.dstAccessMask = VK_ACCESS_SHADER_READ_BIT;
				imageMemoryBarrier.oldLayout = VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL;
				imageMemoryBarrier.newLayout = VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL;

				// Insert a memory dependency at the proper pipeline stages that will execute the image layout transition
				// Source pipeline stage is copy command execution (VK_PIPELINE_STAGE_TRANSFER_BIT)
				// Destination pipeline stage fragment shader access (VK_PIPELINE_STAGE_FRAGMENT_SHADER_BIT)
				vkCmdPipelineBarrier(
					cmdBuff->getVkCmdBuffer(),
					VK_PIPELINE_STAGE_TRANSFER_BIT,
					VK_PIPELINE_STAGE_FRAGMENT_SHADER_BIT,
					0,
					0, nullptr,
					0, nullptr,
					1, &imageMemoryBarrier);

				// Store current layout for later reuse
				// _info.initialLayout = TextureLayout::SHADER_READ_ONLY_OPTIMAL;
				cmdBuff->endSingleTimeCommand();
				// Clean up staging resources
			} else {
				// Copy data to a linear tiled image
				_info.format = Format::R8G8B8A8_UNORM;
				_info.tiling = TextureTiling::LINEAR;
				_info.usage = TextureUsageFlag::SAMPLED;
				_info.sharing = SharingMode::EXCLUSIVE;
				_info.initialLayout = TextureLayout::PREINITIALIZED;
				_info.levels = 1;
				_info.memProp = MemoryPropertyFlag::HOST_VISIBLE | MemoryPropertyFlag::HOST_COHERENT;
				create();

				// Map image memory
				void* data;
				VK_CHECK_RESULT(vkMapMemory(device->getLogicDevice(), _mem, 0, _size, 0, &data));
				// Copy image data of the first mip level into memory
				memcpy(data, ktxTextureData, _size);
				vkUnmapMemory(device->getLogicDevice(), _mem);

				// Linear tiled images don't need to be staged and can be directly used as textures
				// The sub resource range describes the regions of the image we will be transition
				VkImageSubresourceRange subresourceRange = {};
				subresourceRange.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
				subresourceRange.baseMipLevel = 0;
				subresourceRange.levelCount = 1;
				subresourceRange.layerCount = 1;

				// Transition the texture image layout to shader read, so it can be sampled from
				VkImageMemoryBarrier imageMemoryBarrier = initializers::imageMemoryBarrier();;
				imageMemoryBarrier.image = _image;
				imageMemoryBarrier.subresourceRange = subresourceRange;
				imageMemoryBarrier.srcAccessMask = VK_ACCESS_HOST_WRITE_BIT;
				imageMemoryBarrier.dstAccessMask = VK_ACCESS_SHADER_READ_BIT;
				imageMemoryBarrier.oldLayout = VK_IMAGE_LAYOUT_PREINITIALIZED;
				imageMemoryBarrier.newLayout = VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL;

				CommandBufferInfo cmdInfo{};
				cmdInfo.queueIdx = device->getQueueFamilyIndices().graphics;
				std::shared_ptr<VKCommandBuffer> cmdBuff = std::make_shared<VKCommandBuffer>(cmdInfo);
				cmdBuff->beginSingleTimeCommand();

				// Insert a memory dependency at the proper pipeline stages that will execute the image layout transition
				// Source pipeline stage is host write/read execution (VK_PIPELINE_STAGE_HOST_BIT)
				// Destination pipeline stage fragment shader access (VK_PIPELINE_STAGE_FRAGMENT_SHADER_BIT)
				vkCmdPipelineBarrier(
					cmdBuff->getVkCmdBuffer(),
					VK_PIPELINE_STAGE_HOST_BIT,
					VK_PIPELINE_STAGE_FRAGMENT_SHADER_BIT,
					0,
					0, nullptr,
					0, nullptr,
					1, &imageMemoryBarrier);

				cmdBuff->endSingleTimeCommand();
			}
			ktxTexture_Destroy(ktxTexture);
		}
	}
}
