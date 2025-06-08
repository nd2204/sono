#include "imgui.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"
#include "core/sngl/glshader.h"
#include "core/snscreen.h"
#include "core/common/time.h"

#include <cmath>
#include <glad/glad.h>
#include <GLFW/glfw3.h>

f32 vertices[] = {-0.5f, -0.5f, 0.0f, 0.5f, -0.5f, 0.0f, 0.0f, 0.5f, 0.0f};

void ProcessInput(Sono::SNScreen &ctx) {
  if (ctx.GetKey(GLFW_KEY_ESCAPE) == GLFW_PRESS)
    ctx.SetShouldClose(true);
}

i32 main(void) {
  /* Initialize the library */
  if (!glfwInit())
    exit(EXIT_FAILURE);

  glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
  glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
  glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
#ifdef MACOS_BUILD
  glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif

  /* Create a windowed mode window and its OpenGL context */
  Sono::SNScreen screen(800, 600, "Hello Sono");
  screen.EnableVsync(true);
  screen.SetFrameBufferSizeCallback(
    [](Sono::SNScreen &window, i32 width, i32 height) {
      glViewport(0, 0, width, height);
    }
  );

  // Setup Dear ImGui context
  IMGUI_CHECKVERSION();
  ImGui::CreateContext();
  ImGuiIO &io = ImGui::GetIO();
  (void)io;
  io.ConfigFlags = io.ConfigFlags
    | ImGuiConfigFlags_NavEnableKeyboard // Enable Keyboard Controls
    | ImGuiConfigFlags_NavEnableGamepad  // Enable Gamepad Controls
    ;

  // Setup Dear ImGui style
  ImGui::StyleColorsDark();
  // ImGui::StyleColorsLight();

  // Setup Platform/Renderer backends
  ImGui_ImplGlfw_InitForOpenGL(screen, true);
  ImGui_ImplOpenGL3_Init("#version 330 core");

  GLuint fbo, colorTex;
  glGenFramebuffers(1, &fbo);
  glBindFramebuffer(GL_FRAMEBUFFER, fbo);

  // Create texture
  glGenTextures(1, &colorTex);
  glBindTexture(GL_TEXTURE_2D, colorTex);
  glTexImage2D(
    GL_TEXTURE_2D,
    0,
    GL_RGBA8,
    screen.GetWidth(),
    screen.GetHeight(),
    0,
    GL_RGBA,
    GL_UNSIGNED_BYTE,
    nullptr
  );
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

  // Attach to FBO
  glFramebufferTexture2D(
    GL_FRAMEBUFFER,
    GL_COLOR_ATTACHMENT0,
    GL_TEXTURE_2D,
    colorTex,
    0
  );
  glBindFramebuffer(GL_FRAMEBUFFER, 0);

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
  while (!screen.ShouldClose()) {
    ProcessInput(screen);

    /* Poll for and process events */
    glfwPollEvents();

    ImGui_ImplOpenGL3_NewFrame();
    ImGui_ImplGlfw_NewFrame();
    ImGui::NewFrame();

    // Set up a fullscreen dockspace window with no decoration
    ImGuiWindowFlags window_flags =
      ImGuiWindowFlags_MenuBar | ImGuiWindowFlags_NoDocking;
    const ImGuiViewport *viewport = ImGui::GetMainViewport();

    ImGui::SetNextWindowPos(viewport->WorkPos);
    ImGui::SetNextWindowSize(viewport->WorkSize);
    ImGui::SetNextWindowViewport(viewport->ID);

    window_flags |= ImGuiWindowFlags_NoTitleBar
      | ImGuiWindowFlags_NoCollapse
      | ImGuiWindowFlags_NoResize
      | ImGuiWindowFlags_NoMove;
    window_flags |=
      ImGuiWindowFlags_NoBringToFrontOnFocus | ImGuiWindowFlags_NoNavFocus;

    ImGui::PushStyleVar(ImGuiStyleVar_WindowRounding, 0.0f);
    ImGui::PushStyleVar(ImGuiStyleVar_WindowBorderSize, 0.0f);
    ImGui::Begin("DockSpace Demo", nullptr, window_flags);
    ImGui::PopStyleVar(2);

    // Setup actual dockspace
    ImGuiID dockspace_id = ImGui::GetID("MyDockspace");
    ImGui::DockSpace(dockspace_id, ImVec2(0.0f, 0.0f));

    ImGui::End();

    glBindFramebuffer(GL_FRAMEBUFFER, fbo);

    /* Render here */
    glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);

    shaderProgram.Use();
    float greenValue = (sin(Sono::Time::TotalTime()) / 2.0f) + 0.5f;
    shaderProgram.SetVec4("ourColor", 0.0f, greenValue, 0.0f, 1.0f);
    glBindVertexArray(VAO);
    glDrawArrays(GL_TRIANGLES, 0, 3);

    glBindFramebuffer(GL_FRAMEBUFFER, 0);

    ImGui::Begin("Viewport");

    // Get available content region size for dynamic scaling
    ImVec2 size = ImGui::GetContentRegionAvail();

    // Show texture (OpenGL uses GLuint texture as ImTextureID)
    ImGui::Image(
      (ImTextureID)(uintptr_t)colorTex,
      size,
      ImVec2(0, 1),
      ImVec2(1, 0)
    );

    ImGui::End();

    // {
    //   ImGuiIO &io = ImGui::GetIO();

    //   ImGui::SetNextWindowPos(ImVec2(0, io.DisplaySize.y - 30));
    //   ImGui::SetNextWindowSize(ImVec2(io.DisplaySize.x, 20));

    //   ImGui::Begin(
    //     "Status Bar",
    //     nullptr,
    //     ImGuiWindowFlags_NoTitleBar
    //       | ImGuiWindowFlags_NoResize
    //       | ImGuiWindowFlags_NoMove
    //       | ImGuiWindowFlags_NoScrollbar
    //       | ImGuiWindowFlags_NoSavedSettings
    //   );
    //   ImGui::Text("Status: Ready | FPS: %.2f", Sono::Time::GetFPS());
    //   ImGui::End();
    // }

    ImGui::Render();
    ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

    /* Swap front and back buffers */
    screen.SwapBuffers();

    Sono::Time::Tick();
  }

  glDeleteVertexArrays(1, &VAO);
  glDeleteBuffers(1, &VBO);
  glDeleteProgram(shaderProgram);

  glfwTerminate();
  return 0;
}