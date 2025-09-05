#ifndef SN_COMMAND_LIST_H
#define SN_COMMAND_LIST_H

#include <render/render_pass.h>
#include <core/common/types.h>
#include <core/memory/allocator.h>

class CommandList {
public:
  explicit CommandList(Allocator *allocator);
  virtual ~CommandList() = default;

  virtual RenderPass *BeginRenderPass(const RenderPassDesc &) = 0;
  virtual void EndRenderPass() = 0;

public:
  RenderPass *renderPass;

protected:
  Allocator *m_Allocator;
};

#endif // !SN_COMMAND_LIST_H
