#pragma once

#include <LESDK/Headers.hpp>
#include "LExCommon/Base.hpp"


namespace LExTextureMatch
{
	enum EPixelFormat : std::uint16_t
	{
		PF_Unknown						= 0,
		PF_A32B32G32R32F                = 1,
		PF_A8R8G8B8                     = 2,
		PF_G8                           = 3,
		PF_G16                          = 4,
		PF_DXT1                         = 5,
		PF_DXT3                         = 6,
		PF_DXT5                         = 7,
		PF_UYVY                         = 8,
		PF_FloatRGB                     = 9,
		PF_FloatRGBA                    = 10,
		PF_DepthStencil                 = 11,
		PF_ShadowDepth                  = 12,
		PF_FilteredShadowDepth          = 13,
		PF_R32F                         = 14,
		PF_G16R16                       = 15,
		PF_G16R16F                      = 16,
		PF_G16R16F_FILTER               = 17,
		PF_G32R32F                      = 18,
		PF_A2B10G10R10                  = 19,
		PF_A16B16G16R16_UNORM           = 20,
		PF_D24                          = 21,
		PF_R16F                         = 22,
		PF_R16F_FILTER                  = 23,
		PF_BC5                          = 24,
		PF_V8U8                         = 25,
		PF_A1                           = 26,
		PF_NormalMap_LQ                 = 27,
		PF_NormalMap_HQ                 = 28,
		PF_A16B16G16R16_FLOAT           = 29,
		PF_A16B16G16R16_SNORM           = 30,
		PF_FloatR11G11B10               = 31,
		PF_A4R4G4B4                     = 32,
		PF_R5G6B5                       = 33,
		PF_G8R8                         = 34,
		PF_R8_UNORM                     = 35,
		PF_R8_UINT                      = 36,
		PF_R8_SINT                      = 37,
		PF_R16_FLOAT                    = 38,
		PF_R16_UNORM                    = 39,
		PF_R16_UINT                     = 40,
		PF_R16_SINT                     = 41,
		PF_R8G8_UNORM                   = 42,
		PF_R8G8_UINT                    = 43,
		PF_R8G8_SINT                    = 44,
		PF_R16G16_FLOAT                 = 45,
		PF_R16G16_UNORM                 = 46,
		PF_R16G16_UINT                  = 47,
		PF_R16G16_SINT                  = 48,
		PF_R32_FLOAT                    = 49,
		PF_R32_UINT                     = 50,
		PF_R32_SINT                     = 51,
		PF_A8                           = 52,
		PF_BC7                          = 53,
		EPixelFormat_MAX                = 54
	};

    enum EMipFlags : std::uint16_t
    {
        EMF_Placeholder					= 1 << 0,
        EMF_External					= 1 << 1,
    };

#pragma pack(push, 1)
    struct CManifestHeader
    {
        unsigned char   Magic[6];       // Magic bytes of 'LETEXM'.
        std::uint16_t   Version;        // Manifest version (not the mod version).
        std::uint32_t   TargetHash;     // FNV-1 (32 bit) of containing folder name, or UINT32_MAX for joint deployment.
        std::uint32_t   TextureCount;   // Number of @ref CTextureEntry entries after this header.
        unsigned char   Reserved[16];

        static constexpr decltype(Magic)    k_checkMagic{ 'L', 'E', 'T', 'E', 'X', 'M' };
        static constexpr decltype(Version)  k_lastVersion{ 1 };
    };

    static_assert(sizeof(CManifestHeader) == 32);

    struct CMipEntry
    {
        static constexpr size_t k_maxTfcLength = 24;

        EMipFlags       Flags;
        std::uint32_t   UnpackedSize;
        std::uint32_t   PackedSize;
        std::uint32_t   PackedOffset;
        char            TfcName[k_maxTfcLength];
    };

    struct CTextureEntry
    {
        static constexpr size_t k_maxIfpLength = 256;
        static constexpr size_t k_maxMipCount = 16;

        char            InstancedFullPath[k_maxIfpLength];  // Instanced full path of the Texture2D entry being replaced.
        CMipEntry       Mips[k_maxMipCount];                // Mip records, their count and meta must match the original mips.
		EPixelFormat	Format;								// Pixel format for all mips, must match LE definitions.
    };

#pragma pack(pop)
}
