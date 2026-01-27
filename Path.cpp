#include "Path.h"

string* getBaseFolderPath()
{
    if (g_base_folder_path.empty()) {
        const char* l_p_base_directory_path = SDL_GetBasePath();
        if (!l_p_base_directory_path)
            return nullptr;

        g_base_folder_path = string(l_p_base_directory_path) + g_base_folder_name;
        SDL_free((void*)l_p_base_directory_path);
    }
    return &g_base_folder_path;
}

string getFolderName(Folder folder)
{
    switch (folder)
    {
    case Folder::Textures:
        return "Textures\\";
    case Folder::Sounds:
        return "Sounds\\";
    case Folder::Music:
        return "Ambients\\";
    case Folder::Shaders:
        return "Shaders\\";
    default:
        return "";
    }
}

string getAssetPath(Folder folder, const char* p_asset_name)
{
    string l_base_folder_path = *getBaseFolderPath();
    return l_base_folder_path + getFolderName(folder) + p_asset_name;
}