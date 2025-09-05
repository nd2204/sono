#ifndef SN_GL_COMMAND_QUEUE_H
#define SN_GL_COMMAND_QUEUE_H

#include "render-backend/sngl/gl_render_pipeline.h"
#include <render/command_queue.h>
#include <render-backend/sngl/gl_render_pass.h>
#include <render-backend/sngl/gl_command_list.h>

class GLCommandQueue : public CommandQueue {
public:
  GLCommandQueue() = default;
  ~GLCommandQueue() = default;

  void Submit(const SubmitInfo &info) = 0;
  void WaitIdle() = 0;
  void Reset() = 0;

private:
  void ExecuteGLCommand(GLCommand *cmd);

private:
  GLBuffer *m_VertexBuffer;
  GLBuffer *m_IndexBuffer;
  GLRenderPipeline *m_PSO;
};

#endif // !SN_GL_COMMAND_QUEUE_H
