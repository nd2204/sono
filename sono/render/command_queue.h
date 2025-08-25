#ifndef SN_COMMAND_QUEUE_H
#define SN_COMMAND_QUEUE_H

#include "shader/shader.h"
#include <render/command_list.h>
#include <render/render_pass.h>
#include <vector>

struct SubmitInfo {
  std::vector<CommandList *> cmdLists;
};

class CommandQueue {
public:
  virtual ~CommandQueue() = default;
  virtual void Submit(const SubmitInfo &info) = 0;

  std::vector<RenderPass *> renderPassTable;
  std::vector<RenderPipeline *> pipelineTable;
};

#endif // !SN_COMMAND_QUEUE_H
