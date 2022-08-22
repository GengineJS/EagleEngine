/*
 *  Framebuffer.cpp
 *
 *  Copyright (c) 2022 by XiangKui Zheng
 *  This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */

#include <graphics/Framebuffer.h>
namespace eg {
    namespace graphics {
        Framebuffer::Framebuffer(const FramebufferInfo& info) : _info(info)
        {
        }

        Framebuffer::~Framebuffer()
        {
        }
    }
}
