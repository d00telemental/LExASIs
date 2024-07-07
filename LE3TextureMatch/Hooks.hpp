#pragma once

#include <LESDK/Headers.hpp>
#include <LESDK/Init.hpp>
#include "LExCommon/Base.hpp"


namespace LExTextureMatch
{
    #define UTEXTURE2D_SERIALIZE_RVA ::LESDK::Address::FromOffset(0x3C1FB0)
    using t_UTexture2D_Serialize = void(UTexture2D* Context, void* Archive);
    extern t_UTexture2D_Serialize* UTexture2D_Serialize_orig;
    void UTexture2D_Serialize_hook(UTexture2D* Context, void* Archive);
}
