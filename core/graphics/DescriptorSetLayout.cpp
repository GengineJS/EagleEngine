/*
 *  DescriptorSetLayout.cpp
 *
 *  Copyright (c) 2022 by XiangKui Zheng
 *  This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */

#include <graphics/DescriptorSetLayout.h>

namespace eg {
	namespace graphics {
		DescriptorSetLayout::DescriptorSetLayout(const DescriptorSetLayoutInfo& info):_info(info)
		{
		}

		DescriptorSetLayout::~DescriptorSetLayout()
		{
		}
	}
}
