/*
 *  Tools.h
 *
 *  Copyright (c) 2022 by XiangKui Zheng
 *  This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */

#include <graphics/GraphicsDef.h>
namespace eg {
namespace graphics {
    bool getSupportedDepthFormat(Format *depthFormat);

    bool isSupportedDepthFormat(Format depthFormat);
    
    Format getSupportedDepthFormat(Format depthFormat);
}
}
