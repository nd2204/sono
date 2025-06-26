#include <core/global.h>
#include <core/memory/memory_system.h>
#include <core/common/time.h>
#include <render/sngl/sngl.h>
#include <render/render_system.h>
#include <core/common/logger.h>

#include <GLFW/glfw3.h>
#include <glad/glad.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <sstream>

#include "core/math/mat4.h"
#include "core/math/vec3.h"
#include "glm/fwd.hpp"
#include "glm/trigonometric.hpp"

#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>

void ProcessInput(Window &ctx) {
  if (ctx.GetKey(GLFW_KEY_ESCAPE) == GLFW_PRESS) ctx.SetShouldClose(true);
}

i32 main(void) {
  std::unique_ptr<Sono::Global> global = std::make_unique<Sono::Global>();
  Sono::Global::GetPtr()->Init();

  Mat4 mat = Mat4::Rotation(Radians(90.0f), Vec3(0.0f, 1.0f, 1.0f));
  Mat4 m = Mat4::Identity;
  S_LOG_DEBUG((m * mat).ToString());

  glm::mat4 model = glm::mat4(1.0f);
  glm::mat4 rotate = glm::rotate(model, glm::radians(90.0f), glm::vec3(0.0f, 1.0f, 1.0f));
  std::stringstream ss;
  for (int i = 0; i < 4; i++) {
    ss << "(" << rotate[i][0];
    for (int j = 1; j < 4; j++) {
      ss << "," << rotate[i][j];
    }
    ss << ")";
  }
  S_LOG_DEBUG(ss.str());

  Window window(800, 600, "Hello Sono");
  window.EnableVsync(false);
  window.SetFrameBufferSizeCallback([](Window &window, i32 width, i32 height) {
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
  while (!window.ShouldClose()) {
    ProcessInput(window);

    /* Poll for and process events */
    glfwPollEvents();

    /* Render here */
    glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);

    texture.Bind(0);
    texture2.Bind(1);
    shaderProgram.Use();
    if (window.GetKey(GLFW_KEY_UP) == GLFW_PRESS) {
      opacity += Time::DeltaTime();
    } else if (window.GetKey(GLFW_KEY_DOWN) == GLFW_PRESS) {
      opacity -= Time::DeltaTime();
    }
    shaderProgram.SetFloat("uOpacity", opacity);

    // glm::mat4 trans = glm::identity<glm::mat4>();
    // trans = glm::translate(trans, glm::vec3(0.5f, -0.5f, 0.0f));
    // trans = glm::rotate(trans, (float)glfwGetTime(), glm::vec3(0.0, 0.0, 1.0f));
    // shaderProgram.SetMat4("uTransform", glm::value_ptr(trans));
    Mat4 trans = Mat4::Rotation((float)glfwGetTime(), Vec3(0.0, 0.0, 1.0f))
      * Mat4::Translation(Vec3(0.5f, 0.5f, 0.0f));
    shaderProgram.SetMat4("uTransform", trans.ValuePtr());
    Vao.Draw();
    // Vao.Draw(GL_LINE_LOOP);

    /* Swap front and back buffers */
    window.SwapBuffers();

    Time::Tick();
  }

  S_LOG(MemorySystem::GetPtr()->GetAllocsReport());
  S_LOG(MemorySystem::GetPtr()->GetLeaksReport());
  Sono::Global::GetPtr()->Shutdown();
  return 0;
}
