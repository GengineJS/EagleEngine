/*
 *  DescriptorPool.h
 *
 *  Copyright (c) 2022 by XiangKui Zheng
 *  This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */

#pragma once
#include <graphics/GraphicsDef.h>
#include <vector>
namespace eg {
	namespace graphics {
		struct DescriptorPoolSize {
			DescriptorType type;
			uint32_t descCount{ 1 };
		};

		struct DescriptorPoolInfo
		{
			std::vector<DescriptorPoolSize> pPoolSizes;
			uint32_t maxSets{1};
			DescriptorPoolFlag flag{DescriptorPoolFlag::NONE};
		};

		class DescriptorPool {
		public:
			DescriptorPool() = default;
			DescriptorPool(const DescriptorPoolInfo& info);
			virtual ~DescriptorPool();
			virtual void destroy() = 0;
			inline const DescriptorPoolInfo& getDescriptorPoolInfo() const { return _info; }
		protected:
			DescriptorPoolInfo _info{};
		};
	}
}