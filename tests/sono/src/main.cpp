#include "imgui.h"
#include <core/global.h>
#include <core/common/time.h>
#include <core/common/logger.h>
#include <core/math/mat4.h>
#include <core/math/vec3.h>
#include <core/math/lerp.h>

#include <core/input/mouse.h>
#include <core/event/event_dispatcher.h>

#include <render/camera.h>
#include <render/vertex_type.h>

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
      g_RenderSys->SetViewport(0, 0, k.width, k.height);
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

  g_Window = g_RenderSys->CreateRenderWindow(1024, 768, "Hello Sono");
  g_Window->EnableVsync(true);

  std::vector<VertexPN> cubeVertices = {
    // -z face
    VertexPN({-0.5f, +0.5f, -0.5f}, {+0.0f, +0.0f, -1.0f}),
    VertexPN({+0.5f, +0.5f, -0.5f}, {+0.0f, +0.0f, -1.0f}),
    VertexPN({+0.5f, -0.5f, -0.5f}, {+0.0f, +0.0f, -1.0f}),
    VertexPN({-0.5f, -0.5f, -0.5f}, {+0.0f, +0.0f, -1.0f}),
    // +z face
    VertexPN({-0.5f, +0.5f, +0.5f}, {+0.0f, +0.0f, +1.0f}),
    VertexPN({+0.5f, +0.5f, +0.5f}, {+0.0f, +0.0f, +1.0f}),
    VertexPN({+0.5f, -0.5f, +0.5f}, {+0.0f, +0.0f, +1.0f}),
    VertexPN({-0.5f, -0.5f, +0.5f}, {+0.0f, +0.0f, +1.0f}),
    // -x face
    VertexPN({-0.5f, +0.5f, +0.5f}, {-1.0f, +0.0f, +0.0f}),
    VertexPN({-0.5f, +0.5f, -0.5f}, {-1.0f, +0.0f, +0.0f}),
    VertexPN({-0.5f, -0.5f, -0.5f}, {-1.0f, +0.0f, +0.0f}),
    VertexPN({-0.5f, -0.5f, +0.5f}, {-1.0f, +0.0f, +0.0f}),
    // +x face
    VertexPN({+0.5f, +0.5f, +0.5f}, {+1.0f, +0.0f, +0.0f}),
    VertexPN({+0.5f, +0.5f, -0.5f}, {+1.0f, +0.0f, +0.0f}),
    VertexPN({+0.5f, -0.5f, -0.5f}, {+1.0f, +0.0f, +0.0f}),
    VertexPN({+0.5f, -0.5f, +0.5f}, {+1.0f, +0.0f, +0.0f}),
    // -y face
    VertexPN({-0.5f, -0.5f, -0.5f}, {+0.0f, -1.0f, +0.0f}),
    VertexPN({+0.5f, -0.5f, -0.5f}, {+0.0f, -1.0f, +0.0f}),
    VertexPN({+0.5f, -0.5f, +0.5f}, {+0.0f, -1.0f, +0.0f}),
    VertexPN({-0.5f, -0.5f, +0.5f}, {+0.0f, -1.0f, +0.0f}),
    // +y face
    VertexPN({-0.5f, +0.5f, -0.5f}, {+0.0f, +1.0f, +0.0f}),
    VertexPN({+0.5f, +0.5f, -0.5f}, {+0.0f, +1.0f, +0.0f}),
    VertexPN({+0.5f, +0.5f, +0.5f}, {+0.0f, +1.0f, +0.0f}),
    VertexPN({-0.5f, +0.5f, +0.5f}, {+0.0f, +1.0f, +0.0f}),
  };

  std::vector<u16> cubeIndices = {
    0,  1,  2,  2,  3,  0,  4,  5,  6,  6,  7,  4,  8,  9,  10, 10, 11, 8,
    12, 13, 14, 14, 15, 12, 16, 17, 18, 18, 19, 16, 20, 21, 22, 22, 23, 20,
  };

  IBuffer *vb = g_BufferMgr->CreateVertexBuffer(
    BufferUsage::STATIC, sizeof(VertexPN), cubeVertices.size(), cubeVertices.data()
  );

  IBuffer *ib = g_BufferMgr->CreateIndexBuffer(
    BufferUsage::STATIC, INDEX_TYPE_U16, cubeIndices.size(), cubeIndices.data()
  );

  VertexLayout layout = VertexTraits<VertexPN>::GetLayout();

  VertexArray *cubeVAO = g_RenderSys->CreateVertexArray();
  cubeVAO->AddVertexBuffer(vb, layout);
  cubeVAO->SetIndexBuffer(ib);

  VertexArray *lightVAO = g_RenderSys->CreateVertexArray();
  lightVAO->AddVertexBuffer(vb, layout);
  lightVAO->SetIndexBuffer(ib);

  // Texture *texture, *texture2;
  // {
  //   PROFILE_SCOPE("Create Textures");
  //   unsigned char *data;
  //   int width, height, nrChannels;
  //
  //   stbi_set_flip_vertically_on_load(true);
  //   data = stbi_load("assets/textures/container.jpg", &width, &height, &nrChannels, 0);
  //   texture =
  //     g_RenderSys->CreateTexture(TEX_TYPE_2D, TEX_FORMAT_RGB, TEX_FORMAT_RGB, width, height);
  //   texture->Update(data, 0);
  //   texture->GenerateMipmaps();
  //   stbi_image_free(data);
  //
  //   data = stbi_load("assets/textures/awesomeface.png", &width, &height, &nrChannels, 0);
  //   texture2 =
  //     g_RenderSys->CreateTexture(TEX_TYPE_2D, TEX_FORMAT_RGBA, TEX_FORMAT_RGBA, width, height);
  //   texture2->Update(data, 0);
  //   texture2->GenerateMipmaps();
  //   stbi_image_free(data);
  // }

  RenderPipeline *cubePipeline;
  RenderPipeline *lightCubePipeline;
  {
    PROFILE_SCOPE("Compile Shaders");
    Shader *vs = g_RenderSys->CreateShader();
    Shader *fs = g_RenderSys->CreateShader();
    vs->CompileFromFile("./assets/shaders/1.colors.vs.glsl", ShaderStage::VERTEX);
    fs->CompileFromFile("./assets/shaders/1.colors.fs.glsl", ShaderStage::FRAGMENT);
    cubePipeline = g_RenderSys->CreatePipeline(vs, fs);

    Shader *vs2 = g_RenderSys->CreateShader();
    Shader *fs2 = g_RenderSys->CreateShader();
    vs2->CompileFromFile("./assets/shaders/1.light_cube.vs.glsl", ShaderStage::VERTEX);
    fs2->CompileFromFile("./assets/shaders/1.light_cube.fs.glsl", ShaderStage::FRAGMENT);
    lightCubePipeline = g_RenderSys->CreatePipeline(vs2, fs2);
  }

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

  /// ================================================================================
  /// Render States
  /// ================================================================================
  cam.SetPerspective(Sono::Radians(80.0f), g_Window->GetAspect(), 0.1f, 100.0f);

  Vec3 lightPos(1.2f, 1.0f, -2.0f);
  Color3 cubeColor(255, 128, 79);
  Color3 lightColor(255, 255, 255);
  lightPos = Vec3(-1.2, 1.0f, -2.0f);

  Camera lightCube; // NOTE: I didn't have transform class so i used camera instead :)
  lightCube.SetPosition(lightPos);

  /// ================================================================================
  /// Main Loop
  /// ================================================================================
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

      /* Render here */
      {
        PROFILE_SCOPE("MAIN_LOOP::RENDER::RenderOneFrame");

        g_RenderSys->BeginFrame(cam);
        g_RenderSys->Submit<ClearCommand>(Vec4(0.07f, 0.07f, 0.07f, 1.0f));

        lightCube.Move(lightCube.GetRight() * Time::DeltaTime() * 5.0f);
        lightCube.LookAt(Vec3::Zero);
        lightPos = lightCube.GetPosition();
        // cam.SetPosition(lightPos + lightCube.GetForward());
        // cam.LookAt(Vec3::Zero);

        g_RenderSys->Submit<BindShaderCommand>(cubePipeline);
        g_RenderSys->Submit<SetUniformCommand<Vec3>>("uLightPos", lightPos);
        g_RenderSys->Submit<SetUniformCommand<Vec3>>("uViewPos", cam.GetPosition());
        g_RenderSys->Submit<SetUniformCommand<Color3>>("uObjectColor", cubeColor);
        g_RenderSys->Submit<SetUniformCommand<Color3>>("uLightColor", lightColor);
        g_RenderSys->Submit<SetUniformCommand<Mat4>>("uProj", cam.GetProjectionMatrix());
        g_RenderSys->Submit<SetUniformCommand<Mat4>>("uView", cam.GetViewMatrix());
        // g_RenderSys->Submit<DrawCommand>(cubeVao, 36);
        g_RenderSys->Submit<DrawIndexedCommand>(cubeVAO, cubeIndices.size());

        Mat4 lightCubeModel = Mat4::Scale(Vec3(0.5f)) * Mat4::Translation(lightPos);
        g_RenderSys->Submit<BindShaderCommand>(lightCubePipeline);
        g_RenderSys->Submit<SetUniformCommand<Color3>>("uLightColor", lightColor);
        g_RenderSys->Submit<SetUniformCommand<Mat4>>("uProj", cam.GetProjectionMatrix());
        g_RenderSys->Submit<SetUniformCommand<Mat4>>("uView", cam.GetViewMatrix());
        // g_RenderSys->Submit<DrawCommand>(lightVAO, 36, lightCubeModel);
        g_RenderSys->Submit<DrawIndexedCommand>(lightVAO, cubeIndices.size(), lightCubeModel);
        g_RenderSys->Flush();

        g_RenderSys->BeginImGuiFrame();
        if (ImGui::Begin("Debug")) {
          ImGui::Text("FPS: %.0f", Time::GetFPS());
          ImGui::Text("Frame Data");
          const ArenaAllocator &frameAlloc = g_RenderSys->GetFrameAllocator();
          float progress = (f32)frameAlloc.GetMarker() / frameAlloc.GetSize() * 100;
          char buf[32];
          sprintf(
            buf, "%s/%s", MemorySystem::ToHumanReadable(frameAlloc.GetMarker()).c_str(),
            MemorySystem::ToHumanReadable(frameAlloc.GetSize()).c_str()
          );
          ImGui::ProgressBar(progress, ImVec2(0.0f, 0.0f), buf);
          ImGui::SameLine(0.0f, ImGui::GetStyle().ItemInnerSpacing.x);
          ImGui::Text("Arena usage");
          ImGui::Spacing();

          // ImGui::ShowStyleEditor();
          ImGui::Text("Cube");
          ImGui::ColorEdit3("Cube Color", cubeColor.ValuePtr());
          ImGui::Text("Light Cube");
          ImGui::PushID(0);
          ImGui::DragFloat3("position", lightPos.ValuePtr(), 0.5f);
          ImGui::PopID();
          ImGui::ColorEdit3("Light Color", lightColor.ValuePtr());
          ImGui::Spacing();

          ImGui::Text("Camera");
          Vec3 newPosition = cam.GetPosition();
          ImGui::PushID(1);
          ImGui::DragFloat3("position", newPosition.ValuePtr(), 0.5f);
          ImGui::PopID();
          cam.SetPosition(newPosition);
          ImGui::Spacing();
        }
        ImGui::End();
        g_RenderSys->EndImGuiFrame();

        g_RenderSys->EndFrame();
      }
      g_InputSys->EndFrame();

      Time::Tick();
    }
  }

  /// ================================================================================
  /// Cleanup
  /// ================================================================================

  g_RenderSys->ShutdownImGui();
  Sono::Global::GetPtr()->Shutdown();
  return 0;
}
