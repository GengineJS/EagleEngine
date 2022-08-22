/*
 *  Sampler.cpp
 *
 *  Copyright (c) 2022 by XiangKui Zheng
 *  This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */

#include <graphics/Sampler.h>
#include <graphics/Context.h>
namespace eg {
    namespace graphics {
        Sampler::Sampler(const SamplerInfo& info) : _info(info) {}
        Sampler::~Sampler()
        {
        }
    }
}
