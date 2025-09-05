#ifndef SN_GL_RENDER_PASS_H
#define SN_GL_RENDER_PASS_H

#include <render-backend/sngl/gl_command.h>
#include <render/render_pass.h>
#include <vector>

class GLRenderPass : public RenderPass {
public:
  explicit GLRenderPass(const RenderPassDesc &desc, Allocator *allocator = nullptr)
    : RenderPass(desc, allocator) {}

  // clang-format off
  void BindPipeline(RenderPipeline *pso) override;
  void BindVertexBuffer(Buffer *vb) override;
  void BindIndexBuffer(Buffer *vb) override;
  void Draw(u32 vertexCount, u32 instanceCount, u32 firstVertex, u32 firstInstance) override;
  void DrawIndexed(u32 idxCount, u32 instanceCount, u32 firstIdx, u32 firstInstance) override;
  // clang-format on

public:
  std::vector<GLCommand *> cmds;
};

#endif // !SN_GL_RENDER_PASS_H
