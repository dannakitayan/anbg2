#include "Render.h"
#include "Mesh.h"
#include "Shader.h"
#include "Texture.h"
#include "VAO.h"
#include <glad/glad.h>
#include <gtc/type_ptr.hpp>

Render::Render()
{
}

Render::~Render()
{
}

void Render::drawMesh(const Mesh* kModel, std::array<Texture*, 5>& textures, Shader* p_shader)
{
    p_shader->activateShader();

    if (textures.size() == 0) return;

    if (p_shader->model != -1)
        glUniformMatrix4fv(p_shader->model, 1, GL_FALSE, glm::value_ptr(kModel->getModel()));

    //Check all 5 textures;
    if (p_shader->albedo != -1)
        glUniform1i(p_shader->albedo, 0);

    if (p_shader->normal_map != -1)
        glUniform1i(p_shader->normal_map, 1);

    if (p_shader->specular_map != -1)
        glUniform1i(p_shader->specular_map, 2); 

    if (p_shader->environment_map != -1)
        glUniform1i(p_shader->environment_map, 3);

    if (p_shader->glossiness_metallic != -1)
        glUniform1i(p_shader->glossiness_metallic, 4);

    //Activate and bind textures;
    for (size_t i = 0; i < textures.size(); ++i)
    {
        glActiveTexture(GL_TEXTURE0 + i);
        glBindTexture(GL_TEXTURE_2D, textures[i]->getId());
    }

    kModel->getVAO()->bind();
    glDrawElements(GL_TRIANGLES, kModel->getIndexesCount(), GL_UNSIGNED_INT, nullptr);
    kModel->getVAO()->unbind();

    glBindTexture(GL_TEXTURE_2D, 0);
}
