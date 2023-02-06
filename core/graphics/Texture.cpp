/*
 *  Texture.cpp
 *
 *  Copyright (c) 2022 by XiangKui Zheng
 *  This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */

#include <graphics/Texture.h>
#include <graphics/Context.h>
namespace eg {
    namespace graphics {
        Texture::Texture(const TextureInfo& info) : _info(info) {}
        void Texture::create() {}
        Texture::~Texture() {
        }
        const std::unique_ptr<Sampler>& Texture::createSampler(const SamplerInfo& info) {
            _sampler = Context::GetContext()->getDevice()->createSampler(info);
            return _sampler;
        }
    }
}
