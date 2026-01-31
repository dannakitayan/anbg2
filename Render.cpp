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

void Render::drawMesh(const vector<Mesh*>& kModels, Texture* p_texture, Shader* p_shader)
{
    if (kModels.size() <= 0) return;

    for (const auto& l_kModel : kModels)
    {
        p_shader->activateShader();

        if (p_shader->model != -1)
            glUniformMatrix4fv(p_shader->model, 1, GL_FALSE, glm::value_ptr(l_kModel->getModel()));

        if (p_shader->texture0 != -1)
            glUniform1i(p_shader->texture0, 0);

        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, p_texture->getId());

        l_kModel->getVAO()->bind();
        glDrawElements(GL_TRIANGLES, l_kModel->getIndexesCount(), GL_UNSIGNED_INT, nullptr);
        l_kModel->getVAO()->unbind();

        glBindTexture(GL_TEXTURE_2D, 0);
    }
}
