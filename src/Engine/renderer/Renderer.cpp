#include "Renderer.hpp"

#include <vector>

#include <glad/glad.h>
#include <glm/gtc/matrix_transform.hpp>

void Renderer::initRenderer()
{

}

void Renderer::renderAll(xplor::Shader& shader, glm::mat4 view, int fov, int width, int height)
{
    shader.setMat4("view", view);

    glm::mat4 projection = glm::perspective(glm::radians(float(fov)), (float)width / (float)height, 0.1f, 200.0f);
    shader.setMat4("projection", projection);
}