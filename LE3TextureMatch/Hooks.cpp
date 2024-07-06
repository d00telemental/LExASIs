#include "LE3TextureMatch/Hooks.hpp"

namespace LExTextureMatch
{
    t_UTexture2D_Serialize* UTexture2D_Serialize_orig = nullptr;
    void UTexture2D_Serialize_hook(UTexture2D* const Context, void* const Archive)
    {
        LEASI_TRACE(L"UTexture2D::Serialize, context = {}", *Context->GetFullName());
        (*UTexture2D_Serialize_orig)(Context, Archive);

        auto const Mips = *(TArrayView<CMipMapInfo const*>*)&Context->Mips;
        LEASI_UNUSED(Mips);
    }
}
