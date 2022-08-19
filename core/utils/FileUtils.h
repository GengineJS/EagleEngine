#pragma once
#include <string>
#include <fstream>
#include "graphics/GraphicsDef.h"
#include <assert.h>
namespace eg {
	namespace utils {
		class FileUtils
		{
		public:
			FileUtils();
			virtual ~FileUtils();
			static graphics::ShaderStageInfo loadSPIRVShader(const std::string& filename, graphics::ShaderStageFlag stage);
			static const std::string getAssetPath();
			static const std::string getShadersPath(const std::string& shaderDir = "glsl");
		};
	}
}
