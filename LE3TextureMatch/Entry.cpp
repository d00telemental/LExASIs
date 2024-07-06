#include <LESDK/Headers.hpp>
#include <SPI.h>

#include "LExCommon/Base.hpp"


SPI_PLUGINSIDE_SUPPORT(L"LE3TextureMatch", L"d00telemental", L"0.1.0", SPI_GAME_LE3, SPI_VERSION_ANY);
SPI_PLUGINSIDE_POSTLOAD;
SPI_PLUGINSIDE_ASYNCATTACH;


SPI_IMPLEMENT_ATTACH
{
    (void)InterfacePtr;
    ::LESDK::InitializeConsole();
    ::LExCommon::InitLoggerDefault();
    LEASI_DEBUG("Hello there, {}!", "LE3TextureMatch");
    return true;
}

SPI_IMPLEMENT_DETACH
{
    (void)InterfacePtr;
    ::LESDK::TerminateConsole();
    return true;
}
