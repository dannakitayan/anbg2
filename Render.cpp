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

void Render::drawMesh(Mesh* const p_mesh)
{
    if (!p_mesh) return;

    Shader* shader = p_mesh->getShader();
    Texture* tex = p_mesh->getTexture();
    VAO* vao = p_mesh->getVAO();

    if (!shader || !tex || !vao) return;

    shader->activateShader();

    if (shader->model != -1)
        glUniformMatrix4fv(shader->model, 1, GL_FALSE, glm::value_ptr(p_mesh->getModel()));

    if (shader->texture0 != -1)
        glUniform1i(shader->texture0, 0);

    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, tex->getId());

    vao->bind();
    glDrawElements(GL_TRIANGLES, p_mesh->getIndexCount(), GL_UNSIGNED_INT, nullptr);
    vao->unbind();

    glBindTexture(GL_TEXTURE_2D, 0);
}
