#ifndef SN_VERTEX_ARRAY_H
#define SN_VERTEX_ARRAY_H

#include "vertex_buffer.h"
#include "index_buffer.h"
#include "vertex_layout.h"

class VertexArray {
public:
  virtual ~VertexArray() = default;

  virtual void AddVertexBuffer(IBuffer *buffer) = 0;

  virtual void SetIndexBuffer(IBuffer *buffer) = 0;

  virtual void SetVertexLayout(VertexLayout *buffer) = 0;
};

#endif // !SN_VERTEX_ARRAY_H
