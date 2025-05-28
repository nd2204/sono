#include "common/types.h"

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <iostream>

f32 vertices[] = {-0.5f, -0.5f, 0.0f, 0.5f, -0.5f, 0.0f, 0.0f, 0.5f, 0.0f};

void init() {
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

void framebuffer_size_callback(GLFWwindow *window, int width, int height) {
  std::cout << "Changing viewport w:" << width << "h: " << height << std::endl;
  glViewport(0, 0, width, height);
}

void processInput(GLFWwindow *window) {
  if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
    glfwSetWindowShouldClose(window, true);
}

i32 main(void) {
  init();

  GLFWwindow *window;

  /* Create a windowed mode window and its OpenGL context */
  window = glfwCreateWindow(800, 600, "Hello Sono", NULL, NULL);
  if (!window) {
    std::cout << "Failed to create GLFW window" << std::endl;
    glfwTerminate();
    return -1;
  }
  /* Make the window's context current */
  glfwMakeContextCurrent(window);
  glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

  if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
    std::cout << "Failed to initialize GLAD" << std::endl;
    exit(EXIT_FAILURE);
  }

  glViewport(0, 0, 400, 300);

  /* Loop until the user closes the window */
  while (!glfwWindowShouldClose(window)) {
    processInput(window);

    /* Render here */
    glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);

    /* Swap front and back buffers */
    glfwSwapBuffers(window);

    /* Poll for and process events */
    glfwPollEvents();
  }

  glfwTerminate();
  return 0;
}