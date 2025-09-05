#ifndef SN_GL_COMMAND_LIST_H
#define SN_GL_COMMAND_LIST_H

#include <core/memory/allocators/arena.h>
#include <render/command_list.h>

class GLCommandList : public CommandList {
public:
  explicit GLCommandList(Allocator *allocator)
    : CommandList(allocator) {}

  RenderPass *BeginRenderPass(const RenderPassDesc &) override;
  void EndRenderPass() override;
};

#endif // !SN_GL_COMMAND_LIST_H
