#include <graphics/Sampler.h>
#include <graphics/Context.h>
namespace eg {
    namespace graphics {
        Sampler::Sampler(const SamplerInfo& info) : _info(info) {}
        Sampler::~Sampler()
        {
        }
    }
}
