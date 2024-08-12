#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"


#include "Renderer.h"
#include "VertexBuffer.h"
#include "VertexBufferLayout.h"
#include "IndexBuffer.h"
#include "VertexArray.h"
#include "Shader.h"
#include "OBJModel.h"

int main(void)
{
    GLFWwindow* window;

    /* Initialize the library */
    if (!glfwInit())
        return -1;

    /* Create a windowed mode window and its OpenGL context */
    window = glfwCreateWindow(1280, 720, "Assignment 3 - A moving Contigo!", NULL, NULL);
    if (!window)
    {
        glfwTerminate();
        return -1;
    }

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    /* Make the window's context current */
    glfwMakeContextCurrent(window);

    glfwSwapInterval(1);
    glDisable(GL_CULL_FACE);
    glClearColor(0.5f, 0.7f, 1.0f, 1.0f);

    if (glewInit() != GLEW_OK)
    {
        std::cout << "ERROR: GLEW INIT" << std::endl;
    }

    // Extracting the .OBJ
    OBJModel objModel;

    objModel.LoadFromFile("res/OBJ/contigo.obj");

    std::vector<float> vertices = objModel.GetVertexData();
    int vertexCount = objModel.GetVertexCount();

    {

      std::vector<unsigned int> indices;
      for (int i = 0; i < vertexCount; i++) {
        indices.push_back(i);
      }

      VertexArray va;
      VertexBuffer vb(&vertices[0], vertices.size() * sizeof(float)); // 3 is number of unrepeating verticies 

      VertexBufferLayout layout;
      layout.Push<float>(3); // 3 is number of coordinates a position has
      layout.Push<float>(2); // 3 is number of coordinates a texture has
      layout.Push<float>(3); // 3 is number of coordinates a normal has
      va.AddBuffer(vb, layout);

      IndexBuffer ib(&indices[0], vertexCount);

      glm::mat4 model = glm::mat4(1.0f); // Identity matrix for the model, or apply transformations as needed
      glm::mat4 view = glm::translate(glm::mat4(1.0f), glm::vec3(0, 0, -3)); // Move the camera back a bit
      view = glm::rotate(view, glm::radians(180.0f), glm::vec3(0.0f, 1.0f, 0.0f));
      //glm::mat4 projection = glm::perspective(glm::radians(45.0f), 1280.0f / 720.0f, 0.1f, 100.0f); // Perspective projection
      glm::mat4 projection = glm::ortho(-20.0f, 20.0f, -15.0f, 15.0f, -10.0f, 10.0f); // to allow more possible movement

      glm::mat4 mvp = projection * view * model; // Order: Model -> View -> Projection

      Shader shader("res/shaders/Basic.shader");

      shader.Bind(); // Binding my shader
      shader.SetUniform4f("u_color", 0.2f, 0.3f, 0.3f, 1.0f); // set data in the shader and retreiving the location of this color uniform
      shader.SetUniformMat4f("u_MVP", mvp); // set model view projection

      va.UnBind();
      vb.Unbind();
      ib.Unbind();
      shader.UnBind();

      Renderer renderer;

      /*Controls Display*/
      std::cout << " These are the controls to move the Contigo: " << std::endl
                        << "    W for up" << std::endl
                        << "    A for left" << std::endl
                        << "    S for down" << std::endl
                        << "    D for right" << std::endl
                        << "    Q for rotate Anti-CLK wise" << std::endl
                        << "    E for rotate CLK wise" << std::endl
                        << "    R for scale up" << std::endl
                        << "    F for scale down" << std::endl
                        << "    Z for rotate CLK wise on the Y axis" << std::endl
                        << "    X for rotate Anti-CLK wise on the Y axis" << std::endl
                        << "    C for rotate CLK wise on the X axis" << std::endl
                        << "    V for rotate Anti-CLK wise on the X axis" << std::endl;

      glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
      /* Loop until the user closes the window */
      while (!glfwWindowShouldClose(window))
      {
          // Process user inputs
          renderer.ProcessUserInput(window, model);

          // update projection matrix after the transformation
          mvp = projection * view * model;
      
          // Rendering Here 
          renderer.Clear();

          shader.Bind(); // Binding shader
          shader.SetUniformMat4f("u_MVP", mvp); // update projection in the shader


          renderer.Draw(va, ib, shader); // Making the openGL draw call 

          /* Swap front and back buffers */
          GLCall(glfwSwapBuffers(window));

          /* Poll for and process events */
          GLCall(glfwPollEvents());

      }

    } // scope added to terminate the program when the window is closed

    glfwTerminate();
    return 0;
}