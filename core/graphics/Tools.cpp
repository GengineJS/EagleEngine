/*
 *  Tools.cpp
 *
 *  Copyright (c) 2022 by XiangKui Zheng
 *  This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */

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
    auto device = dynamic_cast<VKDevice*>(context->getDevice().get());
    return getVKSupportedDepthFormat(device->getPhysicalDevice(), reinterpret_cast<VkFormat*>(depthFormat));
#endif
    return false;
}

bool isSupportedDepthFormat(Format depthFormat){
    auto context = Context::GetContext();
#if defined(USE_GFX_VULKAN)
    auto device = dynamic_cast<VKDevice*>(context->getDevice().get());
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
