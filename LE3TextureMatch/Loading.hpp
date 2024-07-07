#pragma once

#include <memory>
#include <vector>
#include "LExCommon/Base.hpp"
#include "LE3TextureMatch/Manifest.hpp"


namespace LExTextureMatch
{
    extern std::vector<std::shared_ptr<ManifestLoader>> g_loadedManifests;
    static constexpr std::wstring_view k_searchFoldersRoot = L"../../BioGame/DLC/";

    void LoadDlcManifests();
}
