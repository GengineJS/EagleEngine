#include <graphics/Tools.h>
#if defined(USE_GFX_VULKAN)
    #include <graphics/vulkan/VKTools.h>
    #include <graphics/vulkan/VKDevice.h>
#endif
#include <graphics/Context.h>
namespace eg {
namespace graphics {
bool getSupportedDepthFormat(Format *depthFormat) {
    auto context = Context::GetContext();
#if defined(USE_GFX_VULKAN)
    auto device = std::dynamic_pointer_cast<VKDevice>(context->getDevice());
    return getVKSupportedDepthFormat(device->getPhysicalDevice(), reinterpret_cast<VkFormat*>(depthFormat));
#endif
    return false;
}

bool isSupportedDepthFormat(Format depthFormat){
    auto context = Context::GetContext();
#if defined(USE_GFX_VULKAN)
    auto device = std::dynamic_pointer_cast<VKDevice>(context->getDevice());
    return isVKSupportedDepthFormat(device->getPhysicalDevice(), static_cast<VkFormat>(depthFormat));
#endif
    return false;
}

Format getSupportedDepthFormat(Format depthFormat) {
    if(!isSupportedDepthFormat(depthFormat)) {
        getSupportedDepthFormat(&depthFormat);
    }
    return depthFormat;
}

}
}
