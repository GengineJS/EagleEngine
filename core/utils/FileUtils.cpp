#include <utils/FileUtils.h>
#include <iostream>
#include <graphics/Context.h>
#if (defined(USE_PLATFORM_MACOS_MVK) || defined(USE_PLATFORM_IOS_MVK)) && !defined(EXAMPLE_XCODE_GENERATED)
    #include <CoreFoundation/CFBundle.h>
#endif
namespace eg {
	namespace utils {
		FileUtils::FileUtils()
		{
		}
		FileUtils::~FileUtils()
		{
		}
		graphics::ShaderStageInfo FileUtils::loadSPIRVShader(const std::string& filename, graphics::ShaderStageFlag stage)
		{
			size_t shaderSize;
			char* shaderCode{nullptr};
			auto context = graphics::Context::GetContext();
#if defined(__ANDROID__)
			auto androidApp = context->getAppHandler().androidApp;
			// Load shader from compressed asset
			AAsset* asset = AAssetManager_open(androidApp->activity->assetManager, filename.c_str(), AASSET_MODE_STREAMING);
			assert(asset);
			shaderSize = AAsset_getLength(asset);
			assert(shaderSize > 0);

			shaderCode = new char[shaderSize];
			AAsset_read(asset, shaderCode, shaderSize);
			AAsset_close(asset);
#else
			std::ifstream is(filename, std::ios::binary | std::ios::in | std::ios::ate);

			if (is.is_open())
			{
				shaderSize = is.tellg();
				is.seekg(0, std::ios::beg);
				// Copy file contents into a buffer
				shaderCode = new char[shaderSize];
				is.read(shaderCode, shaderSize);
				is.close();
				assert(shaderSize > 0);
			}
#endif
			if (shaderCode)
			{
				graphics::ShaderStageInfo shaderInfo{};
				shaderInfo.pCode = reinterpret_cast<uint32_t*>(shaderCode);
				shaderInfo.size = static_cast<uint32_t>(shaderSize);
				shaderInfo.stage = stage;
				return shaderInfo;
			}
			else
			{
				std::cerr << "Error: Could not open shader file \"" << filename << "\"" << std::endl;
				return {};
			}
		}
		const std::string FileUtils::getAssetPath()
		{
#if defined(__ANDROID__)
			return "";
#elif defined(EXAMPLE_DATA_DIR)
			return EXAMPLE_DATA_DIR;
#elif (defined(USE_PLATFORM_MACOS_MVK) || defined(USE_PLATFORM_IOS_MVK)) && !defined(EXAMPLE_XCODE_GENERATED)
            // Get a reference to the main bundle
            CFBundleRef mainBundle = CFBundleGetMainBundle();
            // Get a reference to the file's URL
            CFURLRef dataURL = CFBundleCopyResourceURL(mainBundle, CFSTR("data"), NULL, NULL);
            // Convert the URL reference into a string reference
            CFStringRef dataPath = CFURLCopyFileSystemPath(dataURL, kCFURLPOSIXPathStyle);

            // Get the system encoding method
            CFStringEncoding encodingMethod = CFStringGetSystemEncoding();
            // Convert the string reference into a C string
            return std::string(CFStringGetCStringPtr(dataPath, encodingMethod)) + "/";
#else
			return "./../data/";
#endif
		}
		const std::string FileUtils::getShadersPath(const std::string& shaderDir)
		{
			return getAssetPath() + "shaders/" + shaderDir + "/";
		}
	}
}
