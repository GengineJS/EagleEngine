/*
 *  Sampler.h
 *
 *  Copyright (c) 2022 by XiangKui Zheng
 *  This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */

#pragma once
#include "graphics/GraphicsDef.h"
namespace eg {
	namespace graphics {
		struct SamplerInfo {
			Filter magFilter{ Filter::LINEAR };
			Filter minFilter{ Filter::LINEAR };
			SamplerMipmapMode mipmapMode{ SamplerMipmapMode::LINEAR };
			SamplerCreateFlag flag{};
			SamplerAddressMode addressModeU{ SamplerAddressMode::REPEAT };
			SamplerAddressMode addressModeV{ SamplerAddressMode::REPEAT };
			SamplerAddressMode addressModeW{ SamplerAddressMode::REPEAT };
			bool                compareEnable{ false };
			CompareOp             compareOp{ CompareOp::NEVER };
			float                   minLod{ 0.0f };
			float                   maxLod{ 0.0f };
			float					mipLodBias{ 0.0f };
			BorderColor           borderColor{ BorderColor::FLOAT_OPAQUE_BLACK };
			bool                unnormalizedCoordinates{ false };
		};

		using SampleMask = uint32_t;

		class Sampler {
		public:
			Sampler() = default;
			Sampler(const SamplerInfo& info);
			virtual ~Sampler();
			inline const SamplerInfo& getSamplerInfo() const { return _info; }
			inline bool getAnisotropyEnable() const { return _anisotropyEnable; }
		protected:
			SamplerInfo _info{};
			bool _anisotropyEnable{ false };
		};
	}
}