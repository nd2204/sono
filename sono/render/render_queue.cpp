#include "render_queue.h"

RenderQueue::RenderQueue() {}

RenderQueue::~RenderQueue() {}

void RenderQueue::Submit(const RenderCommand *cmd) { m_Commands.push_back(cmd); }

void RenderQueue::Flush(RenderSystem &renderSys) {
  for (const RenderCommand *cmd : m_Commands) {
    cmd->Execute(renderSys);
  }
  m_Commands.clear();
}

void RenderQueue::Clear() { m_Commands.clear(); }
