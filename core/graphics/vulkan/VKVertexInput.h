#pragma once
#include <vulkan/vulkan.h>
#include <graphics/VertexInput.h>
namespace eg {
	namespace graphics {
		class VKInputAssembler : public InputAssembler
		{
		public:
			VKInputAssembler() = default;
			VKInputAssembler(const InputAssemblerInfo& info);
			~VKInputAssembler() override;
			void destroy() override;
			inline const VkPipelineInputAssemblyStateCreateInfo& getVkInputAssemblyState() const { return _assembler; }
		protected:
			VkPipelineInputAssemblyStateCreateInfo _assembler{};
		};

		class VKVertexInput: public VertexInput
		{
		public:
			VKVertexInput() = default;
			VKVertexInput(const VertexInputInfo& info);
			~VKVertexInput() override;
			void destroy() override;
			inline const VkPipelineVertexInputStateCreateInfo& getVkVertexInputStateInfo() const { return _vertState; }
		protected:
			std::vector<VkVertexInputBindingDescription> vkBindings{};
			std::vector<VkVertexInputAttributeDescription> vkAttrs{};
			VkPipelineVertexInputStateCreateInfo _vertState{};
		};
	}
}
