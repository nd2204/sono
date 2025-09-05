#ifndef SN_RENDER_PASS_H
#define SN_RENDER_PASS_H

#include <core/memory/allocator.h>
#include <render/colors.h>
#include <render/render_pipeline.h>
#include <render/buffer_base.h>

struct RenderPassDesc {
  Color3 clearCol;
};

class RenderPass {
public:
  explicit RenderPass(const RenderPassDesc &desc, Allocator *allocator = nullptr)
    : m_Desc(desc)
    , m_Allocator(allocator) {}

  // clang-format off
  virtual void BindPipeline(RenderPipeline *pso) = 0;
  virtual void BindVertexBuffer(Buffer *vb) = 0;
  virtual void BindIndexBuffer(Buffer *vb) = 0;
  virtual void Draw(u32 vertexCount, u32 instanceCount, u32 firstVertex, u32 firstInstance) = 0;
  virtual void DrawIndexed(u32 vertexCount, u32 instanceCount, u32 firstVertex, u32 firstInstance) = 0;
  // clang-format on

protected:
  RenderPassDesc m_Desc;
  Allocator *m_Allocator;
};

#endif // !SN_RENDER_PASS_H
