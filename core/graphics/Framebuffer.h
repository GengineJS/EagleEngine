#pragma once

#include <graphics/Texture.h>
#include <vector>
namespace eg {
    namespace graphics {
        class RenderPass;
        struct FramebufferInfo {
            FramebufferInfo() = default;
            FramebufferInfo(uint32_t w, uint32_t h, std::shared_ptr<RenderPass> renderPass,
                const std::vector<std::shared_ptr<Texture>>& attachments) {
                width = w;
                height = h;
                this->renderPass = renderPass;
                this->pAttachments = attachments;
            }
            std::shared_ptr<RenderPass> renderPass{ nullptr };
            std::vector<std::shared_ptr<Texture>> pAttachments{};
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
