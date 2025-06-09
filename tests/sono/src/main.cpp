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

#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>

SonoGlobal global;

void ProcessInput(SNScreen &ctx) {
  if (ctx.GetKey(GLFW_KEY_ESCAPE) == GLFW_PRESS)
    ctx.SetShouldClose(true);
}

i32 main(void) {
  void *mem = SN_ALLOC(100, SN_ALLOCATION_TYPE_DEFAULT);
  MemorySystem::Get().PrintMemoryReport();
  SN_FREE(mem);
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
  SNScreen screen(800, 600, "Hello Sono");
  screen.EnableVsync(true);
  screen.SetFrameBufferSizeCallback(
    [](SNScreen &window, i32 width, i32 height) {
      glViewport(0, 0, width, height);
    }
  );

  float vertices[] = {
    // positions          // colors           // texture coords
    0.5f,  0.5f,  0.0f, 1.0f, 0.0f, 0.0f, 1.0f, 1.0f, // top right
    0.5f,  -0.5f, 0.0f, 0.0f, 1.0f, 0.0f, 1.0f, 0.0f, // bottom right
    -0.5f, -0.5f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f, // bottom left
    -0.5f, 0.5f,  0.0f, 1.0f, 1.0f, 0.0f, 0.0f, 1.0f  // top left
  };

  u32 indices[] = {
    // note that we start from 0!
    0,
    1,
    3, // first triangle
    1,
    2,
    3 // second triangle
  };

  GLVertexBuffer VBO(vertices, sizeof(vertices));
  GLElementBuffer EBO(indices, sizeof(indices));
  GLVertexLayout layout;
  layout.Push({3, GL_FLOAT, sizeof(f32), GL_FALSE}); // 3 floats per vertex
  layout.Push({3, GL_FLOAT, sizeof(f32), GL_FALSE}); // 3 floats per vertex
  layout.Push({2, GL_FLOAT, sizeof(f32), GL_FALSE}); // 2 floats per vertex

  GLVertexArray VAO;
  VAO.SetVertexBuffer(VBO, layout);
  VAO.SetELementBuffer(EBO);

  int width, height, nrChannels;
  stbi_set_flip_vertically_on_load(true);
  unsigned char *data =
    stbi_load("assets/textures/container.jpg", &width, &height, &nrChannels, 0);
  GLTexture texture(GL_TEXTURE_2D, GL_RGB, width, height);
  texture.SetData(data, GL_RGB, GL_UNSIGNED_BYTE);
  texture.GenerateMipmap();
  stbi_image_free(data);

  data = stbi_load(
    "assets/textures/awesomeface.png",
    &width,
    &height,
    &nrChannels,
    0
  );

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
    VAO.Draw();
    // glDrawArrays(GL_LINE_STRIP, 0, 3);

    /* Swap front and back buffers */
    screen.SwapBuffers();

    Time::Tick();
  }

  glfwTerminate();
  return 0;
}