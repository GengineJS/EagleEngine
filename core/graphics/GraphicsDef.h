#pragma once

#include <cstdint>
#include <string>
#include <utils/Utils.h>
#include <unordered_map>
namespace eg {
    namespace graphics {

        ENUM_FLAGS(Format) {
            UNDEFINED = 0,
                R4G4_UNORM_PACK8 = 1,
                R4G4B4A4_UNORM_PACK16 = 2,
                B4G4R4A4_UNORM_PACK16 = 3,
                R5G6B5_UNORM_PACK16 = 4,
                B5G6R5_UNORM_PACK16 = 5,
                R5G5B5A1_UNORM_PACK16 = 6,
                B5G5R5A1_UNORM_PACK16 = 7,
                A1R5G5B5_UNORM_PACK16 = 8,
                R8_UNORM = 9,
                R8_SNORM = 10,
                R8_USCALED = 11,
                R8_SSCALED = 12,
                R8_UINT = 13,
                R8_SINT = 14,
                R8_SRGB = 15,
                R8G8_UNORM = 16,
                R8G8_SNORM = 17,
                R8G8_USCALED = 18,
                R8G8_SSCALED = 19,
                R8G8_UINT = 20,
                R8G8_SINT = 21,
                R8G8_SRGB = 22,
                R8G8B8_UNORM = 23,
                R8G8B8_SNORM = 24,
                R8G8B8_USCALED = 25,
                R8G8B8_SSCALED = 26,
                R8G8B8_UINT = 27,
                R8G8B8_SINT = 28,
                R8G8B8_SRGB = 29,
                B8G8R8_UNORM = 30,
                B8G8R8_SNORM = 31,
                B8G8R8_USCALED = 32,
                B8G8R8_SSCALED = 33,
                B8G8R8_UINT = 34,
                B8G8R8_SINT = 35,
                B8G8R8_SRGB = 36,
                R8G8B8A8_UNORM = 37,
                R8G8B8A8_SNORM = 38,
                R8G8B8A8_USCALED = 39,
                R8G8B8A8_SSCALED = 40,
                R8G8B8A8_UINT = 41,
                R8G8B8A8_SINT = 42,
                R8G8B8A8_SRGB = 43,
                B8G8R8A8_UNORM = 44,
                B8G8R8A8_SNORM = 45,
                B8G8R8A8_USCALED = 46,
                B8G8R8A8_SSCALED = 47,
                B8G8R8A8_UINT = 48,
                B8G8R8A8_SINT = 49,
                B8G8R8A8_SRGB = 50,
                A8B8G8R8_UNORM_PACK32 = 51,
                A8B8G8R8_SNORM_PACK32 = 52,
                A8B8G8R8_USCALED_PACK32 = 53,
                A8B8G8R8_SSCALED_PACK32 = 54,
                A8B8G8R8_UINT_PACK32 = 55,
                A8B8G8R8_SINT_PACK32 = 56,
                A8B8G8R8_SRGB_PACK32 = 57,
                A2R10G10B10_UNORM_PACK32 = 58,
                A2R10G10B10_SNORM_PACK32 = 59,
                A2R10G10B10_USCALED_PACK32 = 60,
                A2R10G10B10_SSCALED_PACK32 = 61,
                A2R10G10B10_UINT_PACK32 = 62,
                A2R10G10B10_SINT_PACK32 = 63,
                A2B10G10R10_UNORM_PACK32 = 64,
                A2B10G10R10_SNORM_PACK32 = 65,
                A2B10G10R10_USCALED_PACK32 = 66,
                A2B10G10R10_SSCALED_PACK32 = 67,
                A2B10G10R10_UINT_PACK32 = 68,
                A2B10G10R10_SINT_PACK32 = 69,
                R16_UNORM = 70,
                R16_SNORM = 71,
                R16_USCALED = 72,
                R16_SSCALED = 73,
                R16_UINT = 74,
                R16_SINT = 75,
                R16_SFLOAT = 76,
                R16G16_UNORM = 77,
                R16G16_SNORM = 78,
                R16G16_USCALED = 79,
                R16G16_SSCALED = 80,
                R16G16_UINT = 81,
                R16G16_SINT = 82,
                R16G16_SFLOAT = 83,
                R16G16B16_UNORM = 84,
                R16G16B16_SNORM = 85,
                R16G16B16_USCALED = 86,
                R16G16B16_SSCALED = 87,
                R16G16B16_UINT = 88,
                R16G16B16_SINT = 89,
                R16G16B16_SFLOAT = 90,
                R16G16B16A16_UNORM = 91,
                R16G16B16A16_SNORM = 92,
                R16G16B16A16_USCALED = 93,
                R16G16B16A16_SSCALED = 94,
                R16G16B16A16_UINT = 95,
                R16G16B16A16_SINT = 96,
                R16G16B16A16_SFLOAT = 97,
                R32_UINT = 98,
                R32_SINT = 99,
                R32_SFLOAT = 100,
                R32G32_UINT = 101,
                R32G32_SINT = 102,
                R32G32_SFLOAT = 103,
                R32G32B32_UINT = 104,
                R32G32B32_SINT = 105,
                R32G32B32_SFLOAT = 106,
                R32G32B32A32_UINT = 107,
                R32G32B32A32_SINT = 108,
                R32G32B32A32_SFLOAT = 109,
                R64_UINT = 110,
                R64_SINT = 111,
                R64_SFLOAT = 112,
                R64G64_UINT = 113,
                R64G64_SINT = 114,
                R64G64_SFLOAT = 115,
                R64G64B64_UINT = 116,
                R64G64B64_SINT = 117,
                R64G64B64_SFLOAT = 118,
                R64G64B64A64_UINT = 119,
                R64G64B64A64_SINT = 120,
                R64G64B64A64_SFLOAT = 121,
                B10G11R11_UFLOAT_PACK32 = 122,
                E5B9G9R9_UFLOAT_PACK32 = 123,
                D16_UNORM = 124,
                X8_D24_UNORM_PACK32 = 125,
                D32_SFLOAT = 126,
                S8_UINT = 127,
                D16_UNORM_S8_UINT = 128,
                D24_UNORM_S8_UINT = 129,
                D32_SFLOAT_S8_UINT = 130,
                BC1_RGB_UNORM_BLOCK = 131,
                BC1_RGB_SRGB_BLOCK = 132,
                BC1_RGBA_UNORM_BLOCK = 133,
                BC1_RGBA_SRGB_BLOCK = 134,
                BC2_UNORM_BLOCK = 135,
                BC2_SRGB_BLOCK = 136,
                BC3_UNORM_BLOCK = 137,
                BC3_SRGB_BLOCK = 138,
                BC4_UNORM_BLOCK = 139,
                BC4_SNORM_BLOCK = 140,
                BC5_UNORM_BLOCK = 141,
                BC5_SNORM_BLOCK = 142,
                BC6H_UFLOAT_BLOCK = 143,
                BC6H_SFLOAT_BLOCK = 144,
                BC7_UNORM_BLOCK = 145,
                BC7_SRGB_BLOCK = 146,
                ETC2_R8G8B8_UNORM_BLOCK = 147,
                ETC2_R8G8B8_SRGB_BLOCK = 148,
                ETC2_R8G8B8A1_UNORM_BLOCK = 149,
                ETC2_R8G8B8A1_SRGB_BLOCK = 150,
                ETC2_R8G8B8A8_UNORM_BLOCK = 151,
                ETC2_R8G8B8A8_SRGB_BLOCK = 152,
                EAC_R11_UNORM_BLOCK = 153,
                EAC_R11_SNORM_BLOCK = 154,
                EAC_R11G11_UNORM_BLOCK = 155,
                EAC_R11G11_SNORM_BLOCK = 156,
                ASTC_4x4_UNORM_BLOCK = 157,
                ASTC_4x4_SRGB_BLOCK = 158,
                ASTC_5x4_UNORM_BLOCK = 159,
                ASTC_5x4_SRGB_BLOCK = 160,
                ASTC_5x5_UNORM_BLOCK = 161,
                ASTC_5x5_SRGB_BLOCK = 162,
                ASTC_6x5_UNORM_BLOCK = 163,
                ASTC_6x5_SRGB_BLOCK = 164,
                ASTC_6x6_UNORM_BLOCK = 165,
                ASTC_6x6_SRGB_BLOCK = 166,
                ASTC_8x5_UNORM_BLOCK = 167,
                ASTC_8x5_SRGB_BLOCK = 168,
                ASTC_8x6_UNORM_BLOCK = 169,
                ASTC_8x6_SRGB_BLOCK = 170,
                ASTC_8x8_UNORM_BLOCK = 171,
                ASTC_8x8_SRGB_BLOCK = 172,
                ASTC_10x5_UNORM_BLOCK = 173,
                ASTC_10x5_SRGB_BLOCK = 174,
                ASTC_10x6_UNORM_BLOCK = 175,
                ASTC_10x6_SRGB_BLOCK = 176,
                ASTC_10x8_UNORM_BLOCK = 177,
                ASTC_10x8_SRGB_BLOCK = 178,
                ASTC_10x10_UNORM_BLOCK = 179,
                ASTC_10x10_SRGB_BLOCK = 180,
                ASTC_12x10_UNORM_BLOCK = 181,
                ASTC_12x10_SRGB_BLOCK = 182,
                ASTC_12x12_UNORM_BLOCK = 183,
                ASTC_12x12_SRGB_BLOCK = 184,
                G8B8G8R8_422_UNORM = 1000156000,
                B8G8R8G8_422_UNORM = 1000156001,
                G8_B8_R8_3PLANE_420_UNORM = 1000156002,
                G8_B8R8_2PLANE_420_UNORM = 1000156003,
                G8_B8_R8_3PLANE_422_UNORM = 1000156004,
                G8_B8R8_2PLANE_422_UNORM = 1000156005,
                G8_B8_R8_3PLANE_444_UNORM = 1000156006,
                R10X6_UNORM_PACK16 = 1000156007,
                R10X6G10X6_UNORM_2PACK16 = 1000156008,
                R10X6G10X6B10X6A10X6_UNORM_4PACK16 = 1000156009,
                G10X6B10X6G10X6R10X6_422_UNORM_4PACK16 = 1000156010,
                B10X6G10X6R10X6G10X6_422_UNORM_4PACK16 = 1000156011,
                G10X6_B10X6_R10X6_3PLANE_420_UNORM_3PACK16 = 1000156012,
                G10X6_B10X6R10X6_2PLANE_420_UNORM_3PACK16 = 1000156013,
                G10X6_B10X6_R10X6_3PLANE_422_UNORM_3PACK16 = 1000156014,
                G10X6_B10X6R10X6_2PLANE_422_UNORM_3PACK16 = 1000156015,
                G10X6_B10X6_R10X6_3PLANE_444_UNORM_3PACK16 = 1000156016,
                R12X4_UNORM_PACK16 = 1000156017,
                R12X4G12X4_UNORM_2PACK16 = 1000156018,
                R12X4G12X4B12X4A12X4_UNORM_4PACK16 = 1000156019,
                G12X4B12X4G12X4R12X4_422_UNORM_4PACK16 = 1000156020,
                B12X4G12X4R12X4G12X4_422_UNORM_4PACK16 = 1000156021,
                G12X4_B12X4_R12X4_3PLANE_420_UNORM_3PACK16 = 1000156022,
                G12X4_B12X4R12X4_2PLANE_420_UNORM_3PACK16 = 1000156023,
                G12X4_B12X4_R12X4_3PLANE_422_UNORM_3PACK16 = 1000156024,
                G12X4_B12X4R12X4_2PLANE_422_UNORM_3PACK16 = 1000156025,
                G12X4_B12X4_R12X4_3PLANE_444_UNORM_3PACK16 = 1000156026,
                G16B16G16R16_422_UNORM = 1000156027,
                B16G16R16G16_422_UNORM = 1000156028,
                G16_B16_R16_3PLANE_420_UNORM = 1000156029,
                G16_B16R16_2PLANE_420_UNORM = 1000156030,
                G16_B16_R16_3PLANE_422_UNORM = 1000156031,
                G16_B16R16_2PLANE_422_UNORM = 1000156032,
                G16_B16_R16_3PLANE_444_UNORM = 1000156033,
                PVRTC1_2BPP_UNORM_BLOCK_IMG = 1000054000,
                PVRTC1_4BPP_UNORM_BLOCK_IMG = 1000054001,
                PVRTC2_2BPP_UNORM_BLOCK_IMG = 1000054002,
                PVRTC2_4BPP_UNORM_BLOCK_IMG = 1000054003,
                PVRTC1_2BPP_SRGB_BLOCK_IMG = 1000054004,
                PVRTC1_4BPP_SRGB_BLOCK_IMG = 1000054005,
                PVRTC2_2BPP_SRGB_BLOCK_IMG = 1000054006,
                PVRTC2_4BPP_SRGB_BLOCK_IMG = 1000054007,
                ASTC_4x4_SFLOAT_BLOCK_EXT = 1000066000,
                ASTC_5x4_SFLOAT_BLOCK_EXT = 1000066001,
                ASTC_5x5_SFLOAT_BLOCK_EXT = 1000066002,
                ASTC_6x5_SFLOAT_BLOCK_EXT = 1000066003,
                ASTC_6x6_SFLOAT_BLOCK_EXT = 1000066004,
                ASTC_8x5_SFLOAT_BLOCK_EXT = 1000066005,
                ASTC_8x6_SFLOAT_BLOCK_EXT = 1000066006,
                ASTC_8x8_SFLOAT_BLOCK_EXT = 1000066007,
                ASTC_10x5_SFLOAT_BLOCK_EXT = 1000066008,
                ASTC_10x6_SFLOAT_BLOCK_EXT = 1000066009,
                ASTC_10x8_SFLOAT_BLOCK_EXT = 1000066010,
                ASTC_10x10_SFLOAT_BLOCK_EXT = 1000066011,
                ASTC_12x10_SFLOAT_BLOCK_EXT = 1000066012,
                ASTC_12x12_SFLOAT_BLOCK_EXT = 1000066013,
                G8_B8R8_2PLANE_444_UNORM_EXT = 1000330000,
                G10X6_B10X6R10X6_2PLANE_444_UNORM_3PACK16_EXT = 1000330001,
                G12X4_B12X4R12X4_2PLANE_444_UNORM_3PACK16_EXT = 1000330002,
                G16_B16R16_2PLANE_444_UNORM_EXT = 1000330003,
                A4R4G4B4_UNORM_PACK16_EXT = 1000340000,
                A4B4G4R4_UNORM_PACK16_EXT = 1000340001,
        };

        enum class SampleCountFlag
        {
            COUNT_1 = 0x00000001,
            COUNT_2 = 0x00000002,
            COUNT_4 = 0x00000004,
            COUNT_8 = 0x00000008,
            COUNT_16 = 0x00000010,
            COUNT_32 = 0x00000020,
            COUNT_64 = 0x00000040
        };

        ENUM_FLAGS(TextureUsageFlag)
        {
            TRANSFER_SRC = 0x00000001,
                TRANSFER_DST = 0x00000002,
                SAMPLED = 0x00000004,
                STORAGE = 0x00000008,
                COLOR_ATTACHMENT = 0x00000010,
                DEPTH_STENCIL_ATTACHMENT = 0x00000020,
                TRANSIENT_ATTACHMENT = 0x00000040,
                INPUT_ATTACHMENT = 0x00000080,
                SHADING_RATE_IMAGE_NV = 0x00000100,
                FRAGMENT_DENSITY_MAP_EXT = 0x00000200
        };

        ENUM_FLAGS(TextureType) {
            TEX_1D,
                TEX_2D,
                TEX_3D,
                TEX_CUBE,
                TEX1D_ARRAY,
                TEX2D_ARRAY,
                CUBE_ARRAY
        };

        enum class TextureTiling {
            OPTIMAL = 0,
            LINEAR = 1,
            DRM_FORMAT_MODIFIER_EXT = 1000158000
        };

        ENUM_FLAGS(TextureLayout) {
            UNDEFINED = 0,
                GENERAL = 1,
                COLOR_ATTACHMENT_OPTIMAL = 2,
                DEPTH_STENCIL_ATTACHMENT_OPTIMAL = 3,
                DEPTH_STENCIL_READ_ONLY_OPTIMAL = 4,
                SHADER_READ_ONLY_OPTIMAL = 5,
                TRANSFER_SRC_OPTIMAL = 6,
                TRANSFER_DST_OPTIMAL = 7,
                PREINITIALIZED = 8,
                DEPTH_READ_ONLY_STENCIL_ATTACHMENT_OPTIMAL = 1000117000,
                DEPTH_ATTACHMENT_STENCIL_READ_ONLY_OPTIMAL = 1000117001,
                PRESENT_SRC_KHR = 1000001002,
                SHARED_PRESENT_KHR = 1000111000,
                SHADING_RATE_OPTIMAL_NV = 1000164003,
                FRAGMENT_DENSITY_MAP_OPTIMAL_EXT = 1000218000,
                DEPTH_READ_ONLY_STENCIL_ATTACHMENT_OPTIMAL_KHR = DEPTH_READ_ONLY_STENCIL_ATTACHMENT_OPTIMAL,
                DEPTH_ATTACHMENT_STENCIL_READ_ONLY_OPTIMAL_KHR = DEPTH_ATTACHMENT_STENCIL_READ_ONLY_OPTIMAL,
        };

        ENUM_FLAGS(SharingMode) {
            EXCLUSIVE = 0,
                CONCURRENT = 1,
        };

        ENUM_FLAGS(TextureAspectFlag) {
            COLOR = 0x00000001,
                DEPTH = 0x00000002,
                STENCIL = 0x00000004,
                METADATA = 0x00000008,
                PLANE_0 = 0x00000010,
                PLANE_1 = 0x00000020,
                PLANE_2 = 0x00000040,
                MEMORY_PLANE_0_EXT = 0x00000080,
                MEMORY_PLANE_1_EXT = 0x00000100,
                MEMORY_PLANE_2_EXT = 0x00000200,
                MEMORY_PLANE_3_EXT = 0x00000400,
        };

        ENUM_FLAGS(QueueFlag)
        {
            GRAPHICS = 0x00000001,
                COMPUTE = 0x00000002,
                TRANSFER = 0x00000004,
                SPARSE_BINDING = 0x00000008,
                PROTECTED = 0x00000010,
        };

        ENUM_FLAGS(CommandPoolCreateFlag) {
            TRANSIENT = 0x00000001,
                RESET_COMMAND_BUFFER = 0x00000002,
                PROTECTED = 0x00000004,
        };

        enum class CommandBufferLevel {
            PRIMARY = 0,
            SECONDARY = 1,
        };

        ENUM_FLAGS(BufferUsageFlag) {
            TRANSFER_SRC = 0x00000001,
                TRANSFER_DST = 0x00000002,
                UNIFORM_TEXEL = 0x00000004,
                STORAGE_TEXEL = 0x00000008,
                UNIFORM = 0x00000010,
                STORAGE = 0x00000020,
                INDEX = 0x00000040,
                VERTEX = 0x00000080,
                INDIRECT = 0x00000100,
                TRANSFORM_FEEDBACK_EXT = 0x00000800,
                TRANSFORM_FEEDBACK_COUNTER_EXT = 0x00001000,
                CONDITIONAL_RENDERING_EXT = 0x00000200,
                RAY_TRACING_NV = 0x00000400,
                SHADER_DEVICE_ADDRESS_EXT = 0x00020000,
        };

        ENUM_FLAGS(ShaderStageFlag) {
                NONE = 0,
                VERTEX = 0x00000001,
                TESSELLATION_CONTROL = 0x00000002,
                TESSELLATION_EVALUATION = 0x00000004,
                GEOMETRY = 0x00000008,
                FRAGMENT = 0x00000010,
                COMPUTE = 0x00000020,
                ALL_GRAPHICS = 0x0000001F,
                ALL = 0x7FFFFFFF,
                RAYGEN_NV = 0x00000100,
                ANY_HIT_NV = 0x00000200,
                CLOSEST_HIT_NV = 0x00000400,
                MISS_NV = 0x00000800,
                INTERSECTION_NV = 0x00001000,
                CALLABLE_NV = 0x00002000,
                TASK_NV = 0x00000040,
                MESH_NV = 0x00000080,
        };

        ENUM_FLAGS(AttachmentLoadOp) {
            LOAD = 0,
                CLEAR = 1,
                DONT_CARE = 2,
        };

        ENUM_FLAGS(AttachmentStoreOp) {
            STORE = 0,
                DONT_CARE = 1,
        };

        ENUM_FLAGS(PipelineBindPoint) {
            GRAPHICS = 0,
                COMPUTE = 1,
                RAY_TRACING_NV = 1000165000,
        };

        struct ColorAttachment
        {
            Format format{ Format::B8G8R8A8_UNORM };
            SampleCountFlag samples{ SampleCountFlag::COUNT_1 };
            AttachmentLoadOp loadOp{ AttachmentLoadOp::CLEAR };
            AttachmentStoreOp storeOp{ AttachmentStoreOp::STORE };
            TextureLayout initLayout{ TextureLayout::UNDEFINED };
            TextureLayout finalLayout{ TextureLayout::COLOR_ATTACHMENT_OPTIMAL };
        };

        struct DepthStencilAttachment {
            Format format{ Format::D24_UNORM_S8_UINT };
            SampleCountFlag samples{ SampleCountFlag::COUNT_1 };
            AttachmentLoadOp loadOp{ AttachmentLoadOp::CLEAR };
            AttachmentStoreOp storeOp{ AttachmentStoreOp::STORE };
            AttachmentLoadOp stencilLoadOp{ AttachmentLoadOp::DONT_CARE };
            AttachmentStoreOp stencilStoreOp{ AttachmentStoreOp::DONT_CARE };
            TextureLayout initLayout{ TextureLayout::UNDEFINED };
            TextureLayout finalLayout{ TextureLayout::DEPTH_STENCIL_ATTACHMENT_OPTIMAL };
        };

        ENUM_FLAGS(PipelineStageFlag) {
            TOP_OF_PIPE = 0x00000001,
                DRAW_INDIRECT = 0x00000002,
                VERTEX_INPUT = 0x00000004,
                VERTEX_SHADER = 0x00000008,
                TESSELLATION_CONTROL_SHADER = 0x00000010,
                TESSELLATION_EVALUATION_SHADER = 0x00000020,
                GEOMETRY_SHADER = 0x00000040,
                FRAGMENT_SHADER = 0x00000080,
                EARLY_FRAGMENT_TESTS = 0x00000100,
                LATE_FRAGMENT_TESTS = 0x00000200,
                COLOR_ATTACHMENT_OUTPUT = 0x00000400,
                COMPUTE_SHADER = 0x00000800,
                TRANSFER = 0x00001000,
                BOTTOM_OF_PIPE = 0x00002000,
                HOST = 0x00004000,
                ALL_GRAPHICS = 0x00008000,
                ALL_COMMANDS = 0x00010000,
                TRANSFORM_FEEDBACK_EXT = 0x01000000,
                CONDITIONAL_RENDERING_EXT = 0x00040000,
                COMMAND_PROCESS_NVX = 0x00020000,
                SHADING_RATE_IMAGE_NV = 0x00400000,
                RAY_TRACING_SHADER_NV = 0x00200000,
                ACCELERATION_STRUCTURE_BUILD_NV = 0x02000000,
                TASK_SHADER_NV = 0x00080000,
                MESH_SHADER_NV = 0x00100000,
                FRAGMENT_DENSITY_PROCESS_EXT = 0x00800000,
        };

        ENUM_FLAGS(AccessFlag) {
                NONE = 0,
                INDIRECT_COMMAND_READ = 0x00000001,
                INDEX_READ = 0x00000002,
                VERTEX_ATTRIBUTE_READ = 0x00000004,
                UNIFORM_READ = 0x00000008,
                INPUT_ATTACHMENT_READ = 0x00000010,
                SHADER_READ = 0x00000020,
                SHADER_WRITE = 0x00000040,
                COLOR_ATTACHMENT_READ = 0x00000080,
                COLOR_ATTACHMENT_WRITE = 0x00000100,
                DEPTH_STENCIL_ATTACHMENT_READ = 0x00000200,
                DEPTH_STENCIL_ATTACHMENT_WRITE = 0x00000400,
                TRANSFER_READ = 0x00000800,
                TRANSFER_WRITE = 0x00001000,
                HOST_READ = 0x00002000,
                HOST_WRITE = 0x00004000,
                MEMORY_READ = 0x00008000,
                MEMORY_WRITE = 0x00010000,
                TRANSFORM_FEEDBACK_WRITE_EXT = 0x02000000,
                TRANSFORM_FEEDBACK_COUNTER_READ_EXT = 0x04000000,
                TRANSFORM_FEEDBACK_COUNTER_WRITE_EXT = 0x08000000,
                CONDITIONAL_RENDERING_READ_EXT = 0x00100000,
                COMMAND_PROCESS_READ_NVX = 0x00020000,
                COMMAND_PROCESS_WRITE_NVX = 0x00040000,
                COLOR_ATTACHMENT_READ_NONCOHERENT_EXT = 0x00080000,
                SHADING_RATE_IMAGE_READ_NV = 0x00800000,
                ACCELERATION_STRUCTURE_READ_NV = 0x00200000,
                ACCELERATION_STRUCTURE_WRITE_NV = 0x00400000,
                FRAGMENT_DENSITY_MAP_READ_EXT = 0x01000000,
        };

        ENUM_FLAGS(DependencyFlag) {
            BY_REGION = 0x00000001,
                DEVICE_GROUP = 0x00000004,
                VIEW_LOCAL = 0x00000002,
                VIEW_LOCAL_KHR = VIEW_LOCAL,
                DEVICE_GROUP_KHR = DEVICE_GROUP,
        };

        enum class PrimitiveTopology {
            POINT_LIST = 0,
            LINE_LIST = 1,
            LINE_STRIP = 2,
            TRIANGLE_LIST = 3,
            TRIANGLE_STRIP = 4,
            TRIANGLE_FAN = 5,
            LINE_LIST_WITH_ADJACENCY = 6,
            LINE_STRIP_WITH_ADJACENCY = 7,
            TRIANGLE_LIST_WITH_ADJACENCY = 8,
            TRIANGLE_STRIP_WITH_ADJACENCY = 9,
            PATCH_LIST = 10,
        };

        ENUM_FLAGS(DescriptorType) {
            SAMPLER = 0,
                COMBINED_IMAGE_SAMPLER = 1,
                SAMPLED_IMAGE = 2,
                STORAGE_IMAGE = 3,
                UNIFORM_TEXEL_BUFFER = 4,
                STORAGE_TEXEL_BUFFER = 5,
                UNIFORM_BUFFER = 6,
                STORAGE_BUFFER = 7,
                UNIFORM_BUFFER_DYNAMIC = 8,
                STORAGE_BUFFER_DYNAMIC = 9,
                INPUT_ATTACHMENT = 10,
                INLINE_UNIFORM_BLOCK_EXT = 1000138000,
                ACCELERATION_STRUCTURE_NV = 1000165000,
        };

        enum class DescriptorSetLayoutCreateFlag {
            PUSH_DESCRIPTOR_KHR = 0x00000001,
            UPDATE_AFTER_BIND_POOL_EXT = 0x00000002,
        };

        enum class Filter {
            NEAREST = 0,
            LINEAR = 1,
            CUBIC_IMG = 1000015000,
            CUBIC_EXT = CUBIC_IMG,
        };

        enum class SamplerCreateFlag {
            SUBSAMPLED_EXT = 0x00000001,
            SUBSAMPLED_COARSE_RECONSTRUCTION_EXT = 0x00000002,
        };

        enum class SamplerMipmapMode {
            NEAREST = 0,
            LINEAR = 1,
        };

        enum class SamplerAddressMode {
            REPEAT = 0,
            MIRRORED_REPEAT = 1,
            CLAMP_TO_EDGE = 2,
            CLAMP_TO_BORDER = 3,
            MIRROR_CLAMP_TO_EDGE = 4,
        };

        enum class CompareOp {
            NEVER = 0,
            LESS = 1,
            EQUAL = 2,
            LESS_OR_EQUAL = 3,
            GREATER = 4,
            NOT_EQUAL = 5,
            GREATER_OR_EQUAL = 6,
            ALWAYS = 7,
        };

        enum class BorderColor {
            FLOAT_TRANSPARENT_BLACK = 0,
            INT_TRANSPARENT_BLACK = 1,
            FLOAT_OPAQUE_BLACK = 2,
            INT_OPAQUE_BLACK = 3,
            FLOAT_OPAQUE_WHITE = 4,
            INT_OPAQUE_WHITE = 5,
        };

        enum DescriptorPoolFlag {
            NONE = 0,
            FREE_DESCRIPTOR_SET = 0x00000001,
            UPDATE_AFTER_BIND_EXT = 0x00000002,
            HOST_ONLY_BIT_VALVE = 0x00000004,
        };

        ENUM_FLAGS(DynamicState) {
            VIEWPORT = 0,
                SCISSOR = 1,
                LINE_WIDTH = 2,
                DEPTH_BIAS = 3,
                BLEND_CONSTANTS = 4,
                DEPTH_BOUNDS = 5,
                STENCIL_COMPARE_MASK = 6,
                STENCIL_WRITE_MASK = 7,
                STENCIL_REFERENCE = 8,
                VIEWPORT_W_SCALING_NV = 1000087000,
                DISCARD_RECTANGLE_EXT = 1000099000,
                SAMPLE_LOCATIONS_EXT = 1000143000,
                VIEWPORT_SHADING_RATE_PALETTE_NV = 1000164004,
                VIEWPORT_COARSE_SAMPLE_ORDER_NV = 1000164006,
                EXCLUSIVE_SCISSOR_NV = 1000205001,
        };

        enum class VertexInputRate {
            VERTEX = 0,
            INSTANCE = 1,
        };

        enum class PolygonMode {
            FILL = 0,
            LINE = 1,
            POINT = 2,
            FILL_RECTANGLE_NV = 1000153000,
        };

        enum class CullModeFlag {
            NONE = 0,
            FRONT = 0x00000001,
            BACK = 0x00000002,
            FRONT_AND_BACK = 0x00000003,
        };

        enum class FrontFace {
            COUNTER_CLOCKWISE = 0,
            CLOCKWISE = 1,
        };

        enum class StencilOp {
            KEEP = 0,
            ZERO = 1,
            REPLACE = 2,
            INCREMENT_AND_CLAMP = 3,
            DECREMENT_AND_CLAMP = 4,
            INVERT = 5,
            INCREMENT_AND_WRAP = 6,
            DECREMENT_AND_WRAP = 7,
        };

        enum class BlendFactor {
            ZERO = 0,
            ONE = 1,
            SRC_COLOR = 2,
            ONE_MINUS_SRC_COLOR = 3,
            DST_COLOR = 4,
            ONE_MINUS_DST_COLOR = 5,
            SRC_ALPHA = 6,
            ONE_MINUS_SRC_ALPHA = 7,
            DST_ALPHA = 8,
            ONE_MINUS_DST_ALPHA = 9,
            CONSTANT_COLOR = 10,
            ONE_MINUS_CONSTANT_COLOR = 11,
            CONSTANT_ALPHA = 12,
            ONE_MINUS_CONSTANT_ALPHA = 13,
            SRC_ALPHA_SATURATE = 14,
            SRC1_COLOR = 15,
            ONE_MINUS_SRC1_COLOR = 16,
            SRC1_ALPHA = 17,
            ONE_MINUS_SRC1_ALPHA = 18,
        };

        enum class BlendOp {
            ADD = 0,
            SUBTRACT = 1,
            REVERSE_SUBTRACT = 2,
            MIN = 3,
            MAX = 4,
            ZERO_EXT = 1000148000,
            SRC_EXT = 1000148001,
            DST_EXT = 1000148002,
            SRC_OVER_EXT = 1000148003,
            DST_OVER_EXT = 1000148004,
            SRC_IN_EXT = 1000148005,
            DST_IN_EXT = 1000148006,
            SRC_OUT_EXT = 1000148007,
            DST_OUT_EXT = 1000148008,
            SRC_ATOP_EXT = 1000148009,
            DST_ATOP_EXT = 1000148010,
            XOR_EXT = 1000148011,
            MULTIPLY_EXT = 1000148012,
            SCREEN_EXT = 1000148013,
            OVERLAY_EXT = 1000148014,
            DARKEN_EXT = 1000148015,
            LIGHTEN_EXT = 1000148016,
            COLORDODGE_EXT = 1000148017,
            COLORBURN_EXT = 1000148018,
            HARDLIGHT_EXT = 1000148019,
            SOFTLIGHT_EXT = 1000148020,
            DIFFERENCE_EXT = 1000148021,
            EXCLUSION_EXT = 1000148022,
            INVERT_EXT = 1000148023,
            INVERT_RGB_EXT = 1000148024,
            LINEARDODGE_EXT = 1000148025,
            LINEARBURN_EXT = 1000148026,
            VIVIDLIGHT_EXT = 1000148027,
            LINEARLIGHT_EXT = 1000148028,
            PINLIGHT_EXT = 1000148029,
            HARDMIX_EXT = 1000148030,
            HSL_HUE_EXT = 1000148031,
            HSL_SATURATION_EXT = 1000148032,
            HSL_COLOR_EXT = 1000148033,
            HSL_LUMINOSITY_EXT = 1000148034,
            PLUS_EXT = 1000148035,
            PLUS_CLAMPED_EXT = 1000148036,
            PLUS_CLAMPED_ALPHA_EXT = 1000148037,
            PLUS_DARKER_EXT = 1000148038,
            MINUS_EXT = 1000148039,
            MINUS_CLAMPED_EXT = 1000148040,
            CONTRAST_EXT = 1000148041,
            INVERT_OVG_EXT = 1000148042,
            RED_EXT = 1000148043,
            GREEN_EXT = 1000148044,
            BLUE_EXT = 1000148045,
        };

        ENUM_FLAGS(ColorComponentFlag) {
            R = 0x00000001,
            G = 0x00000002,
            B = 0x00000004,
            A = 0x00000008
        };

        enum class LogicOp {
            CLEAR = 0,
            AND = 1,
            AND_REVERSE = 2,
            COPY = 3,
            AND_INVERTED = 4,
            NO_OP = 5,
            XOR = 6,
            OR = 7,
            NOR = 8,
            EQUIVALENT = 9,
            INVERT = 10,
            OR_REVERSE = 11,
            COPY_INVERTED = 12,
            OR_INVERTED = 13,
            NAND = 14,
            SET = 15,
        };

        enum class GraphicsAPI {
            GLES2,
            GLES3,
            WEBGL,
            WEBGPU,
            VULKAN
        };

        enum class ColorSpace {
            SRGB_NONLINEAR = 0,
            DISPLAY_P3_NONLINEAR_EXT = 1000104001,
            EXTENDED_SRGB_LINEAR_EXT = 1000104002,
            DISPLAY_P3_LINEAR_EXT = 1000104003,
            DCI_P3_NONLINEAR_EXT = 1000104004,
            BT709_LINEAR_EXT = 1000104005,
            BT709_NONLINEAR_EXT = 1000104006,
            BT2020_LINEAR_EXT = 1000104007,
            HDR10_ST2084_EXT = 1000104008,
            DOLBYVISION_EXT = 1000104009,
            HDR10_HLG_EXT = 1000104010,
            ADOBERGB_LINEAR_EXT = 1000104011,
            ADOBERGB_NONLINEAR_EXT = 1000104012,
            PASS_THROUGH_EXT = 1000104013,
            EXTENDED_SRGB_NONLINEAR_EXT = 1000104014,
            DISPLAY_NATIVE_AMD = 1000213000,
        };

        enum class ComponentSwizzle {
            IDENTITY = 0,
            ZERO = 1,
            ONE = 2,
            R = 3,
            G = 4,
            B = 5,
            A = 6
        };

        struct ComponentMapping {
            ComponentSwizzle    r;
            ComponentSwizzle    g;
            ComponentSwizzle    b;
            ComponentSwizzle    a;
        };

        struct Rect2D {
            int32_t x{ 0 };
            int32_t y{ 0 };
            uint32_t w{ 0 };
            uint32_t h{ 0 };
        };

        struct Vector4
        {
            float x{ 0 };
            float y{ 0 };
            float z{ 0 };
            float w{ 0 };
        };

        struct Vector3
        {
            float x{ 0 };
            float y{ 0 };
            float z{ 0 };
        };

        struct Vector2
        {
            float x{ 0 };
            float y{ 0 };
        };

        struct ClearColor {
            float r{ 0. };
            float g{ 0. };
            float b{ 0. };
            float a{ 1. };
        };

        struct ClearDepthStencil {
            float       depth{ 1. };
            uint32_t    stencil{ 0 };
        };

        struct Viewport {
            float    x{ 0. };
            float    y{ 0. };
            float    w{ 0. };
            float    h{ 0. };
            float    minDepth{ 0. };
            float    maxDepth{ 1. };
        };
        ENUM_FLAGS(MemoryPropertyFlag) {
            DEVICE_LOCAL = 0x00000001,
            HOST_VISIBLE = 0x00000002,
            HOST_COHERENT = 0x00000004,
            HOST_CACHED = 0x00000008,
            LAZILY_ALLOCATED = 0x00000010,
            PROTECTED = 0x00000020,
            DEVICE_COHERENT_AMD = 0x00000040,
            DEVICE_UNCACHED_AMD = 0x00000080,
            RDMA_CAPABLE_NV = 0x00000100
        };
        enum class SubpassContents {
            INLINE = 0,
            SECONDARY_COMMAND_BUFFERS = 1,
        };

        struct ShaderDesc
        {
            // path and stageFlag
            std::unordered_map<std::string, graphics::ShaderStageFlag> shaders{};
            inline void add(const std::string& name, graphics::ShaderStageFlag stage) {
                shaders.insert({name, stage});
            }
            inline void remove(const std::string& name) {
                if (shaders.find(name) != shaders.end()) {
                    shaders.erase(name);
                }
            }
            inline void clear() {
                shaders.clear();
            }
        };

        struct ShaderStageInfo {
            const uint32_t* pCode{ nullptr };
            uint32_t size;
            ShaderStageFlag stage;
            std::string pName{ "main" };
        };
    }
}
