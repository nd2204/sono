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

void GLBufferManager::DeleteVertexLayout(VertexLayout *pLayout) {
  ASSERT(pLayout);
  m_VertexLayouts.erase(pLayout);
  SN_FREE(pLayout);
}

IBuffer *GLBufferManager::CreateVertexBuffer(BufferUsage usage, usize vertCount, usize vertSize) {
  IBuffer *buffer = SN_NEW(ALLOC_TYPE_RENDER_SYSTEM) GLVertexBuffer(usage, vertSize, vertCount);
  m_VertexBuffers.insert(buffer);
  return buffer;
}

void GLBufferManager::DeleteVertexBuffer(IBuffer *pBuf) {
  ASSERT(pBuf);
  m_VertexBuffers.erase(pBuf);
  SN_FREE(pBuf);
};

IBuffer *GLBufferManager::CreateIndexBuffer(BufferUsage usage, IndexType type, usize idxCount) {
  IBuffer *buffer = SN_NEW(ALLOC_TYPE_RENDER_SYSTEM) GLIndexBuffer(usage, type, idxCount);
  m_IndexBuffers.insert(buffer);
  return buffer;
}

void GLBufferManager::DeleteIndexBuffer(IBuffer *pBuf) {
  ASSERT(pBuf);
  m_IndexBuffers.erase(pBuf);
  SN_FREE(pBuf);
}
