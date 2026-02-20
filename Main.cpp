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
#include <array>

int main(int argc, char* argv[]) {

    upointer<Engine> l_p_engine = std::make_unique<Engine>();

    if (!l_p_engine.get()->createWindow())
    {
        debugLog("[Main] can't create a window");
        return -1;
    }

    upointer<MD0> l_p_md0 = std::make_unique<MD0>();
    //l_p_md0.get()->loadMD0("qarmor.md0");
    l_p_md0.get()->loadMD0("book.md0");

    upointer<Input> l_p_input = std::make_unique<Input>();
    CameraMovement l_camera_movement;

    upointer<Shader> l_p_shader = std::make_unique<Shader>();
    l_p_shader.get()->loadShader("default.vert", "default.frag");
    l_p_shader.get()->sendShader();

    upointer<Texture> l_p_albedo = std::make_unique<Texture>();
    l_p_albedo.get()->loadTexture("book.png", TextureShaderType::Albedo);

    upointer<Texture> l_p_specular = std::make_unique<Texture>();
    l_p_specular.get()->loadTexture("book_special.png", TextureShaderType::Specular);

    upointer<Texture> l_p_normal = std::make_unique<Texture>();
    l_p_normal.get()->loadTexture("book_normal.png", TextureShaderType::Normal);

    upointer<Texture> l_p_environment = std::make_unique<Texture>();
    l_p_environment.get()->loadTexture("environment.png", TextureShaderType::Environment);

    upointer<Texture> l_p_arm = std::make_unique<Texture>();
    l_p_arm.get()->loadTexture("book_glossiness_metallic.png", TextureShaderType::AORoughnessMetallic);

    l_p_albedo.get()->setTextureToShader(l_p_shader.get());
    l_p_normal.get()->setTextureToShader(l_p_shader.get());
    l_p_specular.get()->setTextureToShader(l_p_shader.get());
    l_p_environment.get()->setTextureToShader(l_p_shader.get());
    l_p_arm.get()->setTextureToShader(l_p_shader.get());

    std::array<Texture*, 5> l_textures;
    l_textures[0] = l_p_albedo.get();
    l_textures[1] = l_p_normal.get();
    l_textures[2] = l_p_specular.get();
    l_textures[3] = l_p_environment.get();
    l_textures[4] = l_p_arm.get();

    //upointer<Mesh> l_p_mesh = std::make_unique<Mesh>(l_p_shader.get(), l_p_texture0.get());

    upointer<Camera> l_p_camera = std::make_unique<Camera>();
    l_p_camera.get()->setCameraToShader(l_p_shader.get());

    upointer<Light> l_p_light = std::make_unique<Light>();
    l_p_light.get()->setPostion({ 358.0, 343.0 });
    l_p_light.get()->setRadius(85);
    l_p_light.get()->setIntensity(1);
    l_camera_movement.setLight(l_p_light.get());

    l_p_shader.get()->setLightToShader(l_p_light.get());

    upointer<Render> l_p_render = std::make_unique<Render>();

    //Time;
    uint frame_start;
    uint frame_time;
    float l_time = 0.0f;

    l_p_engine.get()->alphaBlend(false);

    //Enable depth testing;
    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LEQUAL);

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
        l_p_render.get()->drawMesh(l_p_md0.get()->getMeshes(), l_textures, l_p_shader.get());

        l_p_shader.get()->setLightToShader(l_p_light.get());

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