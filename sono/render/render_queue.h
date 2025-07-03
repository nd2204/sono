#ifndef SN_RENDER_QUEUE_H
#define SN_RENDER_QUEUE_H

#include "render_command.h"
#include "render_context.h"
#include <vector>

class RenderQueue {
public:
  RenderQueue();
  ~RenderQueue();

  void Submit(const RenderCommand *cmd);
  void Flush(RenderSystem &renderSys);
  void Clear();

private:
  std::vector<const RenderCommand *> m_Commands;
};

#endif // !SN_RENDER_QUEUE_H
