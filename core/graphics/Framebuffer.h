/*
 *  Framebuffer.h
 *
 *  Copyright (c) 2022 by XiangKui Zheng
 *  This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */

#pragma once

#include <graphics/Texture.h>
#include <vector>
namespace eg {
    namespace graphics {
        class RenderPass;
        struct FramebufferInfo {
            FramebufferInfo() = default;
            FramebufferInfo(uint32_t w, uint32_t h, const RenderPass* renderPass,
                std::vector<Texture*>&& attachments) {
                width = w;
                height = h;
                this->renderPass = const_cast<RenderPass*>(renderPass);
                this->pAttachments = std::move(attachments);
            }
            RenderPass* renderPass{ nullptr };
            std::vector<Texture*> pAttachments{};
            uint32_t width{0};
            uint32_t height{0};
            uint32_t layers{ 1 };
        };

        class Framebuffer
        {
        public:
            Framebuffer() = default;
            Framebuffer(const FramebufferInfo&);
            virtual ~Framebuffer();
            virtual void destroy() = 0;
            inline const FramebufferInfo& getFramebufferInfo() const { return _info; }
        protected:
            FramebufferInfo _info{};
        };
    }
}
