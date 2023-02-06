/*
 *  VKContext.h
 *
 *  Copyright (c) 2022 by XiangKui Zheng
 *  This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */

#pragma once
#include <vulkan/vulkan.h>
#include <graphics/Context.h>

namespace eg {
	namespace graphics {
		class VKContext : public Context {
			friend class VKDevice;
		public:
			VKContext();
			VKContext(VKContext const&) = delete;
			VKContext(VKContext&&) = delete;
			VKContext& operator=(VKContext const&) = delete;
			VKContext& operator=(VKContext&&) = delete;
			~VKContext() override;

			const std::unique_ptr<Device>& createDevice() override;
			void resize(uint32_t w, uint32_t h) override;
			void initialize() override;
			inline const VkInstance& getVkInstance() const { return _instance; }
		protected:
			VkInstance _instance{VK_NULL_HANDLE};
		};
	}
}
