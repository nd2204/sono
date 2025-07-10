#include <algorithm>
#include <core/global.h>
#include <core/common/time.h>
#include <core/common/logger.h>
#include <core/math/mat4.h>
#include <core/math/vec3.h>

#include <core/common/sys_event_queue.h>
#include "GLFW/glfw3.h"
#include "core/input/mouse.h"

#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>

Camera cam;

void ProcessInput() {
  static RenderSystem *renderSys = RenderSystem::GetPtr();
  static RenderWindow *win = static_cast<RenderWindow *>(renderSys->GetCurrentContext());

  if (win->GetKey(KeyCode::KEY_ESC) == GLFW_PRESS) win->SetShouldClose(true);

  const float cameraSpeed = 10.0f * Time::DeltaTime(); // adjust accordingly
  if (win->GetKey(KeyCode::KEY_W) == GLFW_PRESS) {
    cam.Move(cameraSpeed * cam.GetForward());
  }
  if (win->GetKey(KeyCode::KEY_A) == GLFW_PRESS) {
    cam.Move(-cameraSpeed * cam.GetRight());
  }
  if (win->GetKey(KeyCode::KEY_S) == GLFW_PRESS) {
    cam.Move(-cameraSpeed * cam.GetForward());
  }
  if (win->GetKey(KeyCode::KEY_D) == GLFW_PRESS) {
    cam.Move(cameraSpeed * cam.GetRight());
  }

  Vec3 currentEuler = cam.GetEulerAngles();

  Vec2 mouseDelta = Mouse::GetDelta();
  float sensitivity = 0.1f;

  // Apply rotation deltas
  float yawDelta = mouseDelta.x * sensitivity;
  float pitchDelta = -mouseDelta.y * sensitivity;

  // Apply clamped pitch
  float newPitch = std::clamp(currentEuler.y + pitchDelta, -89.9f, 89.9f);
  float newYaw = currentEuler.x + yawDelta;

  cam.SetEulerAngles(Vec3(newPitch, newYaw, 0.0f));
}

void HandleEvent(const Event &ev) {
  static InputSystem *inputSys = InputSystem::GetPtr();
  // S_LOG_TRACE(ev.ToString());

  switch (ev.type) {
    case EventType::KEY:
    case EventType::MOUSE_MOVE:
    case EventType::MOUSE_BUTTON:
    case EventType::MOUSE_SCROLL:
      inputSys->InjectEvent(ev);
      break;
    case EventType::TEXT:
    case EventType::QUIT:
    case EventType::WINDOW_RESIZE:
      break;
  }
}

i32 main(void) {
  std::unique_ptr<Sono::Global> global = std::make_unique<Sono::Global>();
  Sono::Global::GetPtr()->Init();

  RenderSystem *renderSys = RenderSystem::GetPtr();
  InputSystem *inputSys = InputSystem::GetPtr();
  BufferManager *bufferMgr = renderSys->GetBufferManager();
  SystemEventQueue *eventQueue = SystemEventQueue::GetPtr();

  RenderWindow *window = renderSys->CreateRenderWindow(800, 600, "Hello Sono");
  window->EnableVsync(true);

  float vertices[] = {-0.5f, -0.5f, -0.5f, 0.0f, 0.0f, 0.5f,  -0.5f, -0.5f, 1.0f, 0.0f,
                      0.5f,  0.5f,  -0.5f, 1.0f, 1.0f, 0.5f,  0.5f,  -0.5f, 1.0f, 1.0f,
                      -0.5f, 0.5f,  -0.5f, 0.0f, 1.0f, -0.5f, -0.5f, -0.5f, 0.0f, 0.0f,

                      -0.5f, -0.5f, 0.5f,  0.0f, 0.0f, 0.5f,  -0.5f, 0.5f,  1.0f, 0.0f,
                      0.5f,  0.5f,  0.5f,  1.0f, 1.0f, 0.5f,  0.5f,  0.5f,  1.0f, 1.0f,
                      -0.5f, 0.5f,  0.5f,  0.0f, 1.0f, -0.5f, -0.5f, 0.5f,  0.0f, 0.0f,

                      -0.5f, 0.5f,  0.5f,  1.0f, 0.0f, -0.5f, 0.5f,  -0.5f, 1.0f, 1.0f,
                      -0.5f, -0.5f, -0.5f, 0.0f, 1.0f, -0.5f, -0.5f, -0.5f, 0.0f, 1.0f,
                      -0.5f, -0.5f, 0.5f,  0.0f, 0.0f, -0.5f, 0.5f,  0.5f,  1.0f, 0.0f,

                      0.5f,  0.5f,  0.5f,  1.0f, 0.0f, 0.5f,  0.5f,  -0.5f, 1.0f, 1.0f,
                      0.5f,  -0.5f, -0.5f, 0.0f, 1.0f, 0.5f,  -0.5f, -0.5f, 0.0f, 1.0f,
                      0.5f,  -0.5f, 0.5f,  0.0f, 0.0f, 0.5f,  0.5f,  0.5f,  1.0f, 0.0f,

                      -0.5f, -0.5f, -0.5f, 0.0f, 1.0f, 0.5f,  -0.5f, -0.5f, 1.0f, 1.0f,
                      0.5f,  -0.5f, 0.5f,  1.0f, 0.0f, 0.5f,  -0.5f, 0.5f,  1.0f, 0.0f,
                      -0.5f, -0.5f, 0.5f,  0.0f, 0.0f, -0.5f, -0.5f, -0.5f, 0.0f, 1.0f,

                      -0.5f, 0.5f,  -0.5f, 0.0f, 1.0f, 0.5f,  0.5f,  -0.5f, 1.0f, 1.0f,
                      0.5f,  0.5f,  0.5f,  1.0f, 0.0f, 0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
                      -0.5f, 0.5f,  0.5f,  0.0f, 0.0f, -0.5f, 0.5f,  -0.5f, 0.0f, 1.0f};

  Vec3 cubePositions[] = {Vec3(0.0f, 0.0f, 0.0f),    Vec3(2.0f, 5.0f, -15.0f),
                          Vec3(-1.5f, -2.2f, -2.5f), Vec3(-3.8f, -2.0f, -12.3f),
                          Vec3(2.4f, -0.4f, -3.5f),  Vec3(-1.7f, 3.0f, -7.5f),
                          Vec3(1.3f, -2.0f, -2.5f),  Vec3(1.5f, 2.0f, -2.5f),
                          Vec3(1.5f, 0.2f, -1.5f),   Vec3(-1.3f, 1.0f, -1.5f)};

  u32 indices[] = {0, 1, 3, 1, 2, 3};

  IBuffer *vb;
  {
    PROFILE_SCOPE("Create Buffers");
    vb = bufferMgr->CreateVertexBuffer(
      BufferUsage::STATIC, sizeof(vertices) / sizeof(f32), sizeof(f32)
    );
    vb->Update(vertices, sizeof(vertices));
  }

  // IBuffer *ib = bufferMgr->CreateIndexBuffer(
  //   BufferUsage::STATIC, INDEX_TYPE_U32, sizeof(indices) / sizeof(u32)
  // );
  // ib->Update(indices, sizeof(indices));

  // glm::mat4 v =
  //   glm::lookAt(glm::vec3(camX, 0.0, camZ), glm::vec3(0.0, 0.0, 0.0), glm::vec3(0.0, 1.0, 0.0));
  // for (int i = 0; i < 4; i++) {
  //   std::stringstream ss;
  //   for (int j = 0; j < 4; j++) {
  //     ss << std::to_string(v[i][j]) << " ";
  //   }
  //   S_LOG_DEBUG(ss.str());
  // }

  VertexLayout layout;
  layout.Push(VAS_POSITION, VAT_FLOAT3); // 3 floats per vertex
  layout.Push(VAS_TEXCOORD, VAT_FLOAT2); // 2 floats per vertex

  VertexArray *vao;
  {
    PROFILE_SCOPE("Create VertexArray");
    vao = renderSys->CreateVertexArray();
    vao->AddVertexBuffer(vb);
    // vao->SetIndexBuffer(ib);
    vao->SetVertexLayout(&layout);
  }

  LOG_DEBUG("Binding VertexArray");
  renderSys->BindVertexArray(vao);

  Texture *texture, *texture2;
  {
    PROFILE_SCOPE("Create Textures");
    unsigned char *data;
    int width, height, nrChannels;

    stbi_set_flip_vertically_on_load(true);
    data = stbi_load("assets/textures/container.jpg", &width, &height, &nrChannels, 0);
    texture = renderSys->CreateTexture(TEX_TYPE_2D, TEX_FORMAT_RGB, TEX_FORMAT_RGB, width, height);
    texture->Update(data, 0);
    texture->GenerateMipmaps();
    stbi_image_free(data);

    data = stbi_load("assets/textures/awesomeface.png", &width, &height, &nrChannels, 0);
    texture2 =
      renderSys->CreateTexture(TEX_TYPE_2D, TEX_FORMAT_RGBA, TEX_FORMAT_RGBA, width, height);
    texture2->Update(data, 0);
    texture2->GenerateMipmaps();
    stbi_image_free(data);
  }

  Shader *vs = renderSys->CreateShader(nullptr);
  Shader *fs = renderSys->CreateShader(nullptr);
  RenderPipeline *pipeline;
  {
    PROFILE_SCOPE("Compile Shaders");
    vs->CompileFromFile("./assets/shaders/vertex.glsl", ShaderStage::VERTEX);
    fs->CompileFromFile("./assets/shaders/fragment.glsl", ShaderStage::FRAGMENT);
    pipeline = renderSys->CreatePipeline(vs, fs);
    renderSys->BindPipeline(pipeline, 0);
    renderSys->BindTexture(texture, 0);
    renderSys->BindTexture(texture2, 1);
  }

  cam.SetPosition(0.0f, 0.0f, 3.0f);
  cam.LookAt(Vec3::Zero);
  cam.SetPerspective(Sono::Radians(60.0f), window->GetAspect(), 0.1f, 100.0f);

  /* Loop until the user closes the window */
  while (!window->ShouldClose()) {
    /* Poll for and process events */
    glfwPollEvents();
    while (auto *ev = eventQueue->Pop()) {
      HandleEvent(*ev);
    }
    ProcessInput();

    /* Render here */
    renderSys->BeginFrame();
    renderSys->Submit<ClearCommand>(Vec4(255, 255, 255, 255));

    pipeline->SetMat4("uView", cam.GetViewMatrix());
    pipeline->SetMat4("uProj", cam.GetProjectionMatrix());

    Mat4 model;
    for (int i = 0; i < 10; i++) {
      model = Mat4::Translation(cubePositions[i]);
      f32 angle = 20.0f * i;
      model = Mat4::Rotation(Sono::Radians(angle), Vec3(1.0f, 0.3f, 0.5f)) * model;
      pipeline->SetMat4("uModel", model);
      renderSys->Submit<DrawCommand>(vao, 36, texture, PrimitiveType::TRIANGLES);
      renderSys->Flush();
    }

    renderSys->Flush();
    renderSys->EndFrame();
    inputSys->EndFrame();

    /* Swap front and back buffers */
    renderSys->Present();

    Time::Tick();
  }

  Sono::Global::GetPtr()->Shutdown();
  return 0;
}
