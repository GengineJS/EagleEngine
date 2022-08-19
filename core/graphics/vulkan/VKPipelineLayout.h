#pragma once
#include <graphics/PipelineLayout.h>
#include <vulkan/vulkan.h>
namespace eg {
	namespace graphics {
		class VKPipelineLayout : public PipelineLayout
		{
		public:
			VKPipelineLayout() = default;
			VKPipelineLayout(const PipelineLayoutInfo& info);
			~VKPipelineLayout() override;
			void destroy() override;
			const VkPipelineLayout& getVkPipelineLayout() const { return _layout; }
		protected:
			VkPipelineLayout _layout;
		};
	}
}
