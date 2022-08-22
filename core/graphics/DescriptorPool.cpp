/*
 *  DescriptorPool.cpp
 *
 *  Copyright (c) 2022 by XiangKui Zheng
 *  This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */

#include "DescriptorPool.h"

namespace eg {
	namespace graphics {
		DescriptorPool::DescriptorPool(const DescriptorPoolInfo& info): _info(info)
		{
		}

		DescriptorPool::~DescriptorPool()
		{
		}
	}
}
