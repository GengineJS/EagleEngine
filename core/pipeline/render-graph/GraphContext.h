/*
 *  GraphContext.h
 *
 *  Copyright (c) 2022 by XiangKui Zheng
 *  This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */

#pragma once

#include <unordered_map>
#include <graphics/Texture.h>
#include <graphics/Buffer.h>
#include <pipeline/render-graph/RenderGraph.h>
#include <graphics/RenderPass.h>
#include <graphics/Context.h>
#include <map>
#include <pipeline/render-graph/RenderVisitor.h>
namespace eg {
	namespace pipeline {
		class GraphContext;
		class DeviceResource
		{
		public:
			DeviceResource(const RenderResource* res, const GraphContext* context);
			virtual ~DeviceResource();

			void setResource(const RenderResource* res);
			const RenderResource* getReource() const { return _resorce; }
		protected:
			RenderResource* _resorce{ nullptr };
			GraphContext* _context{ nullptr };
		};

		class DeviceTexture : public DeviceResource
		{
		public:
			DeviceTexture(const RenderTexture* res, const GraphContext* context);
			~DeviceTexture() override;
			graphics::Texture* getTexture() const {
				return _tex.get();
			}
		protected:
			std::unique_ptr<graphics::Texture> _tex{ nullptr };
		};

		class DeviceBuffer : public DeviceResource
		{
		public:
			DeviceBuffer(const RenderBuffer* res, const GraphContext* context);
			~DeviceBuffer() override;
			graphics::Buffer* getBuffer() const {
				return _buff;
			}
		protected:
			graphics::Buffer* _buff{ nullptr };
		};

		class DeviceGraphPass {
		public:
			DeviceGraphPass(const BaseGraphPass* graphPass, const GraphContext* context);
			virtual ~DeviceGraphPass();
			inline BaseGraphPass* getGraphPass() const { return _graphPass; }
			inline const std::unique_ptr<RenderPass>& getRenderPass() const { return _renderPass; }
			inline const auto& getFramebuffers() const { return _framebuffers; }
			void execute(uint32_t cmdBuffId);
		protected:
			const std::unique_ptr<RenderVisitor> _renderVisitor{ std::make_unique<RenderVisitor>() };
			BaseGraphPass* _graphPass{ nullptr };
			GraphContext* _context{ nullptr };
			std::unique_ptr<RenderPass> _renderPass{ nullptr };
			
			// Postprocess
			std::unique_ptr<ShaderStage> _shader{ nullptr };
			// set and ds
			std::unordered_map<uint32_t, std::unique_ptr<DescriptorSet>> _descSets{};
			std::unique_ptr<PipelineLayout> _pipelineLayout{ nullptr };
			std::unique_ptr<VertexInput> _vertInput{ nullptr };
			std::unique_ptr<GraphicsPipeline> _pipeline{ nullptr };

			void _createScreenQuad();
			void _createDescriptor();
			void _createPipeline();

			std::vector<DeviceTexture*> _inputTexs;
			// only screenpass will have multiple framebuffers
			std::vector<std::unique_ptr<graphics::Framebuffer>> _framebuffers{};
			// Provides default if rendergraph is not configured with any depth texture
			std::vector<std::unique_ptr<Texture>> _tempDepTexs{};
			// Ditto
			std::unique_ptr<Texture> _tempColTex{ nullptr };
			std::unique_ptr<RenderPass> _createRenderPass(const PassView&, const std::vector<graphics::Format>& colors, graphics::Format dStencil);
		};

		class DeviceRenderPhase {

		};

		class GraphContext
		{
		public:
			GraphContext(const std::unique_ptr<RenderGraph>& renderGraph);
			virtual ~GraphContext();

			void setRenderGraph(const std::unique_ptr<RenderGraph>& renderGraph);
			const std::unordered_map<std::string, std::unique_ptr<DeviceResource>>& getDeviceResources() const { return _resources; }
			const std::vector<std::unique_ptr<DeviceGraphPass>>& getRenderPasses() const { return _passes; }
			graphics::Context* getGfxContext() const { return _gfxContext; }
			RenderGraph* getRenderGraph() const;
			void addPass(std::unique_ptr<DeviceGraphPass> &&pass);
			void addResource(std::unique_ptr<DeviceResource> &&res);
			void clearPasses();
			void clearResources();
		private:
			RenderGraph* _renderGraph{ nullptr };
			graphics::Context* _gfxContext{nullptr};
			std::unordered_map<std::string, std::unique_ptr<DeviceResource>> _resources{};
			std::vector<std::unique_ptr<DeviceGraphPass>> _passes{};
		};
	}
}
