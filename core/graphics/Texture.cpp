#include <graphics/Texture.h>
#include <graphics/Context.h>
namespace eg {
    namespace graphics {
        Texture::Texture(const TextureInfo& info) : _info(info) {}
        void Texture::create() {}
        Texture::~Texture() {
        }
        std::shared_ptr<Sampler> Texture::createSampler(const SamplerInfo& info) {
            _sampler = Context::GetContext()->getDevice()->createSampler(info);
            return _sampler;
        }
    }
}
