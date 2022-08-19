#pragma once
#include "graphics/GraphicsDef.h"
#include <vector>
namespace eg {
    namespace graphics {

        class ShaderStage
        {
        public:
            ShaderStage() = default;
            ShaderStage(const std::vector<ShaderStageInfo>& infos);
            virtual ~ShaderStage();
            inline const ShaderStageInfo& getVertexInfo() const { return _vertexInfo; }
            inline const ShaderStageInfo& getGeometryInfo() const { return _geometryInfo; }
            inline const ShaderStageInfo& getTessellatControlInfo() const { return _tessellateControlInfo; }
            inline const ShaderStageInfo& getTessellatEvaInfo() const { return _tessellateEvaInfo; }
            inline const ShaderStageInfo& getComputeInfo() const { return _computeInfo; }
            inline const ShaderStageInfo& getFragmentInfo() const { return _fragmentInfo; }
        protected:
            ShaderStageInfo _vertexInfo{};
            ShaderStageInfo _geometryInfo{};
            ShaderStageInfo _fragmentInfo{};
            ShaderStageInfo _computeInfo{};
            ShaderStageInfo _tessellateControlInfo{};
            ShaderStageInfo _tessellateEvaInfo{};
        };
    }
}