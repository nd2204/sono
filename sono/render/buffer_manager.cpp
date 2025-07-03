#include "buffer_manager.h"
#include "index_buffer.h"
#include "vertex_buffer.h"
#include "vertex_layout.h"

template <>
BufferManager *Singleton<BufferManager>::m_sInstance = nullptr;

// --------------------------------------------------------------------------------
BufferManager::BufferManager() {}
// --------------------------------------------------------------------------------
BufferManager::~BufferManager() {}
// --------------------------------------------------------------------------------
void BufferManager::DeleteAllVertexBuffers() {
  for (IBuffer *buffer : m_VertexBuffers) {
    DeleteVertexBuffer(buffer);
  }
  m_VertexBuffers.clear();
}

void BufferManager::DeleteAllIndexBuffers() {
  for (IBuffer *buffer : m_IndexBuffers) {
    DeleteIndexBuffer(buffer);
  }
  m_IndexBuffers.clear();
}

void BufferManager::DeleteAllLayout() {
  for (VertexLayout *layout : m_VertexLayouts) {
    DeleteVertexLayout(layout);
  }
  m_VertexLayouts.clear();
}
