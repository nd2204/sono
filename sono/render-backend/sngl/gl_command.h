#ifndef SN_GL_COMMAND_H
#define SN_GL_COMMAND_H

#include <render-backend/sngl/gl_buffer_base.h>
#include <render-backend/sngl/gl_render_pipeline.h>

enum class CmdType {
  BindPipeline,
  BindVertexBuffer,
  BindIndexBuffer,
  Draw,
  DrawIndexed,
};

struct CmdBindPipeline {
  GLRenderPipeline *pipeline;
};

struct CmdBindBuffer {
  GLBuffer *buffer;
};

struct CmdDraw {
  u32 vertexCount, instanceCount, firstVertex, firstInstance;
};

struct CmdDrawIndexed {
  u32 idxCount, instanceCount, firstIdx, firstInstance;
};

struct GLCommand {
  CmdType type;
  union {
    CmdBindPipeline cmdBindPipeline;
    CmdBindBuffer cmdBindBuffer;
    CmdDraw cmdDraw;
    CmdDrawIndexed cmdDrawIndexed;
  };
};

#endif // !SN_GL_COMMAND_H
