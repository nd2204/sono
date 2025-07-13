#include "core/memory/memory_system.h"

#include "gl_buffer_manager.h"
#include "gl_vertex_buffer.h"
#include "gl_index_buffer.h"

// clang-format off

GLBufferManager::~GLBufferManager() {
  DeleteAllVertexBuffers();
  DeleteAllIndexBuffers();
  DeleteAllLayout();
}

GLenum GLBufferManager::ConvertBufferUsage(BufferUsage usage) {
  switch (usage) {
    case BufferUsage::STATIC:    return GL_STATIC_DRAW;
    case BufferUsage::DYNAMIC:   return GL_DYNAMIC_DRAW;
    case BufferUsage::STREAM:    return GL_DYNAMIC_DRAW;
  }
  ASSERT(false);
  return 0;
}

// clang-format on

VertexLayout *GLBufferManager::CreateVertexLayout() {
  VertexLayout *layout = SN_NEW(ALLOC_TYPE_RENDER_SYSTEM) VertexLayout();
  m_VertexLayouts.insert(layout);
  return layout;
}

b8 GLBufferManager::DeleteVertexLayout(VertexLayout *pLayout) {
  ASSERT(pLayout);
  SN_FREE(pLayout);
  return true;
}

IBuffer *GLBufferManager::CreateVertexBuffer(
  BufferUsage usage, usize vertCount, usize vertSize, const void *vertices
) {
  IBuffer *buffer = SN_NEW(ALLOC_TYPE_RENDER_SYSTEM) GLVertexBuffer(usage, vertSize, vertCount);
  if (vertices) {
    buffer->Update(vertices, vertCount * vertSize);
  }
  m_VertexBuffers.insert(buffer);
  return buffer;
}

b8 GLBufferManager::DeleteVertexBuffer(IBuffer *pBuf) {
  ASSERT(pBuf);
  GLVertexBuffer *mem = static_cast<GLVertexBuffer *>(pBuf);
  SN_FREE(mem);
  return true;
};

IBuffer *GLBufferManager::CreateIndexBuffer(
  BufferUsage usage, IndexType type, usize idxCount, const void *indices
) {
  IBuffer *buffer = SN_NEW(ALLOC_TYPE_RENDER_SYSTEM) GLIndexBuffer(usage, type, idxCount);
  if (indices) {
    buffer->Update(indices, idxCount * GLIndexBuffer::IndexTypeSize(type));
  }
  m_IndexBuffers.insert(buffer);
  return buffer;
}

b8 GLBufferManager::DeleteIndexBuffer(IBuffer *pBuf) {
  ASSERT(pBuf);
  GLIndexBuffer *mem = static_cast<GLIndexBuffer *>(pBuf);
  SN_FREE(mem);
  return true;
}
