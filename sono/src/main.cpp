#include "imgui.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"
#include "sngl/glshader.h"
#include "sono/snwindow.h"
#include "sono/time.h"
#include "logger.h"

#include <cmath>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <iostream>

f32 vertices[] = {-0.5f, -0.5f, 0.0f, 0.5f, -0.5f, 0.0f, 0.0f, 0.5f, 0.0f};

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
  window.EnableVsync(false);

  glfwSetFramebufferSizeCallback(
    window,
    [](GLFWwindow *window, int width, int height) {
      glViewport(0, 0, width, height);
    }
  );

  // Setup Dear ImGui context
  IMGUI_CHECKVERSION();
  ImGui::CreateContext();
  ImGuiIO &io = ImGui::GetIO();
  (void)io;
  io.ConfigFlags |=
    ImGuiConfigFlags_NavEnableKeyboard; // Enable Keyboard Controls
  io.ConfigFlags |=
    ImGuiConfigFlags_NavEnableGamepad; // Enable Gamepad Controls

  // Setup Dear ImGui style
  ImGui::StyleColorsDark();
  // ImGui::StyleColorsLight();

  // Setup Platform/Renderer backends
  ImGui_ImplGlfw_InitForOpenGL(window, true);
  ImGui_ImplOpenGL3_Init("#version 330 core");

  u32 VAO;
  glGenVertexArrays(1, &VAO);
  u32 VBO;
  glGenBuffers(1, &VBO);

  glBindVertexArray(VAO);
  glBindBuffer(GL_ARRAY_BUFFER, VBO);
  glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
  glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(f32), (void *)0);
  glEnableVertexAttribArray(0);
  glBindVertexArray(0);

  GLShaderProgram &shaderProgram = GLShaderProgram::DefaultPipeLine();

  /* Loop until the user closes the window */
  while (!glfwWindowShouldClose(window)) {
    ProcessInput(window);

    /* Poll for and process events */
    glfwPollEvents();

    ImGui_ImplOpenGL3_NewFrame();
    ImGui_ImplGlfw_NewFrame();
    ImGui::NewFrame();

    {
      ImGuiIO &io = ImGui::GetIO();

      ImGui::SetNextWindowPos(ImVec2(0, io.DisplaySize.y - 30));
      ImGui::SetNextWindowSize(ImVec2(io.DisplaySize.x, 20));

      ImGui::Begin(
        "Status Bar",
        nullptr,
        ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoResize |
          ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoScrollbar |
          ImGuiWindowFlags_NoSavedSettings
      );

      ImGui::Text("Status: Ready | FPS: %.2f", Time::GetFPS());
      ImGui::End();
    }

    ImGui::Render();

    /* Render here */
    glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);

    shaderProgram.Use();
    float greenValue = (sin(Time::TotalTime()) / 2.0f) + 0.5f;
    shaderProgram.SetUniform4f("ourColor", 0.0f, greenValue, 0.0f, 1.0f);
    glBindVertexArray(VAO);
    glDrawArrays(GL_TRIANGLES, 0, 3);

    ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

    /* Swap front and back buffers */
    glfwSwapBuffers(window);

    Time::Tick();
  }

  glDeleteVertexArrays(1, &VAO);
  glDeleteBuffers(1, &VBO);
  glDeleteProgram(shaderProgram);

  glfwTerminate();
  return 0;
}