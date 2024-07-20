#include "Common/Base.hpp"
#include "TextureOverride/Entry.hpp"
#include "TextureOverride/Hooks.hpp"
#include "TextureOverride/Loading.hpp"


#ifndef SDK_TARGET_LE3
    // This is only a proof of concept for now.
    #error TextureOverride target only supports LE3 at the moment.
#endif


SPI_PLUGINSIDE_SUPPORT(L"LE3TextureOverride", L"d00telemental", L"0.1.0", SPI_GAME_LE3, SPI_VERSION_ANY);
SPI_PLUGINSIDE_POSTLOAD;
SPI_PLUGINSIDE_ASYNCATTACH;


SPI_IMPLEMENT_ATTACH
{
    ::LESDK::Initializer Init{ InterfacePtr, "LE3TextureOverride" };

    ::LESDK::InitializeConsole();
    ::TextureOverride::InitializeLogger();
    ::TextureOverride::InitializeGlobals(Init);
    ::TextureOverride::InitializeHooks(Init);

    LEASI_INFO("hello there, {}!", "LE3TextureOverride");
    ::TextureOverride::LoadDlcManifests();

    return true;
}

SPI_IMPLEMENT_DETACH
{
    LEASI_UNUSED(InterfacePtr);
    ::LESDK::TerminateConsole();
    return true;
}


namespace TextureOverride
{
    void InitializeLogger()
    {
        spdlog::default_logger()->set_pattern("%^[%H:%M:%S.%e] [%l] (LE3TextureOverride) %v%$");
        spdlog::default_logger()->set_level(spdlog::level::trace);
    }

#define CHECK_RESOLVED(variable)                                                    \
    do {                                                                            \
        LEASI_VERIFYA(variable != nullptr, "failed to resolve " #variable, "");     \
        LEASI_TRACE("resolved " #variable " => {}", (void*)variable);               \
    } while (false)

    void InitializeGlobals(::LESDK::Initializer& Init)
    {
        GMalloc = Init.ResolveTyped<FMallocLike*>(BUILTIN_GMALLOC_RIP);
        CHECK_RESOLVED(GMalloc);

        //GEngine = Init.ResolveTyped<UEngine*>(BUILTIN_GENGINE_RIP);
        //CHECK_RESOLVED(GEngine);
        //GNatives = Init.ResolveTyped<tNative*>(BUILTIN_GNATIVES_RIP);
        //CHECK_RESOLVED(GNatives);
        //GSys = Init.ResolveTyped<USystem*>(BUILTIN_GSYS_RIP);
        //CHECK_RESOLVED(GSys);
        //GWorld = Init.ResolveTyped<UWorld*>(BUILTIN_GWORLD_RIP);
        //CHECK_RESOLVED(GWorld);

        UObject::GObjObjects = Init.ResolveTyped<TArray<UObject*>>(BUILTIN_GOBOBJECTS_RIP);
        CHECK_RESOLVED(UObject::GObjObjects);
        SFXName::GBioNamePools = Init.ResolveTyped<SFXNameEntry const*>(BUILTIN_SFXNAMEPOOLS_RIP);
        CHECK_RESOLVED(SFXName::GBioNamePools);
        SFXName::GInitMethod = Init.ResolveTyped<SFXName::tInitMethod>(BUILTIN_SFXNAMEINIT_PHOOK);
        CHECK_RESOLVED(SFXName::GInitMethod);

        LEASI_INFO("globals initialized");
    }

    void InitializeHooks(::LESDK::Initializer& Init)
    {
        auto const UTexture2D_Serialize_target = Init.ResolveTyped<t_UTexture2D_Serialize>(UTEXTURE2D_SERIALIZE_RVA);
        CHECK_RESOLVED(UTexture2D_Serialize_target);
        UTexture2D_Serialize_orig = (t_UTexture2D_Serialize*)Init.InstallHook("UTexture2D::Serialize", UTexture2D_Serialize_target, UTexture2D_Serialize_hook);
        CHECK_RESOLVED(UTexture2D_Serialize_orig);

        LEASI_INFO("hooks initialized");
    }
}
