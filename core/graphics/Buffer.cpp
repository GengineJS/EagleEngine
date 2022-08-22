/*
 *  Buffer.cpp
 *
 *  Copyright (c) 2022 by XiangKui Zheng
 *  This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */

#include <graphics/Buffer.h>
namespace eg {
    namespace graphics {
        Buffer::Buffer(const BufferInfo& info) : _info(info)
        {
        }

        Buffer::~Buffer()
        {
        }
        void Buffer::update(const void* data) { update(data, _info.size, _info.offset); };
        BufferView::BufferView(const BufferViewInfo& info) : _info(info)
        {
        }

        BufferView::~BufferView()
        {
        }
        BufferInfo::BufferInfo(uint64_t size, uint64_t offset, BufferUsageFlag usage,
            MemoryPropertyFlag memProp, DescriptorType desc,
            ShaderStageFlag stage):size(size),offset(offset),usage(usage),memProp(memProp),descType(desc),stage(stage)
        {}
    }
}
