#ifndef SN_GL_BUFFER_MANAGER_H
#define SN_GL_BUFFER_MANAGER_H

#include "render/buffer_manager.h"
#include "glad/glad.h"

class GLBufferManager : public BufferManager {
public:
  ~GLBufferManager();

  static GLenum ConvertBufferUsage(BufferUsage usage);

  VertexLayout *CreateVertexLayout() override;

  b8 DeleteVertexLayout(VertexLayout *pLayout) override;

  IBuffer *CreateVertexBuffer(BufferUsage usage, usize vertCount, usize vertSize) override;

  b8 DeleteVertexBuffer(IBuffer *pBuf) override;

  IBuffer *CreateIndexBuffer(BufferUsage usage, IndexType type, usize idxCount) override;

  b8 DeleteIndexBuffer(IBuffer *pBuf) override;
};

// ------------------------------------------------------------------------------------------

#endif // !SN_GL_BUFFER_MANAGER_H
