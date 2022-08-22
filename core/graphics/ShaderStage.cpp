/*
 *  ShaderStage.cpp
 *
 *  Copyright (c) 2022 by XiangKui Zheng
 *  This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */

#include <graphics/ShaderStage.h>
namespace eg {
	namespace graphics {
		ShaderStage::ShaderStage(const std::vector<ShaderStageInfo>& infos)
		{
			for (auto curInfo : infos) {
				switch (curInfo.stage)
				{
				case ShaderStageFlag::VERTEX:
					_vertexInfo = curInfo;
					break;
				case ShaderStageFlag::GEOMETRY:
					_geometryInfo = curInfo;
					break;
				case ShaderStageFlag::TESSELLATION_CONTROL:
					_tessellateControlInfo = curInfo;
					break;
				case ShaderStageFlag::TESSELLATION_EVALUATION:
					_tessellateEvaInfo = curInfo;
					break;
				case ShaderStageFlag::FRAGMENT:
					_fragmentInfo = curInfo;
					break;
				case ShaderStageFlag::COMPUTE:
					_computeInfo = curInfo;
					break;
				default:
					break;
				}
			}
		}

		ShaderStage::~ShaderStage()
		{
		}
	}
}
