#include <core/global.h>
#include <core/memory/memory_system.h>
#include <core/common/time.h>
#include <core/common/logger.h>
#include <core/math/mat4.h>
#include <core/math/vec3.h>

#include <render/render_system.h>
#include <render/sngl/gl_shader.h>

#include <GLFW/glfw3.h>
#include <glad/glad.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>

void ProcessInput(RenderWindow &ctx) {
  if (ctx.GetKey(GLFW_KEY_ESCAPE) == GLFW_PRESS) ctx.SetShouldClose(true);
}

i32 main(void) {
  std::unique_ptr<Sono::Global> global = std::make_unique<Sono::Global>();
  Sono::Global::GetPtr()->Init();

  RenderSystem *renderSys = RenderSystem::GetPtr();
  BufferManager *bufferMgr = renderSys->GetBufferManager();

  RenderWindow *window = renderSys->CreateRenderWindow(800, 600, "Hello Sono");
  window->EnableVsync(false);
  window->SetFrameBufferSizeCallback([](RenderWindow &window, i32 width, i32 height) {
    glViewport(0, 0, width, height);
  });

  f32 vertices[] = {
    // positions          // colors           // texture coords
    0.5f,  0.5f,  0.0f, 1.0f, 0.0f, 0.0f, 1.0f, 1.0f, // top right
    0.5f,  -0.5f, 0.0f, 0.0f, 1.0f, 0.0f, 1.0f, 0.0f, // bottom right
    -0.5f, -0.5f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f, // bottom left
    -0.5f, 0.5f,  0.0f, 1.0f, 1.0f, 0.0f, 0.0f, 1.0f  // top left
  };

  u32 indices[] = {0, 1, 3, 1, 2, 3};

  LOG_DEBUG("Creating buffers");
  IBuffer *vb =
    bufferMgr->CreateVertexBuffer(BufferUsage::STATIC, sizeof(vertices) / sizeof(f32), sizeof(f32));
  vb->Update(vertices, sizeof(vertices));

  IBuffer *ib = bufferMgr->CreateIndexBuffer(
    BufferUsage::STATIC, INDEX_TYPE_U32, sizeof(indices) / sizeof(u32)
  );
  ib->Update(indices, sizeof(indices));

  LOG_DEBUG("Creating VertexLayout");
  VertexLayout layout;
  layout.Push(VAS_POSITION, VAT_FLOAT3); // 3 floats per vertex
  layout.Push(VAS_COLOR, VAT_FLOAT3);    // 3 floats per vertex
  layout.Push(VAS_TEXCOORD, VAT_FLOAT2); // 2 floats per vertex

  LOG_DEBUG("Creating VertexArray");
  VertexArray *vao = renderSys->CreateVertexArray();
  vao->AddVertexBuffer(vb);
  vao->SetIndexBuffer(ib);
  vao->SetVertexLayout(&layout);

  LOG_DEBUG("Binding VertexArray");
  renderSys->BindVertexArray(vao);

  LOG_DEBUG("Creating Textures");
  int width, height, nrChannels;
  stbi_set_flip_vertically_on_load(true);
  unsigned char *data = stbi_load("assets/textures/container.jpg", &width, &height, &nrChannels, 0);
  Texture *texture =
    renderSys->CreateTexture(TEX_TYPE_2D, TEX_FORMAT_RGB, TEX_FORMAT_RGB, width, height);
  texture->Update(data, 0);
  texture->GenerateMipmaps();
  stbi_image_free(data);

  data = stbi_load("assets/textures/awesomeface.png", &width, &height, &nrChannels, 0);
  Texture *texture2 =
    renderSys->CreateTexture(TEX_TYPE_2D, TEX_FORMAT_RGBA, TEX_FORMAT_RGBA, width, height);
  texture2->Update(data, 0);
  texture2->GenerateMipmaps();
  stbi_image_free(data);

  LOG_DEBUG("Creating Shader");
  Shader *vs = renderSys->CreateShader(nullptr);
  vs->CompileFromFile("./assets/shaders/vertex.glsl", ShaderStage::VERTEX);
  Shader *fs = renderSys->CreateShader(nullptr);
  fs->CompileFromFile("./assets/shaders/fragment.glsl", ShaderStage::FRAGMENT);

  GLRenderPipeline *pipeline =
    reinterpret_cast<GLRenderPipeline *>(renderSys->CreatePipeline(vs, fs));
  renderSys->BindPipeline(pipeline, 0);
  renderSys->BindTexture(texture, 0);
  renderSys->BindTexture(texture2, 1);

  f32 opacity = 0.2f;
  /* Loop until the user closes the window */
  while (!window->ShouldClose()) {
    ProcessInput(*window);

    /* Poll for and process events */
    glfwPollEvents();

    if (window->GetKey(GLFW_KEY_UP) == GLFW_PRESS) {
      opacity += Time::DeltaTime();
    } else if (window->GetKey(GLFW_KEY_DOWN) == GLFW_PRESS) {
      opacity -= Time::DeltaTime();
    }

    /* Render here */
    renderSys->BeginFrame();

    pipeline->SetFloat("uOpacity", opacity);
    Mat4 trans = Mat4::Rotation((float)glfwGetTime(), Vec3(0.0, 0.0, 1.0f))
      * Mat4::Translation(Vec3(0.5f, 0.5f, 0.0f));
    pipeline->SetMat4("uTransform", trans.ValuePtr());

    renderSys->Submit<ClearCommand>(Vec4(0.2f, 0.3f, 0.3f, 1.0f));
    renderSys->Submit<DrawIndexedCommand>(vao, 6, texture, PrimitiveType::TRIANGLES);

    renderSys->Flush();
    renderSys->EndFrame();

    /* Swap front and back buffers */
    renderSys->Present();

    Time::Tick();
  }

  Sono::Global::GetPtr()->Shutdown();
  return 0;
}
