#include "render/buffer/buffer_manager.h"

template <>
BufferManager *Singleton<BufferManager>::m_sInstance = nullptr;

// --------------------------------------------------------------------------------
BufferManager::BufferManager() {}
// --------------------------------------------------------------------------------
BufferManager::~BufferManager() {}
// --------------------------------------------------------------------------------
void BufferManager::DeleteAllVertexBuffers() {
  for (auto it = m_VertexBuffers.begin(); it != m_VertexBuffers.end();) {
    if (DeleteVertexBuffer(*it))
      it = m_VertexBuffers.erase(it);
    else {
      it++;
    }
  }
}

void BufferManager::DeleteAllIndexBuffers() {
  for (auto it = m_IndexBuffers.begin(); it != m_IndexBuffers.end();) {
    if (DeleteIndexBuffer(*it))
      it = m_IndexBuffers.erase(it);
    else {
      it++;
    }
  }
}

void BufferManager::DeleteAllLayout() {
  for (auto it = m_VertexLayouts.begin(); it != m_VertexLayouts.end();) {
    if (DeleteVertexLayout(*it))
      it = m_VertexLayouts.erase(it);
    else {
      it++;
    }
  }
}
