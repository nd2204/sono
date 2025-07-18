#ifndef SN_GL_BUFFER_MANAGER_H
#define SN_GL_BUFFER_MANAGER_H

#include "render/buffer/buffer_manager.h"
#include "glad/glad.h"

class GLBufferManager : public BufferManager {
public:
  ~GLBufferManager();

  static GLenum ConvertBufferUsage(BufferUsage usage);

  VertexLayout *CreateVertexLayout() override;

  b8 DeleteVertexLayout(VertexLayout *pLayout) override;

  IBuffer *CreateVertexBuffer(
    BufferUsage usage, usize vertSize, usize vertCount, const void *vertices
  ) override;

  b8 DeleteVertexBuffer(IBuffer *pBuf) override;

  IBuffer *CreateIndexBuffer(
    BufferUsage usage, IndexType type, usize idxCount, const void *indices
  ) override;

  b8 DeleteIndexBuffer(IBuffer *pBuf) override;
};

// ------------------------------------------------------------------------------------------

#endif // !SN_GL_BUFFER_MANAGER_H
