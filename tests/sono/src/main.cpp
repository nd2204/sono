#include <core/memory/memory_system.h>
#include <core/common/time.h>
#include <render/sngl/sngl.h>
#include <core/snscreen.h>
#include <core/common/logger.h>

#include <GLFW/glfw3.h>
#include <cstdlib>
#include <glad/glad.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <core/global.h>

#include "core/math/vec3.h"
#include "core/math/mat4.h"

#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>

SonoGlobal global;

void ProcessInput(SNScreen &ctx) {
  if (ctx.GetKey(GLFW_KEY_ESCAPE) == GLFW_PRESS) ctx.SetShouldClose(true);
}

i32 main(void) {
  /* Initialize the library */
  if (!glfwInit()) exit(EXIT_FAILURE);

  glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
  glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
  glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
#ifdef MACOS_BUILD
  glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif

  /* Create a windowed mode window and its OpenGL context */
  SNScreen screen(800, 600, "Hello Sono");
  screen.EnableVsync(true);
  screen.SetFrameBufferSizeCallback([](SNScreen &window, i32 width, i32 height) {
    glViewport(0, 0, width, height);
  });

  float vertices[] = {
    // positions          // colors           // texture coords
    0.5f,  0.5f,  0.0f, 1.0f, 0.0f, 0.0f, 1.0f, 1.0f, // top right
    0.5f,  -0.5f, 0.0f, 0.0f, 1.0f, 0.0f, 1.0f, 0.0f, // bottom right
    -0.5f, -0.5f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f, // bottom left
    -0.5f, 0.5f,  0.0f, 1.0f, 1.0f, 0.0f, 0.0f, 1.0f  // top left
  };

  u32 indices[] = {0, 1, 3, 1, 2, 3};

  GLVertexBuffer Vbo(vertices, sizeof(vertices));
  GLElementBuffer Ebo(indices, sizeof(indices));
  GLVertexLayout layout;
  layout.Push({3, GL_FLOAT, sizeof(f32), GL_FALSE}); // 3 floats per vertex
  layout.Push({3, GL_FLOAT, sizeof(f32), GL_FALSE}); // 3 floats per vertex
  layout.Push({2, GL_FLOAT, sizeof(f32), GL_FALSE}); // 2 floats per vertex

  GLVertexArray Vao;
  Vao.SetVertexBuffer(Vbo, layout);
  Vao.SetELementBuffer(Ebo);

  int width, height, nrChannels;
  stbi_set_flip_vertically_on_load(true);
  unsigned char *data = stbi_load("assets/textures/container.jpg", &width, &height, &nrChannels, 0);
  GLTexture texture(GL_TEXTURE_2D, GL_RGB, width, height);
  texture.SetData(data, GL_RGB, GL_UNSIGNED_BYTE);
  texture.GenerateMipmap();
  stbi_image_free(data);

  data = stbi_load("assets/textures/awesomeface.png", &width, &height, &nrChannels, 0);

  GLTexture texture2(GL_TEXTURE_2D, GL_RGBA, width, height);
  texture2.SetData(data, GL_RGBA, GL_UNSIGNED_BYTE);
  texture2.GenerateMipmap();
  stbi_image_free(data);

  GLShaderProgram &shaderProgram = GLShaderProgram::DefaultPipeLine();
  shaderProgram.Use();
  shaderProgram.SetInt("texture1", 0);
  shaderProgram.SetInt("texture2", 1);

  f32 opacity = 0.2f;
  /* Loop until the user closes the window */
  while (!screen.ShouldClose()) {
    ProcessInput(screen);

    /* Poll for and process events */
    glfwPollEvents();

    /* Render here */
    glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);

    texture.Bind(0);
    texture2.Bind(1);
    shaderProgram.Use();
    if (screen.GetKey(GLFW_KEY_UP) == GLFW_PRESS) {
      opacity += Time::DeltaTime();
    } else if (screen.GetKey(GLFW_KEY_DOWN) == GLFW_PRESS) {
      opacity -= Time::DeltaTime();
    }
    shaderProgram.SetFloat("uOpacity", opacity);

    glm::mat4 trans = glm::identity<glm::mat4>();
    trans = glm::translate(trans, glm::vec3(0.5f, -0.5f, 0.0f));
    trans = glm::rotate(trans, (float)glfwGetTime(), glm::vec3(0.0, 0.0, 1.0f));
    shaderProgram.SetMat4("uTransform", glm::value_ptr(trans));
    Vao.Draw();
    // glDrawArrays(GL_LINE_STRIP, 0, 3);
    glm::mat4 trans2 = glm::identity<glm::mat4>();
    trans2 = glm::translate(trans2, glm::vec3(-0.5f, 0.5f, 0.0f));
    trans2 = glm::scale(trans2, glm::vec3(glm::vec2(std::sin(Time::TotalTime())), 0.0f));
    shaderProgram.SetMat4("uTransform", glm::value_ptr(trans2));
    Vao.Draw();

    /* Swap front and back buffers */
    screen.SwapBuffers();

    Time::Tick();
  }

  glfwTerminate();
  return 0;
}
