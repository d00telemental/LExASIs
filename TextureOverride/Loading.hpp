#pragma once

#include <cassert>
#include <memory>
#include <vector>
#include <LESDK/Headers.hpp>
#include "Common/Base.hpp"
#include "Common/Memory.hpp"
#include "TextureOverride/Manifest.hpp"


namespace TextureOverride
{
    // ! Engine types.
    // ========================================

    enum ETextureFlags : std::uint32_t
    {
        ETF_External            = 1 << 0,           // Mip is stored in an external file (texture file cache).
        ETF_SingleUse           = 1 << 3,           // Purpose unknown...
        ETF_OodleCompression    = 1 << 12,          // Mip is compressed with Oodle.
    };

    #pragma pack(push, 1)

    struct CMipMapInfo final : public Common::AllocateThroughEngine
    {
        void*                   Vftable;            // 0x00
        ETextureFlags           Flags;              // 0x08
        std::int32_t            Elements;           // 0x0C
        std::int32_t            CompressedOffset;   // 0x10
        std::int32_t            CompressedSize;     // 0x14
        unsigned char           Unknown_18[0x10];   // 0x18
        void*                   Data;               // 0x28
        int                     Unknown_2C;         // 0x2C
        void*                   Archive;            // 0x34
        int                     bNeedsFree;         // 0x3C
        std::int32_t            Width;              // 0x40
        std::int32_t            Height;             // 0x44
    };

    static_assert(sizeof(CMipMapInfo) == 0x48);
    static_assert(offsetof(CMipMapInfo, Flags) == 0x08);
    static_assert(offsetof(CMipMapInfo, Data) == 0x28);
    static_assert(offsetof(CMipMapInfo, Archive) == 0x34);
    static_assert(offsetof(CMipMapInfo, bNeedsFree) == 0x3C);
    static_assert(offsetof(CMipMapInfo, Width) == 0x40);
    static_assert(offsetof(CMipMapInfo, Height) == 0x44);

    #pragma pack(pop)


    // ! Texture loading.
    // ========================================

    extern std::vector<std::shared_ptr<ManifestLoader>> g_loadedManifests;
    static constexpr std::wstring_view k_searchFoldersRoot = L"../../BioGame/DLC/";

    void LoadDlcManifests();

    FString const& GetTextureFullName(UTexture2D* InObject);
    void UpdateTextureFromManifest(UTexture2D* InTexture, ManifestLoader const& Manifest, CTextureEntry const& Entry);
}
