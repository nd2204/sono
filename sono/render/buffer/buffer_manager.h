#ifndef SN_BUFFER_MANAGER_H
#define SN_BUFFER_MANAGER_H

#include "core/common/singleton.h"

#include "render/buffer/buffer_base.h"
#include "render/buffer/index_buffer.h"
#include "render/vertex_layout.h"

#include <set>

/// This class is responsible for managing buffers for rendering
/// All buffers creation and deletion should be called by this class
/// TODO: include locking mechanism for resources when going threaded
class BufferManager : public Singleton<BufferManager> {
public:
  BufferManager();

  virtual ~BufferManager();

  virtual VertexLayout *CreateVertexLayout() = 0;

  virtual b8 DeleteVertexLayout(VertexLayout *pLayout) = 0;

  virtual IBuffer *CreateVertexBuffer(
    BufferUsage usage, usize vertSize, usize vertCount, const void *vertices = nullptr
  ) = 0;

  virtual b8 DeleteVertexBuffer(IBuffer *pBuf) = 0;

  virtual IBuffer *CreateIndexBuffer(
    BufferUsage usage, IndexType type, usize idxCount, const void *indices
  ) = 0;

  virtual b8 DeleteIndexBuffer(IBuffer *pBuf) = 0;

  virtual void DeleteAllVertexBuffers();

  virtual void DeleteAllIndexBuffers();

  virtual void DeleteAllLayout();

protected:
  std::set<IBuffer *> m_VertexBuffers;
  std::set<IBuffer *> m_IndexBuffers;
  std::set<VertexLayout *> m_VertexLayouts;
};

#endif // !SN_BUFFER_MANAGER_H
