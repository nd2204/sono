#include "render/resource/texture.h"
#include <core/math/math.h>
#include <core/common/logger.h>
#include <core/common/time.h>
#include <core/global.h>
#include <core/math/mat4.h>
#include <core/math/transform.h>
#include <core/math/vec3.h>
#include <core/event/event_dispatcher.h>
#include <core/input/mouse.h>
#include <render/render_command.h>
#include <render/shader/shader.h>
#include <render/camera.h>
#include <render/vertex_type.h>
#include <render/resource/mesh.h>
#include <unordered_map>
#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>
#include <imgui.h>
#include <core/entry.h>

class SonoEditor : public Sono::App {
public:
  Camera cam;
  RenderSystem *g_RenderSys;
  InputSystem *g_InputSys;

  Mesh cubeMesh;
  Mesh gridMesh;
  std::vector<VertexP> axesVertices;

  Buffer *axesVb;
  VertexArray *axesVAO;

  RenderPipeline *cubePipeline;
  RenderPipeline *lightCubePipeline;
  RenderPipeline *gridPipeline;
  RenderPipeline *axesPipeline;

  Color3 cubeColor;
  Color3 lightColor;
  Transform cubeTransform;
  Transform cubeTransforms[40];
  Transform lightCubeTransform;

  void Init() override {
    g_RenderSys = RenderSystem::GetPtr();
    g_InputSys = InputSystem::GetPtr();
    m_ActiveWindow->EnableVsync(0);

    RenderDevice *device = g_RenderSys->GetRenderDevice();

    gridMesh = MeshFactory::CreateWordGridMesh(device, 20, 1.0f);
    cubeMesh = MeshFactory::CreateCubeMesh(device);

    axesVertices = {
      {0.0f, 0.0f, 0.0f},
      {1.0f, 0.0f, 0.0f},
      {0.0f, 0.0f, 0.0f},
      {0.0f, 1.0f, 0.0f},
      {0.0f, 0.0f, 0.0f},
      {0.0f, 0.0f, 1.0f}
    };
    axesVb = device->CreateBuffer(
      {.count = axesVertices.size(),
       .stride = sizeof(VertexP),
       .usage = BufferUsage::Vertex,
       .data = axesVertices.data()}
    );
    axesVAO = device->CreateVertexArray();
    axesVAO->AddVertexBuffer(axesVb, VertexTraits<VertexP>::GetLayout());

    Texture *containerTex, *containerSpecTex;
    {
      PROFILE_SCOPE("Create Textures");
      unsigned char *data;
      int width, height, nrChannels;

      stbi_set_flip_vertically_on_load(true);
      data = stbi_load("assets/textures/container2.png", &width, &height, &nrChannels, 0);
      containerTex = device->CreateTexture({
        .size = {(u32)width, (u32)height},
        .format = TextureFormat::rgba8_unorm,
        .usage = TextureUsage::TextureBinding | TextureUsage::CopyDest
      });
      containerTex->Update(data, 0);
      containerTex->GenerateMipmaps();
      stbi_image_free(data);

      // TODO: delegate texture Update to device command queue
      data =
        stbi_load("assets/textures/container2_specular_map.png", &width, &height, &nrChannels, 0);
      containerSpecTex = device->CreateTexture({
        .size = {(u32)width, (u32)height},
        .format = TextureFormat::rgba8_unorm,
        .usage = TextureUsage::TextureBinding | TextureUsage::CopyDest
      });
      containerSpecTex->Update(data, 0);
      containerSpecTex->GenerateMipmaps();
      stbi_image_free(data);

      g_RenderSys->BindTexture(containerTex, 0);
      g_RenderSys->BindTexture(containerSpecTex, 1);
    }

    {
      // clang-format off
      PROFILE_SCOPE("CreateRenderPipelines");
      axesPipeline = device->CreatePipeline({
        .label = "axesPipeline",
        .vertex = device->CreateShader({
          .src = LoadShaderSrcFromFile("./assets/shaders/default.vs.glsl"),
          .stage = ShaderStage::VERTEX
        }),
        .fragment = device->CreateShader({
          .src = LoadShaderSrcFromFile("./assets/shaders/axes.fs.glsl"),
          .stage = ShaderStage::FRAGMENT
        })
      });

      gridPipeline = device->CreatePipeline({
        .label = "gridPipeline",
        .vertex = device->CreateShader({
          .src = LoadShaderSrcFromFile("./assets/shaders/default.vs.glsl"),
          .stage = ShaderStage::VERTEX
        }),
        .fragment = device->CreateShader({
          .src = LoadShaderSrcFromFile("./assets/shaders/grid.fs.glsl"),
          .stage = ShaderStage::FRAGMENT
        })
      });


      cubePipeline = device->CreatePipeline({
        .label = "cubePipeline",
        .vertex = device->CreateShader({
          .src = LoadShaderSrcFromFile("./assets/shaders/colors.vs.glsl"),
          .stage = ShaderStage::VERTEX
        }),
        .fragment = device->CreateShader({
          .src = LoadShaderSrcFromFile("./assets/shaders/colors.fs.glsl"),
          .stage = ShaderStage::FRAGMENT
        })
      });

      lightCubePipeline = device->CreatePipeline({
        .label = "lightCubePipeline",
        .vertex = device->CreateShader({
          .src = LoadShaderSrcFromFile("./assets/shaders/light_cube.vs.glsl"),
          .stage = ShaderStage::VERTEX
        }),
        .fragment = device->CreateShader({
          .src = LoadShaderSrcFromFile("./assets/shaders/light_cube.fs.glsl"),
          .stage = ShaderStage::FRAGMENT
        })
      });
      // clang-format on
    }

    EventDispatcher::Register<MouseMoveEvent>([this](const MouseMoveEvent &e) {
      ImGuiIO &io = ImGui::GetIO();
      io.MousePos = ImVec2(e.xpos, e.ypos);

      if (m_ActiveWindow->GetCurrentCursorMode() != CursorMode::DISABLED) return;

      Vec2 mouseDelta = Mouse::GetDelta();
      float sensitivity = 0.04f;

      // Apply rotation deltas
      Vec3 eulerDelta = Vec3(-mouseDelta.y, mouseDelta.x, 0.0f) * sensitivity;
      cam.Rotate(eulerDelta);
    });

    cam.SetPerspective(Sono::Radians(80.0f), m_ActiveWindow->GetAspect(), 0.1f, 100.0f);

    cubeColor = Color3(255, 128, 79);
    lightColor = Color3(255, 255, 255);
    cubeTransform.SetPosition({0.0f, 0.5f, 0.0f});

    f32 spacing = 1.5f;
    for (int i = 0; i < 20; ++i) {
      cubeTransforms[i] = cubeTransform;
      f32 xPos = cos(i * (Sono::PI / 10.0f)) * spacing;
      f32 yPos = 0;
      f32 zPos = sin(i * (Sono::PI / 10.0f)) * spacing;
      cubeTransforms[i].RotateAxis(Vec3(1.0f, 0.7f, 1.0f), 0);
      cubeTransforms[i].Scale(0.25f);
      cubeTransforms[i].Move({xPos, yPos, zPos});
    }

    for (int i = 20; i < 40; ++i) {
      cubeTransforms[i] = cubeTransform;
      f32 xPos = cos(i * (Sono::PI / 10.0f)) * spacing * 1.5f;
      f32 yPos = 0;
      f32 zPos = sin(i * (Sono::PI / 10.0f)) * spacing * 1.5f;
      cubeTransforms[i].RotateAxis(Vec3(1.0f, -0.7f, 1.0f), 0);
      cubeTransforms[i].Scale(0.25f);
      cubeTransforms[i].Move({xPos, yPos, zPos});
    }

    lightCubeTransform = cubeTransform;
    lightCubeTransform.Move({-1.2f, 1.0f, 2.0f});
    lightCubeTransform.Scale(Vec3(0.5f));
  }

  void Update() override {
    ProcessInput();

    lightCubeTransform.Move(Vec3::Right * Time::DeltaTime() * 5.00f);
    lightCubeTransform.LookAt(cubeTransform);
    if (lightCubeTransform.IsDirty()) {
      lightCubeTransform.UpdateModelMatrix();
    }
    if (cubeTransform.IsDirty()) {
      cubeTransform.UpdateModelMatrix();
    }
    for (int i = 0; i < 40; ++i) {
      if (cubeTransforms[i].IsDirty()) {
        cubeTransforms[i].UpdateModelMatrix();
      }
    }

    // cam.SetPosition(lightPos + lightCube.GetForward());
    // cam.LookAt(Vec3::Zero);
    /* Render here */
    PROFILE_SCOPE("MAIN_LOOP::RENDER::RenderOneFrame");

    g_RenderSys->BeginFrame(cam);

    g_RenderSys->Submit<ClearCommand>(Color3(20, 20, 20));
    // clang-format off
    g_RenderSys->Submit<BindShaderCommand>(axesPipeline);
    {
      g_RenderSys->Submit<SetUniformCommand<Mat4>>("uProj", cam.GetProjectionMatrix());
      g_RenderSys->Submit<SetUniformCommand<Mat4>>("uView", cam.GetViewMatrix());
      g_RenderSys->Submit<SetUniformCommand<Color3>>("uAxisColor", Color3(255, 0, 0));
      g_RenderSys->Submit<DrawCommand>(axesVAO, 0, 2, PrimitiveType::LINES, cubeTransform.GetModelMatrix());
      g_RenderSys->Submit<SetUniformCommand<Color3>>("uAxisColor", Color3(0, 255, 0));
      g_RenderSys->Submit<DrawCommand>(axesVAO, 2, 2, PrimitiveType::LINES, cubeTransform.GetModelMatrix());
      g_RenderSys->Submit<SetUniformCommand<Color3>>("uAxisColor", Color3(0, 0, 255));
      g_RenderSys->Submit<DrawCommand>(axesVAO, 4, 2, PrimitiveType::LINES, cubeTransform.GetModelMatrix());
      g_RenderSys->Submit<SetUniformCommand<Color3>>("uAxisColor", Color3(255, 0, 0));
      g_RenderSys->Submit<DrawCommand>(axesVAO, 0, 2, PrimitiveType::LINES, lightCubeTransform.GetModelMatrix());
      g_RenderSys->Submit<SetUniformCommand<Color3>>("uAxisColor", Color3(0, 255, 0));
      g_RenderSys->Submit<DrawCommand>(axesVAO, 2, 2, PrimitiveType::LINES, lightCubeTransform.GetModelMatrix());
      g_RenderSys->Submit<SetUniformCommand<Color3>>("uAxisColor", Color3(0, 0, 255));
      g_RenderSys->Submit<DrawCommand>(axesVAO, 4, 2, PrimitiveType::LINES, lightCubeTransform.GetModelMatrix());
    }

    // Draw world grid
    g_RenderSys->Submit<BindShaderCommand>(gridPipeline);
    {
      g_RenderSys->Submit<SetUniformCommand<Mat4>>("uProj", cam.GetProjectionMatrix());
      g_RenderSys->Submit<SetUniformCommand<Mat4>>("uView", cam.GetViewMatrix());
      // Draw non axis line
      g_RenderSys->Submit<SetUniformCommand<Color3>>("uLineColor", Color3(40, 40, 40));
      g_RenderSys->Submit<DrawCommand>(gridMesh.pVertexArray, 0, gridMesh.pVertexBuffer->GetCount() - 6, PrimitiveType::LINES);
      // Draw axis line
      g_RenderSys->Submit<SetUniformCommand<Color3>>("uLineColor", Color3(255, 0, 0));
      g_RenderSys->Submit<DrawCommand>(gridMesh.pVertexArray, gridMesh.pVertexBuffer->GetCount() - 6, 2, PrimitiveType::LINES);
      g_RenderSys->Submit<SetUniformCommand<Color3>>("uLineColor", Color3(0, 0, 255));
      g_RenderSys->Submit<DrawCommand>(gridMesh.pVertexArray, gridMesh.pVertexBuffer->GetCount() - 2, 2, PrimitiveType::LINES);
    }

    // Draw cubes
    g_RenderSys->Submit<BindShaderCommand>(cubePipeline);
    {
      g_RenderSys->Submit<SetUniformCommand<i32>>("uMaterial.diffuse", 0);
      g_RenderSys->Submit<SetUniformCommand<i32>>("uMaterial.specular", 1);
      g_RenderSys->Submit<SetUniformCommand<f32>>("uMaterial.shininess", 32.0f);
      g_RenderSys->Submit<SetUniformCommand<f32>>("uTime", Time::Now());
      Vec3 lightDiffuse = lightColor * Vec3(0.6f);
      Vec3 lightAmbient = lightDiffuse * Vec3(0.4f);
      g_RenderSys->Submit<SetUniformCommand<Vec3>>("uLight.position", lightCubeTransform.GetPosition());
      g_RenderSys->Submit<SetUniformCommand<Vec3>>("uLight.ambient", lightAmbient);
      g_RenderSys->Submit<SetUniformCommand<Vec3>>("uLight.diffuse", lightDiffuse);
      g_RenderSys->Submit<SetUniformCommand<Vec3>>("uLight.specular", lightColor);
      g_RenderSys->Submit<SetUniformCommand<Mat4>>("uProj", cam.GetProjectionMatrix());
      g_RenderSys->Submit<SetUniformCommand<Mat4>>("uView", cam.GetViewMatrix());
      g_RenderSys->Submit<DrawIndexedCommand>(
        cubeMesh.pVertexArray,
        0,
        cubeMesh.pIndexBuffer->GetCount(),
        cubeTransform.GetModelMatrix()
      );
      for (int i = 0; i < 40; ++i) {
        g_RenderSys->Submit<DrawIndexedCommand>(
          cubeMesh.pVertexArray,
          0,
          cubeMesh.pIndexBuffer->GetCount(),
          cubeTransforms[i].GetModelMatrix()
        );
      }
    }

    g_RenderSys->Submit<BindShaderCommand>(lightCubePipeline);
    {
      g_RenderSys->Submit<SetUniformCommand<Color3>>("uLightColor", lightColor);
      g_RenderSys->Submit<SetUniformCommand<Mat4>>("uProj", cam.GetProjectionMatrix());
      g_RenderSys->Submit<SetUniformCommand<Mat4>>("uView", cam.GetViewMatrix());
      // g_RenderSys->Submit<DrawCommand>(lightVAO, 36, lightCubeModel);
      g_RenderSys->Submit<DrawIndexedCommand>(
        cubeMesh.pVertexArray, 0, cubeMesh.pIndexBuffer->GetCount(),
        lightCubeTransform.GetModelMatrix()
      );
    }

    g_RenderSys->Flush();
    // clang-format on
  }

  void OnImGuiFrame() override {
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
  }

private:
  void ProcessInput() {
    if (!m_ActiveWindow) return;

    if (m_ActiveWindow->GetKey(KeyCode::KEY_ESC) == GLFW_PRESS) {
      m_ActiveWindow->SetCursorMode(CursorMode::NORMAL);
    }
    if (m_ActiveWindow->GetKey(KeyCode::KEY_F) == GLFW_PRESS) {
      m_ActiveWindow->SetCursorMode(CursorMode::DISABLED);
    }

    const float cameraSpeed = 10.0f * Time::DeltaTime(); // adjust accordingly
    if (m_ActiveWindow->GetKey(KeyCode::KEY_W) == GLFW_PRESS) {
      cam.Move(cameraSpeed * cam.GetForward());
    }
    if (m_ActiveWindow->GetKey(KeyCode::KEY_A) == GLFW_PRESS) {
      cam.Move(-cameraSpeed * cam.GetRight());
    }
    if (m_ActiveWindow->GetKey(KeyCode::KEY_S) == GLFW_PRESS) {
      cam.Move(-cameraSpeed * cam.GetForward());
    }
    if (m_ActiveWindow->GetKey(KeyCode::KEY_D) == GLFW_PRESS) {
      cam.Move(cameraSpeed * cam.GetRight());
    }
  }

  void ImGui_DrawTransformComponent(
    Transform &transform, const std::string &name = "Unamed transform"
  ) {
    Vec3 position = transform.GetPosition();
    Vec3 eulerDeg = transform.GetEuler();
    Vec3 scale = transform.GetScale();

    ImGui::Text("%s", name.c_str());
    ImGui::PushID(GetImGuiId(name));
    if (ImGui::DragFloat3("position", position.ValuePtr(), 0.25f)) {
      transform.SetPosition(position);
    }
    if (ImGui::DragFloat3("rotation", eulerDeg.ValuePtr(), 0.5f)) {
      transform.SetEuler(eulerDeg);
    }
    if (ImGui::DragFloat3("scale", scale.ValuePtr(), 0.05f)) {
      transform.Scale(scale);
    }
    ImGui::PopID();
  }

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
};

SONO_IMPLEMENT_MAIN(SonoEditor);
