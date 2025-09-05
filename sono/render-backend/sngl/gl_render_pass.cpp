#include "core/common/logger.h"
#include "render-backend/sngl/gl_buffer_base.h"
#include "render-backend/sngl/gl_render_pipeline.h"
#include <render-backend/sngl/gl_render_pass.h>

void GLRenderPass::BindPipeline(RenderPipeline *pso) {
  GLCommand *cmd = m_Allocator->New<GLCommand>();
  cmd->type = CmdType::BindPipeline;
  cmd->cmdBindPipeline.pipeline = reinterpret_cast<GLRenderPipeline *>(pso);
  this->cmds.push_back(cmd);
}

void GLRenderPass::BindVertexBuffer(Buffer *vb) {
  GLCommand *cmd = m_Allocator->New<GLCommand>();
  GLBuffer *buffer = reinterpret_cast<GLBuffer *>(vb);
  ASSERT(buffer->GetUsage() & BufferUsage::Vertex);
  cmd->type = CmdType::BindVertexBuffer;
  cmd->cmdBindBuffer.buffer = buffer;
  this->cmds.push_back(cmd);
}

void GLRenderPass::BindIndexBuffer(Buffer *vb) {
  GLCommand *cmd = m_Allocator->New<GLCommand>();
  GLBuffer *buffer = reinterpret_cast<GLBuffer *>(vb);
  ASSERT(buffer->GetUsage() & BufferUsage::Index);
  cmd->type = CmdType::BindIndexBuffer;
  cmd->cmdBindBuffer.buffer = buffer;
  this->cmds.push_back(cmd);
}

void GLRenderPass::Draw(u32 vertexCount, u32 instanceCount, u32 firstVertex, u32 firstInstance) {
  GLCommand *cmd = m_Allocator->New<GLCommand>();
  cmd->type = CmdType::Draw;
  cmd->cmdDraw.vertexCount = vertexCount;
  cmd->cmdDraw.instanceCount = instanceCount;
  cmd->cmdDraw.firstVertex = firstVertex;
  cmd->cmdDraw.firstInstance = firstInstance;
  this->cmds.push_back(cmd);
}

void GLRenderPass::DrawIndexed(u32 idxCount, u32 instanceCount, u32 firstIdx, u32 firstInstance) {
  GLCommand *cmd = m_Allocator->New<GLCommand>();
  cmd->type = CmdType::DrawIndexed;
  cmd->cmdDrawIndexed.idxCount = idxCount;
  cmd->cmdDrawIndexed.instanceCount = instanceCount;
  cmd->cmdDrawIndexed.firstIdx = firstIdx;
  cmd->cmdDrawIndexed.firstInstance = firstInstance;
  this->cmds.push_back(cmd);
}
