#include "core/memory/memory_system.h"

#include "render/sngl/gl_render_device.h"
#include "render/sngl/gl_buffer_base.h"
#include "render/sngl/gl_texture.h"
#include "render/sngl/gl_shader.h"

const std::string kDefaultVertexShader = // force clang-format
  "#version 330 core"
  "layout (location = 0) in vec3 aPos;"
  "uniform mat4 uModel;"
  "uniform mat4 uView;"
  "uniform mat4 uProj;"
  "void main() {"
  "  gl_Position = uProj * uView * uModel * vec4(aPos, 1.0);"
  "}";

const std::string kDefaultFragmentShader = // force clang-format
  "#version 330 core"
  "out vec4 FragColor;"
  "void main() {"
  "  FragColor = vec4(1.0f);"
  "}";

GLRenderDevice::GLRenderDevice(Allocator *allocator)
  : RenderDevice(allocator) {
  DeleteAllBuffers();
  DeleteAllLayout();
}
// --------------------------------------------------------------------------------
GLRenderDevice::~GLRenderDevice() {
  DeleteAllBuffers();
  DeleteAllLayout();
}
// --------------------------------------------------------------------------------
VertexLayout *GLRenderDevice::CreateVertexLayout() {
  VertexLayout *layout = SN_NEW(ALLOC_TYPE_RENDER_SYSTEM) VertexLayout();
  m_VertexLayouts.insert(layout);
  return layout;
}
// --------------------------------------------------------------------------------
RenderPipeline *GLRenderDevice::CreateDefaultPipeline() {
  // clang-format off
  PipelineDesc desc = {
    .vertex = CreateShader({
      .src = kDefaultVertexShader,
      .stage = ShaderStage::VERTEX,
    }),
    .fragment = CreateShader({
      .src = kDefaultFragmentShader,
      .stage = ShaderStage::FRAGMENT
    })
  };
  // clang-format on
  return m_pAllocator->New<GLRenderPipeline>(desc);
}
// --------------------------------------------------------------------------------
RenderPipeline *GLRenderDevice::CreatePipeline(const PipelineDesc &desc) {
  return m_pAllocator->New<GLRenderPipeline>(desc);
}
// --------------------------------------------------------------------------------
Shader *GLRenderDevice::CreateShader(const ShaderDesc &desc) {
  return m_pAllocator->New<GLShader>(desc);
}
// --------------------------------------------------------------------------------
Texture *GLRenderDevice::CreateTexture(
  TextureType type, TextureFormat internalFmt, TextureFormat fmt, u32 width, u32 height
) {
  return m_pAllocator->New<GLTexture>(type, internalFmt, fmt, width, height);
}
// --------------------------------------------------------------------------------
b8 GLRenderDevice::DeleteVertexLayout(VertexLayout *pLayout) {
  ASSERT(pLayout);
  SN_FREE(pLayout);
  return true;
}
// --------------------------------------------------------------------------------
Buffer *GLRenderDevice::CreateBuffer(const BufferDesc &desc) {
  Buffer *buffer = SN_NEW(ALLOC_TYPE_RENDER_SYSTEM) GLBuffer(desc);
  m_Buffers.insert(buffer);
  return buffer;
}
// --------------------------------------------------------------------------------
b8 GLRenderDevice::DeleteBuffer(Buffer *pBuf) {
  ASSERT(pBuf);
  GLBuffer *mem = static_cast<GLBuffer *>(pBuf);
  SN_FREE(mem);
  return true;
};
