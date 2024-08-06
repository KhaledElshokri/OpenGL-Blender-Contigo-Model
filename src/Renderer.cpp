#include "Renderer.h"
#include <iostream>

void GLClearError()
{
    while (glGetError() != GL_NO_ERROR);
}

bool GLLogCall(const char* function, const char* file, int line)
{
    while (GLenum error = glGetError())
    {
        std::cout << "[OpenGL error] (" << error << "):" << function << " " << file << ":" << line << std::endl;
        return false;
    }
    return true;
}

void Renderer::Clear() const
{
    GLCall(glClear(GL_COLOR_BUFFER_BIT));
}

void Renderer::Draw(const VertexArray& va, const IndexBuffer& ib, const Shader& shader) const
{

    shader.Bind(); // Binding shader

    va.Bind(); // Binding vertex array
    ib.Bind(); // Binding index buffer

    GLCall(glDrawElements(GL_TRIANGLES, ib.GetCount(), GL_UNSIGNED_INT, nullptr)); // issuing Draw Call
}

void Renderer::ProcessUserInput(GLFWwindow* window, glm::mat4& model)
{
  if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
    glfwSetWindowShouldClose(window, true);
  if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
    model = glm::translate(model, glm::vec3(0.0f, 0.1f, 0.0f));
  if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
    model = glm::translate(model, glm::vec3(0.0f, -0.1f, 0.0f));
  if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
    model = glm::translate(model, glm::vec3(-0.1f, 0.0f, 0.0f));
  if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
    model = glm::translate(model, glm::vec3(0.1f, 0.0f, 0.0f));
  if (glfwGetKey(window, GLFW_KEY_Q) == GLFW_PRESS)
    model = glm::rotate(model, glm::radians(30.0f), glm::vec3(0.0f, 0.0f, 1.0f));
  if (glfwGetKey(window, GLFW_KEY_E) == GLFW_PRESS)
    model = glm::rotate(model, -glm::radians(30.0f), glm::vec3(0.0f, 0.0f, 1.0f));
  if (glfwGetKey(window, GLFW_KEY_R) == GLFW_PRESS)
    model = glm::scale(model, glm::vec3(1.1f, 1.1f, 1.0f));
  if (glfwGetKey(window, GLFW_KEY_F) == GLFW_PRESS)
    model = glm::scale(model, glm::vec3(1.0f / 1.1f, 1.0f / 1.1f, 1.0f));
}
