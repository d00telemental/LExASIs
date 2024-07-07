#include "LE3TextureMatch/Hooks.hpp"
#include "LE3TextureMatch/Loading.hpp"

namespace LExTextureMatch
{
    inline FString const& GetTextureFullName(UTexture2D* const InObject)
    {
        thread_local FString OutString{};
        OutString.Clear();

        if (InObject->Class != nullptr)
        {
            if (InObject->Outer != nullptr)
            {
                if (InObject->Outer->Outer != nullptr)
                {
                    LESDK::AppendObjectName(InObject->Outer->Outer, OutString, SFXName::k_formatBasic);
                    OutString.Append(L".");
                }

                LESDK::AppendObjectName(InObject->Outer, OutString, SFXName::k_formatBasic);
                OutString.Append(L".");
            }

            LESDK::AppendObjectName(InObject, OutString, SFXName::k_formatInstanced);
            return OutString;
        }

        OutString.Append(L"(null)");
        return OutString;
    }

    t_UTexture2D_Serialize* UTexture2D_Serialize_orig = nullptr;
    void UTexture2D_Serialize_hook(UTexture2D* const Context, void* const Archive)
    {
        auto const& TextureFullName = GetTextureFullName(Context);

        //LEASI_TRACE(L"UTexture2D::Serialize: {}", *TextureFullName);
        (*UTexture2D_Serialize_orig)(Context, Archive);

        auto const Mips = *(TArrayView<CMipMapInfo const*>*) &Context->Mips;
        LEASI_UNUSED(Mips);

        for (auto const& Manifest : g_loadedManifests)
        {
            CTextureEntry const* const Entry = Manifest->FindEntry(TextureFullName);
            if (Entry == nullptr)
                continue;

            LEASI_DEBUG(L"UTexture2D::Serialize: replacing {}", *TextureFullName);
        }
    }
}
