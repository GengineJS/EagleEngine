/*
 *  Surface.cpp
 *
 *  Copyright (c) 2022 by XiangKui Zheng
 *  This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */

#include <graphics/Surface.h>
#include <type_traits>
namespace eg {
    namespace graphics {
        Surface::Surface(const SurfaceInfo& info) :_info(info) {}

        Surface::~Surface()
        {
        }
    }
}
