#include "Base.h"

GLuint g_indices[6] =
{
	2, 0, 1, // CAB
	2, 1, 3 // CBD
};

size_t g_indices_size = sizeof(g_indices);

char g_game_name[32] = "Shaders test";
string g_base_folder_path = "";
char g_base_folder_name[6] = "Base\\";

int32 g_windows_width = 768;
int32 g_windows_height = 780;
int32 g_screen_width = 360;
int32 g_screen_height = 360;

bool g_is_engine_run = true;

float g_delta_time = 0.0f;
float g_fps = 0.0f;