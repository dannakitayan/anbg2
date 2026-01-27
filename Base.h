#pragma once
#include "BasicTypes.h"
#include <SDL.h>

extern GLuint g_indices[6];
extern size_t g_indices_size;
extern float g_delta_time;
extern float g_fps;
extern char g_game_name[32];
extern int32 g_windows_width; 
extern int32 g_windows_height;
extern int32 g_screen_width;
extern int32 g_screen_height;
extern bool g_is_engine_run;
extern string g_base_folder_path;
extern char g_base_folder_name[6];

static constexpr int g_target_fps = 60;
static constexpr int g_frame_delay = 1000 / g_target_fps;