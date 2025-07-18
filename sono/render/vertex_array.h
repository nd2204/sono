#ifndef SN_VERTEX_ARRAY_H
#define SN_VERTEX_ARRAY_H

#include "vertex_layout.h"
#include "render/buffer/buffer_base.h"

class VertexArray {
public:
  virtual ~VertexArray() = default;

  virtual void Bind() const = 0;

  virtual void Unbind() const = 0;

  virtual void AddVertexBuffer(IBuffer *buffer, const VertexLayout &layout) = 0;

  virtual void SetIndexBuffer(IBuffer *buffer) = 0;
};

#endif // !SN_VERTEX_ARRAY_H
