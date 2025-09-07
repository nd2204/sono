#ifndef SN_VERTEX_ARRAY_H
#define SN_VERTEX_ARRAY_H

#include <render/vertex_layout.h>
#include <render/buffer_base.h>

class VertexArray {
public:
  virtual ~VertexArray() = default;

  virtual void Bind() const = 0;

  virtual void Unbind() const = 0;

  virtual void AddVertexBuffer(Buffer *buffer, const VertexLayout &layout) = 0;

  virtual void SetIndexBuffer(Buffer *buffer) = 0;
};

#endif // !SN_VERTEX_ARRAY_H
