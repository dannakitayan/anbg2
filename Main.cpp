#define SDL_MAIN_HANDLED

#include "Base.h"
#include "Engine.h"
#include "Debug.h"
#include "Input.h"
#include "CameraMovement.h"
#include "Shader.h"
#include "Texture.h"
#include "Plain.h"
#include "Camera.h"
#include "Render.h"
#include "Light.h"
#include "MD0.h"

int main(int argc, char* argv[]) {

    upointer<Engine> l_p_engine = std::make_unique<Engine>();

    if (!l_p_engine.get()->createWindow())
    {
        debugLog("[Main] can't create a window");
        return -1;
    }

    upointer<MD0> l_p_md0 = std::make_unique<MD0>();
    l_p_md0.get()->loadMD0("test.md0");

    upointer<Input> l_p_input = std::make_unique<Input>();
    CameraMovement l_camera_movement;

    upointer<Shader> l_p_shader = std::make_unique<Shader>();
    l_p_shader.get()->loadShader("default.vert", "default.frag");
    l_p_shader.get()->sendShader();

    /*upointer<Shader> l_p_screen_shader = std::make_unique<Shader>();
    l_p_screen_shader.get()->loadShader("screen.vert", "screen.frag");
    l_p_screen_shader.get()->sendShader();*/

    upointer<Texture> l_p_texture0 = std::make_unique<Texture>();
    l_p_texture0.get()->loadTexture("test.png", TextureShaderType::Texture0);

    upointer<Texture> l_p_texture1 = std::make_unique<Texture>();
    l_p_texture1.get()->loadTexture("texture1.png", TextureShaderType::Texture1);

    upointer<Texture> l_p_normal = std::make_unique<Texture>();
    l_p_normal.get()->loadTexture("normalmap.png", TextureShaderType::NormalMap);

    l_p_texture0.get()->setTextureToShader(l_p_shader.get());
    l_p_texture1.get()->setTextureToShader(l_p_shader.get());
    l_p_normal.get()->setTextureToShader(l_p_shader.get());

    //upointer<Mesh> l_p_mesh = std::make_unique<Mesh>(l_p_shader.get(), l_p_texture0.get());

    upointer<Camera> l_p_camera = std::make_unique<Camera>();
    l_p_camera.get()->setCameraToShader(l_p_shader.get());

    upointer<Light> l_p_light = std::make_unique<Light>();
    l_p_light.get()->setPostion({ 177, 180 });
    l_p_light.get()->setRadius(5);
    l_p_light.get()->setIntensity(9);
    l_camera_movement.setLight(l_p_light.get());

    //l_p_shader.get()->setLightToShader(l_p_light.get());

    upointer<Render> l_p_render = std::make_unique<Render>();

    //Time;
    uint frame_start;
    uint frame_time;
    float l_time = 0.0f;

    l_p_engine.get()->alphaBlend(false);

    //Main;
    while (g_is_engine_run)
    {
        frame_start = SDL_GetTicks();

        uint current_time = SDL_GetTicks();
        g_delta_time = (current_time - frame_start) / 1000.0f;
        if (g_delta_time <= 0.0f) g_delta_time = 0.001f;

        l_p_engine.get()->cleanScreen();

        //Input;
        l_p_input.get()->processAction(&l_camera_movement);

        l_time += g_delta_time;
        l_p_shader.get()->activateShader();
        l_p_shader.get()->setShaderUniform1f("time", l_time);

        
        //Draw;
        l_p_render.get()->drawMesh(l_p_md0.get()->getMeshes(), l_p_texture0.get(), l_p_shader.get());

        //l_p_shader.get()->setLightToShader(l_p_light.get());

        frame_time = SDL_GetTicks() - frame_start;
        if (g_frame_delay > frame_time)
        {
            SDL_Delay(g_frame_delay - frame_time);
        }

        g_fps = 1000.0f / static_cast<float>(SDL_GetTicks() - frame_start);

        l_p_engine.get()->swapBuffers();
    }

    return 0;
}