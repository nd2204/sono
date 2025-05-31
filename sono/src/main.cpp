#include "sngl/glshader.h"
#include "sono/snwindow.h"

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <iostream>

f32 vertices[] = {-1.0f, -0.5f, 0.0f, 0.0f, -0.5f, 0.0f, -0.5f, 0.5f, 0.0f};
f32 vertices2[] = {-0.0f, -0.5f, 0.0f, 1.0f, -0.5f, 0.0f, 0.5f, 0.5f, 0.0f};

void Init() {
  /* Initialize the library */
  if (!glfwInit())
    exit(EXIT_FAILURE);

  glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
  glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
  glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
#ifdef MACOS_BUILD
  glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif
}

void ProcessInput(SNWindow &window) {
  if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
    glfwSetWindowShouldClose(window, true);
}

i32 main(void) {
  Init();

  /* Create a windowed mode window and its OpenGL context */
  SNWindow window(800, 600, "Hello Sono");

  /* Make the window's context current */
  glfwSetFramebufferSizeCallback(
    window,
    [](GLFWwindow *window, int width, int height) {
      std::cout << "Changing viewport w:" << width << "h: " << height
                << std::endl;
      glViewport(0, 0, width, height);
    }
  );

  u32 VAO[2];
  glGenVertexArrays(2, VAO);
  u32 VBO[2];
  glGenBuffers(2, VBO);

  glBindVertexArray(VAO[0]);
  glBindBuffer(GL_ARRAY_BUFFER, VBO[0]);
  glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
  glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(f32), (void *)0);
  glEnableVertexAttribArray(0);
  glBindVertexArray(0);

  glBindVertexArray(VAO[1]);
  glBindBuffer(GL_ARRAY_BUFFER, VBO[1]);
  glBufferData(GL_ARRAY_BUFFER, sizeof(vertices2), vertices2, GL_STATIC_DRAW);
  glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(f32), (void *)0);
  glEnableVertexAttribArray(0);
  glBindVertexArray(0);

  GLShaderProgram &shaderProgram = GLShaderProgram::DefaultPipeLine();

  /* Loop until the user closes the window */
  while (!glfwWindowShouldClose(window)) {
    ProcessInput(window);

    /* Render here */
    glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);

    shaderProgram.Use();
    glBindVertexArray(VAO[0]);
    glDrawArrays(GL_TRIANGLES, 0, 3);
    glBindVertexArray(VAO[1]);
    glDrawArrays(GL_TRIANGLES, 0, 3);
    glBindVertexArray(0);

    /* Swap front and back buffers */
    glfwSwapBuffers(window);

    /* Poll for and process events */
    glfwPollEvents();
  }

  glDeleteVertexArrays(2, VAO);
  glDeleteBuffers(2, VBO);
  glDeleteProgram(shaderProgram);

  glfwTerminate();
  return 0;
}