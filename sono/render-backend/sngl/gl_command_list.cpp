#include <render-backend/sngl/gl_command_list.h>
#include <render-backend/sngl/gl_render_pass.h>

RenderPass *GLCommandList::BeginRenderPass(const RenderPassDesc &desc) {
  if (!renderPass) {
    renderPass = m_Allocator->New<GLRenderPass>(desc);
  }
  return renderPass;
}

void GLCommandList::EndRenderPass() {
  if (renderPass) {
    m_Allocator->Free(renderPass);
    renderPass = nullptr;
  }
}
