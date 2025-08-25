#ifndef SN_COMMAND_LIST_H
#define SN_COMMAND_LIST_H

#include <render/buffer_base.h>
#include <render/shader/shader.h>
#include <core/common/types.h>

struct RenderPassDesc {};

class CommandList {
public:
  virtual ~CommandList() = default;
  virtual void BeginRenderPass(const RenderPassDesc &) = 0;
  virtual void EndRenderPass() = 0;
  virtual void BindPipeline(RenderPipeline *pso);
  virtual void BindVertexBuffer(Buffer *vb);
  virtual void Draw(u32 vertexCount, u32 instanceCount, u32 firstVertex, u32 firstInstance);
  virtual void DrawIndexed(u32 vertexCount, u32 instanceCount, u32 firstVertex, u32 firstInstance);
};

#endif // !SN_COMMAND_LIST_H
