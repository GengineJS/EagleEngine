/*
 *  VertexInput.cpp
 *
 *  Copyright (c) 2022 by XiangKui Zheng
 *  This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */

#include <graphics/VertexInput.h>
#include <assert.h>
#include <graphics/Context.h>
#include <utils/Utils.h>
namespace eg {
	namespace graphics {
		InputAssembler::InputAssembler(const InputAssemblerInfo& info) : _info(info)
		{
		}
		InputAssembler::~InputAssembler()
		{
		}
		VertexInput::~VertexInput()
		{
		}

		void VertexInput::destroy()
		{
		}
		void VertexInput::createInputAssembler()
		{
			_assembler = Context::GetContext()->getDevice()->createInputAssembler(_info.iaInfo);
		}
		VertexInput::VertexInput(const VertexInputInfo& info) :_info(info) {
			assert(info.vertexBuffer.size() > 0);
			createInputAssembler();
			_vertexData = info.vertexBuffer;
			_indexData = info.indexBuffer;
			_createPipelineVertexInputState({
				VertexComponent::Position,
				VertexComponent::Normal,
				VertexComponent::UV,
				VertexComponent::Color,
				VertexComponent::Tangent,
				VertexComponent::Joint0,
				VertexComponent::Weight0
				});
			auto context = Context::GetContext();
			auto& device = context->getDevice();
			size_t indexBufferSize = _indexData.size() * sizeof(uint32_t);
			size_t vertexBufferSize = _vertexData.size() * sizeof(Vertex);
			if (_indexData.size() > 0) {
				BufferInfo idxBuffInfo{};
				idxBuffInfo.memProp = MemoryPropertyFlag::HOST_VISIBLE | MemoryPropertyFlag::HOST_COHERENT;
				idxBuffInfo.usage = BufferUsageFlag::TRANSFER_SRC;
				idxBuffInfo.size = indexBufferSize;
				auto idxStagingBuffer = device->createBuffer(idxBuffInfo);
				idxStagingBuffer->update(_indexData.data());
				BufferInfo idxDstBuffInfo{};
				idxDstBuffInfo.memProp = MemoryPropertyFlag::DEVICE_LOCAL;
				idxDstBuffInfo.usage = BufferUsageFlag::INDEX | BufferUsageFlag::TRANSFER_DST;
				idxDstBuffInfo.size = indexBufferSize;
				_indexBuffer = device->createBuffer(idxDstBuffInfo);
				_indexBuffer->copyBuffer(idxStagingBuffer.get());
			}
			BufferInfo vertBuffInfo{};
			vertBuffInfo.memProp = MemoryPropertyFlag::HOST_VISIBLE | MemoryPropertyFlag::HOST_COHERENT;
			vertBuffInfo.usage = BufferUsageFlag::TRANSFER_SRC;
			vertBuffInfo.size = vertexBufferSize;
			auto vertStagingBuffer = device->createBuffer(vertBuffInfo);
			vertStagingBuffer->update(_vertexData.data());
			BufferInfo vertDstBuffInfo{};
			vertDstBuffInfo.memProp = MemoryPropertyFlag::DEVICE_LOCAL;
			vertDstBuffInfo.usage = BufferUsageFlag::VERTEX | BufferUsageFlag::TRANSFER_DST;
			vertDstBuffInfo.size = vertexBufferSize;
			_vertexBuffer = device->createBuffer(vertDstBuffInfo);
			_vertexBuffer->copyBuffer(vertStagingBuffer.get());
		}

		VertexInputBindingInfo VertexInput::_inputBindingDescription(uint32_t binding) {
			return VertexInputBindingInfo({ binding, sizeof(Vertex), VertexInputRate::VERTEX });
		}

		VertexInputAttributeInfo VertexInput::_inputAttributeDescription(uint32_t binding, uint32_t location, VertexComponent component) {
			switch (component) {
			case VertexComponent::Position:
				return VertexInputAttributeInfo({ location, binding, Format::R32G32B32_SFLOAT, offsetof(Vertex, pos) });
			case VertexComponent::Normal:
				return VertexInputAttributeInfo({ location, binding, Format::R32G32B32_SFLOAT, offsetof(Vertex, normal) });
			case VertexComponent::UV:
				return VertexInputAttributeInfo({ location, binding, Format::R32G32_SFLOAT, offsetof(Vertex, uv) });
			case VertexComponent::Color:
				return VertexInputAttributeInfo({ location, binding, Format::R32G32B32A32_SFLOAT, offsetof(Vertex, color) });
			case VertexComponent::Tangent:
				return VertexInputAttributeInfo({ location, binding, Format::R32G32B32A32_SFLOAT, offsetof(Vertex, tangent) });
			case VertexComponent::Joint0:
				return VertexInputAttributeInfo({ location, binding, Format::R32G32B32A32_SFLOAT, offsetof(Vertex, joint0) });
			case VertexComponent::Weight0:
				return VertexInputAttributeInfo({ location, binding, Format::R32G32B32A32_SFLOAT, offsetof(Vertex, weight0) });
			default:
				return VertexInputAttributeInfo({});
			}
		}

		std::vector<VertexInputAttributeInfo> VertexInput::_inputAttributeDescriptions(uint32_t binding, const std::vector<VertexComponent> components) {
			std::vector<VertexInputAttributeInfo> result;
			uint32_t location = 0;
			for (VertexComponent component : components) {
				result.emplace_back(VertexInput::_inputAttributeDescription(binding, location, component));
				location++;
			}
			return result;
		}

		void VertexInput::_createPipelineVertexInputState(const std::vector<VertexComponent> components) {
			_vertexInputBindingDescription = _inputBindingDescription(0);
			_vertexInputAttributeDescriptions = _inputAttributeDescriptions(0, components);
			_stateInfo.bindings = { _vertexInputBindingDescription };
			_stateInfo.attributes = _vertexInputAttributeDescriptions;
		}
	}
}
