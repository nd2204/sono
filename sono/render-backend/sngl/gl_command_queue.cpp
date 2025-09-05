#include <render-backend/sngl/gl_command_queue.h>
#include <render-backend/sngl/gl_command.h>

void GLCommandQueue::ExecuteGLCommand(GLCommand *cmd) {
  switch (cmd->type) {
    case CmdType::BindPipeline:
      cmd->cmdBindPipeline.pipeline->Bind();
      break;
    case CmdType::BindIndexBuffer: {
      m_VertexBuffer = cmd->cmdBindBuffer.buffer;
      cmd->cmdBindBuffer.buffer->Bind();
      break;
    }
    case CmdType::BindVertexBuffer: {
      m_IndexBuffer = cmd->cmdBindBuffer.buffer;
      cmd->cmdBindBuffer.buffer->Bind();
      break;
    }
    case CmdType::Draw: {
      const CmdDraw &draw = cmd->cmdDraw;
      glDrawArrays(GL_TRIANGLES, draw.firstVertex, draw.vertexCount);
      break;
    }
    case CmdType::DrawIndexed: {
      const CmdDrawIndexed &draw = cmd->cmdDrawIndexed;
      GLenum type = GL_UNSIGNED_INT;
      if (m_IndexBuffer->GetStride() == sizeof(u16))
        type = GL_UNSIGNED_SHORT;
      else if (m_IndexBuffer->GetStride() == sizeof(u8))
        type = GL_UNSIGNED_BYTE;
      glDrawElements(GL_TRIANGLES, draw.idxCount, type, reinterpret_cast<void *>(draw.firstIdx));
      break;
    }
  }
}
// --------------------------------------------------------------------------------
void GLCommandQueue::Submit(const SubmitInfo &info) {
  for (CommandList *cmdList : info.cmdLists) {
    GLRenderPass *pass =
      reinterpret_cast<GLRenderPass *>(reinterpret_cast<GLCommandList *>(cmdList)->renderPass);
    for (GLCommand *cmd : pass->cmds) {
      this->ExecuteGLCommand(cmd);
      m_ActivePass.push_back(pass);
    }
  }
}
// --------------------------------------------------------------------------------
void GLCommandQueue::WaitIdle() { glFlush(); }
// --------------------------------------------------------------------------------
void GLCommandQueue::Reset() { m_ActivePass.clear(); }
