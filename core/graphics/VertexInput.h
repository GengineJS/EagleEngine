/*
 *  VertexInput.h
 *
 *  Copyright (c) 2022 by XiangKui Zheng
 *  This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */

#pragma once
#include <vector>
#include <graphics/GraphicsDef.h>
#include <graphics/Buffer.h>
#include <glm/vec2.hpp>
#include <glm/vec3.hpp>
#include <glm/vec4.hpp>

namespace eg {
	namespace graphics {
        struct VertexInputAttributeInfo {
            uint32_t    location;
            uint32_t    binding;
            Format      format;
            uint32_t    offset;
        };

        struct VertexInputBindingInfo {
            uint32_t             binding;
            uint32_t             stride;
            VertexInputRate    inputRate;
        };

        struct InputAssemblerInfo
        {
            PrimitiveTopology topology{ PrimitiveTopology::TRIANGLE_LIST };
            bool isPrimitiveRestart{ false };
        };
        /*
            glTF default vertex layout
        */
        enum class VertexComponent { Position, Normal, UV, Color, Tangent, Joint0, Weight0 };
        struct Vertex {
            glm::vec3 pos;
            glm::vec3 normal;
            glm::vec2 uv;
            glm::vec4 color;
            glm::vec4 tangent;
            glm::vec4 joint0;
            glm::vec4 weight0;
        };

        struct VertexInputInfo
        {
            InputAssemblerInfo iaInfo{};
            std::vector<Vertex> vertexBuffer{};
            std::vector<uint32_t> indexBuffer{};
        };
        struct VertexInputStateInfo
        {
            std::vector<VertexInputBindingInfo> bindings{};
            std::vector<VertexInputAttributeInfo> attributes{};
        };

        
        class InputAssembler
        {
        public:
            InputAssembler() = default;
            InputAssembler(const InputAssemblerInfo&);
            virtual ~InputAssembler();
            virtual void destroy() = 0;
            inline const InputAssemblerInfo& getInputAssemblerInfo() const { return _info; }
        protected:
            InputAssemblerInfo _info{};
        };

		class VertexInput
		{
		public:
			VertexInput() = default;
            VertexInput(const VertexInputInfo& info);
			virtual ~VertexInput();
            virtual void destroy() = 0;
            void createInputAssembler();
            inline const VertexInputInfo& getVertexInputInfo() const { return _info; }
            inline std::shared_ptr<InputAssembler> getInputAssembler() const { return _assembler; }
            inline const std::vector<Vertex>& getVertexData() const { return _vertexData; }
            inline std::shared_ptr<Buffer> getVertexBuffer() const { return _vertexBuffer; }
            inline std::shared_ptr<Buffer> getIndexBuffer() const { return _indexBuffer; }
            inline const std::vector<uint32_t>& getIndexData() const { return _indexData; }
		protected:
            VertexInputBindingInfo _vertexInputBindingDescription;
            std::vector<VertexInputAttributeInfo> _vertexInputAttributeDescriptions;
            VertexInputBindingInfo _inputBindingDescription(uint32_t binding);
            VertexInputAttributeInfo _inputAttributeDescription(uint32_t binding, uint32_t location, VertexComponent component);
            std::vector<VertexInputAttributeInfo> _inputAttributeDescriptions(uint32_t binding, const std::vector<VertexComponent> components);
            /** @brief Returns the default pipeline vertex input state create info structure for the requested vertex components */
            void _createPipelineVertexInputState(const std::vector<VertexComponent> components);
            std::shared_ptr<InputAssembler> _assembler{ nullptr };
            VertexInputInfo _info{};
            VertexInputStateInfo _stateInfo{};
            std::vector<Vertex> _vertexData{};
            std::shared_ptr<Buffer> _vertexBuffer{ nullptr };
            std::vector<uint32_t> _indexData{};
            std::shared_ptr<Buffer> _indexBuffer{ nullptr };
		};
	}
}
