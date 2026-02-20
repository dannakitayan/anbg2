#include "Path.h"
#define DEBUG

#ifdef DEBUG
string g_debug_path = "C:\\Users\\a4cht\\source\\repos\\ShadersTest\\Assets\\";
#endif

string* getBaseFolderPath()
{
#ifndef DEBUG
    if (g_base_folder_path.empty()) {
        const char* l_p_base_directory_path = SDL_GetBasePath();
        if (!l_p_base_directory_path)
            return nullptr;

        g_base_folder_path = string(l_p_base_directory_path) + g_base_folder_name;
        SDL_free((void*)l_p_base_directory_path);
    }
    return &g_base_folder_path;
#else
    return &g_debug_path;
#endif
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
    case Folder::Models:
        return "Models\\";
    default:
        return "";
    }
}

string getAssetPath(Folder folder, const char* p_asset_name)
{
    string l_base_folder_path = *getBaseFolderPath();
    return l_base_folder_path + getFolderName(folder) + p_asset_name;
}