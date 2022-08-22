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
			DeviceResource(std::shared_ptr<RenderResource> res, std::shared_ptr<GraphContext> context);
			virtual ~DeviceResource();

			void setResource(std::shared_ptr<RenderResource> res);
			const std::shared_ptr<RenderResource> getReource() const { return _resorce; }
		protected:
			std::shared_ptr<RenderResource> _resorce{ nullptr };
			std::shared_ptr<GraphContext> _context{ nullptr };
		};

		class DeviceTexture : public DeviceResource
		{
		public:
			DeviceTexture(std::shared_ptr<RenderTexture> res, std::shared_ptr<GraphContext> context);
			~DeviceTexture() override;
			const std::shared_ptr<graphics::Texture> getTexture() const {
				return _tex;
			}
		protected:
			std::shared_ptr<graphics::Texture> _tex{ nullptr };
		};

		class DeviceBuffer : public DeviceResource
		{
		public:
			DeviceBuffer(std::shared_ptr<RenderBuffer> res, std::shared_ptr<GraphContext> context);
			~DeviceBuffer() override;
			const std::shared_ptr<graphics::Buffer> getBuffer() const {
				return _buff;
			}
		protected:
			std::shared_ptr<graphics::Buffer> _buff{ nullptr };
		};

		class DeviceGraphPass {
		public:
			DeviceGraphPass(std::shared_ptr<BaseGraphPass> graphPass, std::shared_ptr<GraphContext> context);
			virtual ~DeviceGraphPass();
			inline std::shared_ptr<BaseGraphPass> getGraphPass() const { return _graphPass; }
			inline std::shared_ptr<RenderPass> getRenderPass() const { return _renderPass; }
			inline const std::vector <std::shared_ptr<graphics::Framebuffer>> getFramebuffers() const { return _framebuffers; }
			void execute(uint32_t cmdBuffId);
		protected:
			const std::shared_ptr<RenderVisitor> _renderVisitor = std::make_shared<RenderVisitor>();
			std::shared_ptr<BaseGraphPass> _graphPass{ nullptr };
			std::shared_ptr<GraphContext> _context{ nullptr };
			std::shared_ptr<RenderPass> _renderPass{ nullptr };
			
			// Postprocess
			std::shared_ptr<ShaderStage> _shader{ nullptr };
			// set and ds
			std::unordered_map<uint32_t, std::shared_ptr<DescriptorSet>> _descSets{};
			std::shared_ptr<PipelineLayout> _pipelineLayout{ nullptr };
			std::shared_ptr<VertexInput> _vertInput{ nullptr };
			std::shared_ptr<GraphicsPipeline> _pipeline{ nullptr };

			void _createScreenQuad();
			void _createDescriptor();
			void _createPipeline();

			std::vector<std::shared_ptr<DeviceTexture>> _inputTexs;
			// only screenpass will have multiple framebuffers
			std::vector<std::shared_ptr<graphics::Framebuffer>> _framebuffers{};
			std::shared_ptr<RenderPass> _createRenderPass(const PassView&, const std::vector<graphics::Format>& colors, graphics::Format dStencil);
		};

		class DeviceRenderPhase {

		};

		class GraphContext
		{
		public:
			GraphContext(std::shared_ptr<RenderGraph> renderGraph);
			virtual ~GraphContext();

			void setRenderGraph(std::shared_ptr<RenderGraph> renderGraph);
			const std::unordered_map<std::string, std::shared_ptr<DeviceResource>>& getDeviceResources() const { return _resources; }
			const std::vector<std::shared_ptr<DeviceGraphPass>>& getRenderPasses() const { return _passes; }
			const std::shared_ptr<graphics::Device> getDevice() const { return _device; }
			const std::shared_ptr<graphics::Context> getGfxContext() const { return _gfxContext; }
			std::shared_ptr<RenderGraph> getRenderGraph() const;
			void addPass(std::shared_ptr<DeviceGraphPass> pass);
			void addResource(std::shared_ptr<DeviceResource> res);
			void resetPasses();
			void clearResources();
		private:
			std::shared_ptr<RenderGraph> _renderGraph{ nullptr };
			std::shared_ptr<graphics::Context> _gfxContext{nullptr};
			std::shared_ptr<graphics::Device> _device{ nullptr };
			std::unordered_map<std::string, std::shared_ptr<DeviceResource>> _resources{};
			std::vector<std::shared_ptr<DeviceGraphPass>> _passes{};
		};
	}
}
