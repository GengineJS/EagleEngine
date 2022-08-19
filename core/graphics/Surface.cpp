#include <graphics/Surface.h>
#include <type_traits>
namespace eg {
    namespace graphics {
        Surface::Surface(const SurfaceInfo& info) :_info(info) {}

        Surface::~Surface()
        {
        }
    }
}
