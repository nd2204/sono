#include "imgui.h"
#include <core/global.h>
#include <core/common/time.h>
#include <core/common/logger.h>
#include <core/math/mat4.h>
#include <core/math/vec3.h>

#include <core/input/mouse.h>
#include <core/event/event_dispatcher.h>

#include <render/camera.h>

#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>

Camera cam;

MemorySystem *g_MemSys;
RenderSystem *g_RenderSys;
EventSystem *g_EventSys;
InputSystem *g_InputSys;
BufferManager *g_BufferMgr;
RenderWindow *g_Window;

void ProcessInput() {
  if (g_Window->GetKey(KeyCode::KEY_ESC) == GLFW_PRESS) {
    g_Window->SetCursorMode(CursorMode::NORMAL);
  }
  if (g_Window->GetKey(KeyCode::KEY_F) == GLFW_PRESS) {
    g_Window->SetCursorMode(CursorMode::DISABLED);
  }

  const float cameraSpeed = 10.0f * Time::DeltaTime(); // adjust accordingly
  if (g_Window->GetKey(KeyCode::KEY_W) == GLFW_PRESS) {
    cam.Move(cameraSpeed * cam.GetForward());
  }
  if (g_Window->GetKey(KeyCode::KEY_A) == GLFW_PRESS) {
    cam.Move(-cameraSpeed * cam.GetRight());
  }
  if (g_Window->GetKey(KeyCode::KEY_S) == GLFW_PRESS) {
    cam.Move(-cameraSpeed * cam.GetForward());
  }
  if (g_Window->GetKey(KeyCode::KEY_D) == GLFW_PRESS) {
    cam.Move(cameraSpeed * cam.GetRight());
  }
}

void HandleEvent(const Event &ev) {
  // S_LOG_TRACE(ev.ToString());

  switch (ev.type) {
    case EventType::KEY:
    case EventType::MOUSE_MOVE:
    case EventType::MOUSE_BUTTON:
    case EventType::MOUSE_SCROLL:
      g_InputSys->InjectEvent(ev);
      break;
    case EventType::WINDOW_RESIZE: {
      auto &k = std::get<WindowResizeEvent>(ev.payload);
      g_RenderSys->Submit<SetViewportCommand>(0, 0, k.width, k.height);
      break;
    }
    case EventType::TEXT:
    case EventType::QUIT:
      break;
  }
}

i32 main(void) {
  std::unique_ptr<Sono::Global> global = std::make_unique<Sono::Global>();
  Sono::Global::GetPtr()->Init();

  g_MemSys = MemorySystem::GetPtr();
  g_RenderSys = RenderSystem::GetPtr();
  g_EventSys = EventSystem::GetPtr();
  g_InputSys = InputSystem::GetPtr();
  g_BufferMgr = g_RenderSys->GetBufferManager();

  g_Window = g_RenderSys->CreateRenderWindow(800, 600, "Hello Sono");
  g_Window->EnableVsync(true);

  f32 vertices[] = {-0.5f, -0.5f, -0.5f, 0.0f, 0.0f, 0.5f,  -0.5f, -0.5f, 1.0f, 0.0f,
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

  IBuffer *vb = g_BufferMgr->CreateVertexBuffer(
    BufferUsage::STATIC, sizeof(vertices) / sizeof(f32), sizeof(f32), vertices
  );

  VertexLayout layout = {
    {VAS_POSITION, VAT_FLOAT3},
    {VAS_TEXCOORD, VAT_FLOAT2},
  };
  VertexArray *vao = g_RenderSys->CreateVertexArray();
  vao->AddVertexBuffer(vb, layout);
  // vao->SetIndexBuffer(ib);

  VertexLayout lightLayout = {
    {VAS_POSITION, VAT_FLOAT3},
  };
  VertexArray *lightVAO = g_RenderSys->CreateVertexArray();
  lightVAO->AddVertexBuffer(vb, lightLayout);
  // vao->SetIndexBuffer(ib);

  Texture *texture, *texture2;
  {
    PROFILE_SCOPE("Create Textures");
    unsigned char *data;
    int width, height, nrChannels;

    stbi_set_flip_vertically_on_load(true);
    data = stbi_load("assets/textures/container.jpg", &width, &height, &nrChannels, 0);
    texture =
      g_RenderSys->CreateTexture(TEX_TYPE_2D, TEX_FORMAT_RGB, TEX_FORMAT_RGB, width, height);
    texture->Update(data, 0);
    texture->GenerateMipmaps();
    stbi_image_free(data);

    data = stbi_load("assets/textures/awesomeface.png", &width, &height, &nrChannels, 0);
    texture2 =
      g_RenderSys->CreateTexture(TEX_TYPE_2D, TEX_FORMAT_RGBA, TEX_FORMAT_RGBA, width, height);
    texture2->Update(data, 0);
    texture2->GenerateMipmaps();
    stbi_image_free(data);
  }

  Shader *vs = g_RenderSys->CreateShader();
  Shader *fs = g_RenderSys->CreateShader();
  RenderPipeline *pipeline;
  {
    PROFILE_SCOPE("Compile Shaders");
    vs->CompileFromFile("./assets/shaders/vertex.glsl", ShaderStage::VERTEX);
    fs->CompileFromFile("./assets/shaders/fragment.glsl", ShaderStage::FRAGMENT);
    pipeline = g_RenderSys->CreatePipeline(vs, fs);
    g_RenderSys->BindPipeline(pipeline, 0);
    g_RenderSys->BindTexture(texture, 0);
    g_RenderSys->BindTexture(texture2, 1);
  }

  cam.SetPosition(0.0f, 0.0f, 3.0f);
  cam.LookAt(Vec3::Zero);
  cam.SetPerspective(Sono::Radians(80.0f), g_Window->GetAspect(), 0.1f, 100.0f);

  EventDispatcher::Register<MouseMoveEvent>([](const MouseMoveEvent &e) {
    ImGuiIO &io = ImGui::GetIO();
    io.MousePos = ImVec2(e.xpos, e.ypos);

    if (g_Window->GetCurrentCursorMode() != CursorMode::DISABLED) return;

    Vec2 mouseDelta = Mouse::GetDelta();
    float sensitivity = 0.05f;

    // Apply rotation deltas
    float yawDelta = mouseDelta.x * sensitivity;
    float pitchDelta = -mouseDelta.y * sensitivity;

    cam.Rotate(Vec3(pitchDelta, yawDelta, 0.0f));
  });

  g_RenderSys->InitImGui(g_Window);
  /* Loop until the user closes the window */
  {
    PROFILE_SCOPE("MAIN_LOOP");
    while (!g_Window->ShouldClose()) {
      /* Poll for and process events */
      {
        PROFILE_SCOPE("MAIN_LOOP::Handle Events");
        glfwPollEvents();
        while (auto *ev = g_EventSys->Pop()) {
          HandleEvent(*ev);
          EventDispatcher::Dispatch(*ev);
        }
        ProcessInput();
      }

      {
        /* Render here */
        PROFILE_SCOPE("MAIN_LOOP::RENDER::RenderOneFrame");
        g_RenderSys->BeginFrame(cam);
        g_RenderSys->Submit<ClearCommand>(Vec4(0.07f, 0.07f, 0.07f, 1.0f));

        Mat4 model;
        for (int i = 0; i < 10; i++) {
          model = Mat4::Translation(cubePositions[i]);
          f32 angle = 20.0f * i;
          model = Mat4::Rotation(Sono::Radians(angle), Vec3(1.0f, 0.3f, 0.5f)) * model;
          g_RenderSys->Submit<DrawCommand>(vao, 36, model, PrimitiveType::TRIANGLES);
          g_RenderSys->Flush();
        }

        g_RenderSys->BeginImGuiFrame();
        ImGui::Begin("My Window"); // Start a new window (panel)
        ImGui::Text("FPS: %.0f", Time::GetFPS());
        ImGui::End();
        g_RenderSys->EndImGuiFrame();

        g_RenderSys->EndFrame();
      }
      g_InputSys->EndFrame();

      Time::Tick();
    }
  }

  g_RenderSys->ShutdownImGui();
  Sono::Global::GetPtr()->Shutdown();
  return 0;
}
