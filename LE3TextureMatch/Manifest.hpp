#pragma once

#include <string_view>
#include <unordered_map>
#include <Windows.h>
#include <LESDK/Headers.hpp>
#include "LExCommon/Base.hpp"


template<>
struct std::hash<FString>
{
    std::size_t operator()(FString const& Key) const
    {
        // Reusing the TMap's hash implementation.
        return static_cast<std::size_t>(GetTypeHash(Key));
    }
};


namespace LExTextureMatch
{
    // ! Enumerations.
    // ========================================

    enum EPixelFormat : std::uint32_t
    {
        PF_Unknown                      = 0,
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

    enum EMipFlags : std::uint32_t
    {
        EMF_Placeholder                 = 1 << 0,   // This mip and all records after it are absent (not "empty").
        EMF_Original                    = 1 << 1,   // Mip should not be modified.
        EMF_External                    = 1 << 2,   // Mip is located in a texture file cache.
    };


    // ! Plain-old-data structs.
    // ========================================

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
        std::uint32_t   UnpackedSize;   // Number of bytes this mip occupies when fully uncompressed.
        std::uint32_t   PackedSize;     // Number of bytes this mip currently occupies on the disk.
        std::uint32_t   PackedOffset;   // Number of bytes from start of manifest to this mip contents.
        std::uint16_t   Width;          // Size in horizontal dimension.
        std::uint16_t   Height;         // Size in vertical dimension.
        EMipFlags       Flags;          // Additional settings for this mip.

        inline bool IsPlaceholder() const noexcept { return (Flags & EMF_Placeholder) != 0; }
        inline bool IsOriginal() const noexcept { return (Flags & EMF_Original) != 0; }
        inline bool IsExternal() const noexcept { return (Flags & EMF_External) != 0; }

        /** Checks if this mip is "empty" (in LEX / MEM parlance). */
        bool IsEmpty() const noexcept;
        /** Retrieves mip dimensions as a tuple. */
        std::pair<std::uint16_t, std::uint16_t> GetDimensions() const noexcept;
    };

    struct CTextureEntry
    {
        static constexpr std::size_t k_maxFullPathLength = 256;
        static constexpr std::size_t k_maxTfcNameLength = 32;
        static constexpr std::size_t k_maxMipCount = 16;

        char            FullPath[k_maxFullPathLength];  // Full path of the Texture2D entry being matched (replaced).
        char            TfcName[k_maxTfcNameLength];    // Name of the texture file cache used by external mips in this entry.
        CMipEntry       Mips[k_maxMipCount];            // Mip records, their count and meta must match the original mips.
        EPixelFormat    Format;                         // Pixel format for all mips, must match the LE definition.

        /** Retrieves the matched Texture2D path as an Unreal string. */
        FString GetFullPath() const;
        /** Retrieves the texture file cache name as an Unreal string. */
        FString GetTfcName() const;
        /** Finds the actual number of mips. */
        std::size_t GetMipCount() const;
    };

#pragma pack(pop)


    // ! Manifest types.
    // ========================================

    class ManifestLoader final : public NonCopyable
    {
        using TextureMap_t = std::unordered_map<FString, CTextureEntry const*>;

        HANDLE          FileHandle{ INVALID_HANDLE_VALUE };
        HANDLE          MappingHandle{ NULL };
        LPVOID          View{ NULL };
        SIZE_T          CachedSize{ 0 };
        TextureMap_t    TextureMap{};

    public:

        ManifestLoader() = default;
        ~ManifestLoader();

        /**
         * @brief       Attempts to initialize this manifest from a given file.
         * @param[in]   InPath - path to the manifest file being loaded.
         * @param[out]  OutError - receives error message if loading encounters an error.
         * @return      Whether loading was successful.
         */
        bool Load(std::wstring_view InPath, FString& OutError);

        struct ResolvedMip final
        {
            std::span<unsigned char const> View{};
            CMipEntry const* Entry{};
        };

        /**
         * @brief       Attempts to find a texture override entry by its @c FullPath .
         * @param[in]   FullPath - full path of the looked-for texture override.
         * @return      Pointer to the entry within the mapped memory view, or @c nullptr otherwise.
         * @warning     This manifest must be loaded before calling this method.
         */
        CTextureEntry const* FindEntry(FString const& FullPath) const;

        /**
         * @brief       Retrieves mip level info and memory view.
         * @param[in]   InEntry - texture override entry within the mapped memory view.
         * @param[in]   Index - index of the mip level within the @c InEntry given.
         * @return      Mip override entry, and the memory region it corresponds to if it's not empty.
         * @warning     This manifest must be loaded before calling this method.
         * @warning     The @c Index parameter must be within this entry's mip array bounds.
         *              Call @ref CTextureEntry::GetMipCount() beforehand to verify.
         */
        ResolvedMip GetEntryMip(CTextureEntry const& InEntry, std::size_t Index) const;

        /** Retrieves the manifest header from the start of mapped memory view. */
        CManifestHeader const& GetMappedHeader() const;
        /** Retrieves the mapped memory view. */
        std::span<unsigned char const> GetMappedView() const;

    private:

        inline bool ValidateEntry(CTextureEntry const& Ref) const noexcept
        {
            auto const Pointer = reinterpret_cast<unsigned char const*>(&Ref);
            auto const Start = reinterpret_cast<unsigned char const*>(View);
            return Pointer >= Start && Pointer < Start + CachedSize;
        }
    };

}
