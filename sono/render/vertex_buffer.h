#ifndef SN_VERTEX_BUFFER_H
#define SN_VERTEX_BUFFER_H

#include "core/common/types.h"
#include "render/buffer_base.h"

// ================================================================================
// VERTEX BUFFER ABSTRACTION
// ================================================================================

class IVertexBuffer {
public:
  virtual u32 GetStride() const = 0;

  virtual usize GetVertexSize() const = 0;

  virtual u32 GetVertexCount() const = 0;
};

#endif // !SN_VERTEX_BUFFER_H
