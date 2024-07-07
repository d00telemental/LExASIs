#include <filesystem>
#include "LE3TextureMatch/Loading.hpp"

namespace fs = std::filesystem;

namespace LExTextureMatch
{
    std::vector<std::shared_ptr<ManifestLoader>> g_loadedManifests{};

    void LoadDlcManifests()
    {
        fs::path const DlcFolder{ k_searchFoldersRoot };
        LEASI_INFO(L"looking for dlc roots in {}", DlcFolder.c_str());
        
        for (fs::directory_entry const& DlcRoot : fs::directory_iterator{ DlcFolder })
        {
            auto const& DlcPath = DlcRoot.path();
            std::wstring const DlcName{ DlcPath.filename().c_str() };

            if (!DlcRoot.is_directory() || !DlcName.starts_with(L"DLC_MOD_"))
            {
                //LEASI_TRACE(L"disregarding {}, not a valid directory", DlcPath.c_str());
                continue;
            }

            auto const ManifestPath = DlcPath / std::format(L"{}_TextureOverride.letexm", DlcName);
            LEASI_DEBUG(L"looking for manifest {}", ManifestPath.c_str());

            if (!fs::exists(ManifestPath))
                continue;

            FString LoadError{};
            LEASI_DEBUG(L"loading manifest {}", ManifestPath.c_str());

            auto Manifest = std::make_shared<ManifestLoader>();
            if (!Manifest->Load(ManifestPath.wstring(), LoadError))
            {
                LEASI_ERROR(L"failed to load manifest {}", ManifestPath.c_str());
                LEASI_ERROR(L"error: {}", *LoadError);
                continue;
            }

            g_loadedManifests.push_back(std::move(Manifest));
        }
    }
}
