#include "imgui.h"
#include "render/render_command.h"
#include "render/shader/shader.h"
#include <core/common/logger.h>
#include <core/common/time.h>
#include <core/global.h>
#include <core/math/lerp.h>
#include <core/math/mat4.h>
#include <core/math/transform.h>
#include <core/math/vec3.h>

#include <core/event/event_dispatcher.h>
#include <core/input/mouse.h>
#include <core/resource/image.h>

#include <render/camera.h>
#include <render/vertex_type.h>
#include <unordered_map>

#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>

Camera cam;

MemorySystem *g_MemSys;
RenderSystem *g_RenderSys;
EventSystem *g_EventSys;
InputSystem *g_InputSys;
BufferManager *g_BufferMgr;
RenderWindow *g_Window;

void DrawTransformGizmo(const Transform &transform) {}

u32 GetImGuiId(const std::string &key) {
  static std::unordered_map<std::string, u32> s_AutoIdMap;
  static u32 s_AutoId = 0;

  auto idItor = s_AutoIdMap.find(key);
  if (idItor == s_AutoIdMap.end()) {
    return (s_AutoIdMap[key] = s_AutoId++);
  } else {
    return idItor->second;
  }
}

void ImGui_DrawTransformComponent(
  Transform &transform, const std::string &name = "Unamed transform"
) {
  Vec3 position = transform.GetPosition();
  Vec3 rotation = transform.GetEulerRotation();
  Vec3 scale = transform.GetScale();

  ImGui::Text("%s", name.c_str());
  ImGui::PushID(GetImGuiId(name));
  if (ImGui::DragFloat3("position", position.ValuePtr(), 0.25f)) {
    transform.SetPosition(position);
  }
  if (ImGui::DragFloat3("rotation", rotation.ValuePtr(), 0.5f)) {
    transform.SetEulerRotation(rotation);
  }
  if (ImGui::DragFloat3("scale", scale.ValuePtr(), 0.05f)) {
    transform.SetScale(scale);
  }
  ImGui::PopID();
}

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

std::vector<VertexP> CreateWordGridMesh(i32 gridSize, f32 spacing) {
  std::vector<VertexP> mesh;
  for (int i = -gridSize; i <= gridSize; i++) {
    mesh.emplace_back(i * spacing, 0.0f, -gridSize * spacing);
    mesh.emplace_back(i * spacing, 0.0f, +gridSize * spacing);
    mesh.emplace_back(-gridSize * spacing, 0.0f, i * spacing);
    mesh.emplace_back(+gridSize * spacing, 0.0f, i * spacing);
  }
  return mesh;
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

  std::vector<VertexP> gridVertices = CreateWordGridMesh(20, 1.0f);
  std::vector<VertexP> axesVertices = {
    {0.0f, 0.0f, 0.0f},
    {1.0f, 0.0f, 0.0f},
    {0.0f, 0.0f, 0.0f},
    {0.0f, 1.0f, 0.0f},
    {0.0f, 0.0f, 0.0f},
    {0.0f, 0.0f, 1.0f}
  };
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

  // Debugging buffers
  IBuffer *gridVb = g_BufferMgr->CreateVertexBuffer(
    BufferUsage::STATIC, sizeof(VertexP), gridVertices.size(), gridVertices.data()
  );
  IBuffer *axesVb = g_BufferMgr->CreateVertexBuffer(
    BufferUsage::STATIC, sizeof(VertexP), axesVertices.size(), axesVertices.data()
  );

  VertexArray *cubeVAO = g_RenderSys->CreateVertexArray();
  cubeVAO->AddVertexBuffer(vb, VertexTraits<VertexPN>::GetLayout());
  cubeVAO->SetIndexBuffer(ib);
  VertexArray *lightVAO = g_RenderSys->CreateVertexArray();
  lightVAO->AddVertexBuffer(vb, VertexTraits<VertexPN>::GetLayout());
  lightVAO->SetIndexBuffer(ib);
  VertexArray *gridVAO = g_RenderSys->CreateVertexArray();
  gridVAO->AddVertexBuffer(gridVb, VertexTraits<VertexP>::GetLayout());
  VertexArray *axesVAO = g_RenderSys->CreateVertexArray();
  axesVAO->AddVertexBuffer(axesVb, VertexTraits<VertexP>::GetLayout());

  // Texture *texture, *texture2;
  // {
  //   PROFILE_SCOPE("Create Textures");
  //   unsigned char *data;
  //   int width, height, nrChannels;
  //
  //   stbi_set_flip_vertically_on_load(true); data =
  //   stbi_load("assets/textures/container.jpg", &width, &height, &nrChannels,
  //   0); texture =
  //     g_RenderSys->CreateTexture(TEX_TYPE_2D, TEX_FORMAT_RGB, TEX_FORMAT_RGB,
  //     width, height);
  //   texture->Update(data, 0);
  //   texture->GenerateMipmaps();
  //   stbi_image_free(data);
  //
  //   data = stbi_load("assets/textures/awesomeface.png", &width, &height,
  //   &nrChannels, 0); texture2 =
  //     g_RenderSys->CreateTexture(TEX_TYPE_2D, TEX_FORMAT_RGBA,
  //     TEX_FORMAT_RGBA, width, height);
  //   texture2->Update(data, 0);
  //   texture2->GenerateMipmaps();
  //   stbi_image_free(data);
  // }

  RenderPipeline *cubePipeline;
  RenderPipeline *lightCubePipeline;
  RenderPipeline *gridPipeline;
  RenderPipeline *axesPipeline;
  {
    // clang-format off
    PROFILE_SCOPE("CreateRenderPipelines");
    cubePipeline = g_RenderSys->CreatePipeline({
      .vertex = g_RenderSys->CreateShader({
        .src = LoadShaderSrcFromFile("./assets/shaders/colors.vs.glsl"),
        .stage = ShaderStage::VERTEX
      }),
      .fragment = g_RenderSys->CreateShader({
        .src = LoadShaderSrcFromFile("./assets/shaders/colors.fs.glsl"),
        .stage = ShaderStage::FRAGMENT
      })
    });

    lightCubePipeline = g_RenderSys->CreatePipeline({
      .vertex = g_RenderSys->CreateShader({
        .src = LoadShaderSrcFromFile("./assets/shaders/light_cube.vs.glsl"),
        .stage = ShaderStage::VERTEX
      }),
      .fragment = g_RenderSys->CreateShader({
        .src = LoadShaderSrcFromFile("./assets/shaders/light_cube.fs.glsl"),
        .stage = ShaderStage::FRAGMENT
      })
    });

    gridPipeline = g_RenderSys->CreatePipeline({
      .vertex = g_RenderSys->CreateShader({
        .src = LoadShaderSrcFromFile("./assets/shaders/default.vs.glsl"),
        .stage = ShaderStage::VERTEX
      }),
      .fragment = g_RenderSys->CreateShader({
        .src = LoadShaderSrcFromFile("./assets/shaders/grid.fs.glsl"),
        .stage = ShaderStage::FRAGMENT
      })
    });

    axesPipeline = g_RenderSys->CreatePipeline({
      .vertex = g_RenderSys->CreateShader({
        .src = LoadShaderSrcFromFile("./assets/shaders/default.vs.glsl"),
        .stage = ShaderStage::VERTEX
      }),
      .fragment = g_RenderSys->CreateShader({
        .src = LoadShaderSrcFromFile("./assets/shaders/axes.fs.glsl"),
        .stage = ShaderStage::FRAGMENT
      })
    });
    // clang-format on
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

  Color3 cubeColor(255, 128, 79);
  Color3 lightColor(255, 255, 255);

  Transform cubeTransform;
  cubeTransform.SetPosition(Vec3::Zero);

  Transform lightCubeTransform = cubeTransform;
  lightCubeTransform.Move({-1.2f, 1.0f, -2.0f});
  lightCubeTransform.SetScale(Vec3(0.5f));

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

      // lightCubeTransform.Move(Vec3::Right * Time::DeltaTime() * 1.25f);
      lightCubeTransform.LookAt(cubeTransform);

      // cam.SetPosition(lightPos + lightCube.GetForward());
      // cam.LookAt(Vec3::Zero);
      /* Render here */
      {
        PROFILE_SCOPE("MAIN_LOOP::RENDER::RenderOneFrame");

        g_RenderSys->BeginFrame(cam);
        g_RenderSys->Submit<ClearCommand>(Color3(20, 20, 20));

        g_RenderSys->Submit<BindShaderCommand>(axesPipeline);
        g_RenderSys->Submit<SetUniformCommand<Mat4>>("uProj", cam.GetProjectionMatrix());
        g_RenderSys->Submit<SetUniformCommand<Mat4>>("uView", cam.GetViewMatrix());
        g_RenderSys->Submit<SetUniformCommand<Color3>>("uAxisColor", Color3(255, 0, 0));
        g_RenderSys->Submit<DrawCommand>(
          axesVAO, 0, 2, PrimitiveType::LINES, cubeTransform.GetModelMatrix()
        );
        g_RenderSys->Submit<SetUniformCommand<Color3>>("uAxisColor", Color3(0, 255, 0));
        g_RenderSys->Submit<DrawCommand>(
          axesVAO, 2, 2, PrimitiveType::LINES, cubeTransform.GetModelMatrix()
        );
        g_RenderSys->Submit<SetUniformCommand<Color3>>("uAxisColor", Color3(0, 0, 255));
        g_RenderSys->Submit<DrawCommand>(
          axesVAO, 4, 2, PrimitiveType::LINES, cubeTransform.GetModelMatrix()
        );

        g_RenderSys->Submit<SetUniformCommand<Color3>>("uAxisColor", Color3(255, 0, 0));
        g_RenderSys->Submit<DrawCommand>(
          axesVAO, 0, 2, PrimitiveType::LINES, lightCubeTransform.GetModelMatrix()
        );
        g_RenderSys->Submit<SetUniformCommand<Color3>>("uAxisColor", Color3(0, 255, 0));
        g_RenderSys->Submit<DrawCommand>(
          axesVAO, 2, 2, PrimitiveType::LINES, lightCubeTransform.GetModelMatrix()
        );
        g_RenderSys->Submit<SetUniformCommand<Color3>>("uAxisColor", Color3(0, 0, 255));
        g_RenderSys->Submit<DrawCommand>(
          axesVAO, 4, 2, PrimitiveType::LINES, lightCubeTransform.GetModelMatrix()
        );

        g_RenderSys->Submit<BindShaderCommand>(gridPipeline);
        g_RenderSys->Submit<SetUniformCommand<Mat4>>("uProj", cam.GetProjectionMatrix());
        g_RenderSys->Submit<SetUniformCommand<Mat4>>("uView", cam.GetViewMatrix());
        g_RenderSys->Submit<DrawCommand>(gridVAO, 0, gridVertices.size(), PrimitiveType::LINES);

        Vec3 cubeDiffuse = cubeColor * Vec3(0.6f);
        Vec3 cubeAmbient = cubeDiffuse * Vec3(0.4f);

        g_RenderSys->Submit<BindShaderCommand>(cubePipeline);
        g_RenderSys->Submit<SetUniformCommand<Vec3>>("uMaterial.ambient", cubeAmbient);
        g_RenderSys->Submit<SetUniformCommand<Vec3>>("uMaterial.diffuse", cubeDiffuse);
        g_RenderSys->Submit<SetUniformCommand<Vec3>>("uMaterial.specular", Vec3(0.5f));
        g_RenderSys->Submit<SetUniformCommand<f32>>("uMaterial.shininess", 32.0f);

        // lightColor.x = sin(glfwGetTime() * 2.0f);
        // lightColor.y = sin(glfwGetTime() * 0.7f);
        // lightColor.z = sin(glfwGetTime() * 1.3f);
        Vec3 lightDiffuse = lightColor * Vec3(0.6f);
        Vec3 lightAmbient = lightDiffuse * Vec3(0.4f);
        g_RenderSys->Submit<SetUniformCommand<Vec3>>(
          "uLight.position", lightCubeTransform.GetPosition()
        );
        g_RenderSys->Submit<SetUniformCommand<Vec3>>("uLight.ambient", lightAmbient);
        g_RenderSys->Submit<SetUniformCommand<Vec3>>("uLight.diffuse", lightDiffuse);
        g_RenderSys->Submit<SetUniformCommand<Vec3>>("uLight.specular", lightColor);
        g_RenderSys->Submit<SetUniformCommand<Mat4>>("uProj", cam.GetProjectionMatrix());
        g_RenderSys->Submit<SetUniformCommand<Mat4>>("uView", cam.GetViewMatrix());
        // g_RenderSys->Submit<DrawCommand>(cubeVao, 36);
        g_RenderSys->Submit<DrawIndexedCommand>(
          cubeVAO, 0, cubeIndices.size(), cubeTransform.GetModelMatrix()
        );

        g_RenderSys->Submit<BindShaderCommand>(lightCubePipeline);
        g_RenderSys->Submit<SetUniformCommand<Color3>>("uLightColor", lightColor);
        g_RenderSys->Submit<SetUniformCommand<Mat4>>("uProj", cam.GetProjectionMatrix());
        g_RenderSys->Submit<SetUniformCommand<Mat4>>("uView", cam.GetViewMatrix());
        // g_RenderSys->Submit<DrawCommand>(lightVAO, 36, lightCubeModel);
        g_RenderSys->Submit<DrawIndexedCommand>(
          lightVAO, 0, cubeIndices.size(), lightCubeTransform.GetModelMatrix()
        );
        g_RenderSys->Flush();

        g_RenderSys->BeginImGuiFrame();
        if (ImGui::Begin("Debug")) {
          ImGui::Text("FPS: %.0f", Time::GetFPS());
          ImGui::Text("Frame Data");
          const ArenaAllocator &frameAlloc = g_RenderSys->GetFrameAllocator();
          float progress = ((f32)frameAlloc.GetMarker() / (f32)frameAlloc.GetSize());
          char buf[32];
          std::string frameAllocOffsetStr = MemorySystem::ToHumanReadable(frameAlloc.GetMarker());
          std::string frameAllocSizeStr = MemorySystem::ToHumanReadable(frameAlloc.GetSize());
          snprintf(
            buf, frameAllocSizeStr.size() + frameAllocOffsetStr.size() + 2, "%s/%s",
            frameAllocOffsetStr.c_str(), frameAllocSizeStr.c_str()
          );
          ImGui::ProgressBar(progress, ImVec2(0.0f, 0.0f), buf);
          ImGui::SameLine(0.0f, ImGui::GetStyle().ItemInnerSpacing.x);
          ImGui::Text("Arena usage (%.2f)", progress * 100);
          ImGui::Spacing();

          // ImGui::ShowStyleEditor();
          ImGui_DrawTransformComponent(cubeTransform, "Cube");
          ImGui::ColorEdit3("Cube Color", cubeColor.ValuePtr());
          ImGui::Spacing();

          ImGui_DrawTransformComponent(lightCubeTransform, "Light cube");
          ImGui::ColorEdit3("Light Color", lightColor.ValuePtr());
          ImGui::Spacing();

          ImGui::Text("Camera");
          ImGui::PushID(GetImGuiId("Camera"));
          Vec3 newPosition = cam.GetPosition();
          if (ImGui::DragFloat3("position", newPosition.ValuePtr(), 0.5f)) {
            cam.SetPosition(newPosition);
          }
          ImGui::PopID();
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
