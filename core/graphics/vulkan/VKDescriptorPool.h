#pragma once
#include <graphics/DescriptorPool.h>
#include <vulkan/vulkan.h>
namespace eg {
	namespace graphics {
		class VKDescriptorPool: public DescriptorPool
		{
		public:
			VKDescriptorPool() = default;
			VKDescriptorPool(const DescriptorPoolInfo& info);
			~VKDescriptorPool() override;
			void destroy() override;
			inline const VkDescriptorPool& getVkDescriptorPool() const { return _pool; }
		protected:
			VkDescriptorPool _pool;
		};
	}
}
