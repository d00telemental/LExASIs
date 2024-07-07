#pragma once

#include <LESDK/Headers.hpp>
#include <LESDK/Init.hpp>
#include "LExCommon/Base.hpp"


#pragma pack(push, 1)

struct CMipMapInfo
{
    void*               Vftable;            // 0x00
    std::uint32_t       Flags;              // 0x08
    std::int32_t        Elements;           // 0x0C
    std::int32_t        CompressedOffset;   // 0x10
    std::int32_t        CompressedSize;     // 0x14
    unsigned char       Unknown_18[0x10];   // 0x18
    void*               Data;               // 0x28
    int                 Unknown_2C;         // 0x2C
    void*               Archive;            // 0x34
    int                 bNeedsFree;         // 0x3C
    std::int32_t        Width;              // 0x40
    std::int32_t        Height;             // 0x44
};

static_assert(sizeof(CMipMapInfo) == 0x48);
static_assert(offsetof(CMipMapInfo, Flags) == 0x08);
static_assert(offsetof(CMipMapInfo, Data) == 0x28);
static_assert(offsetof(CMipMapInfo, Archive) == 0x34);
static_assert(offsetof(CMipMapInfo, bNeedsFree) == 0x3C);
static_assert(offsetof(CMipMapInfo, Width) == 0x40);
static_assert(offsetof(CMipMapInfo, Height) == 0x44);

#pragma pack(pop)


namespace LExTextureMatch
{
    #define UTEXTURE2D_SERIALIZE_RVA ::LESDK::Address::FromOffset(0x3C1FB0)
    using t_UTexture2D_Serialize = void(UTexture2D* Context, void* Archive);
    extern t_UTexture2D_Serialize* UTexture2D_Serialize_orig;
    void UTexture2D_Serialize_hook(UTexture2D* Context, void* Archive);
}
