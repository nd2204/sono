#ifndef GLVAO_H
#define GLVAO_H

#include "core/common/types.h"
#include "glad/glad.h"

#include "render/vertex_array.h"
#include "render/vertex_layout.h"
#include "gl_vertex_buffer.h"
#include "gl_index_buffer.h"

class GLVertexArray : public VertexArray {
public:
  GLVertexArray();

  void Bind() const override;

  void Unbind() const override;

  void AddVertexBuffer(IBuffer *buffer, const VertexLayout &layout) override;

  void SetIndexBuffer(IBuffer *buffer) override;

  GLuint GetID() const;

  const GLIndexBuffer *GetCurrentIndexBuffer() const;

  const std::vector<GLVertexBuffer *> &GetVertexBuffers() const;

  GLVertexBuffer *operator[](usize index);

  const GLVertexBuffer *operator[](usize index) const;

  ~GLVertexArray();

private:
  std::vector<GLVertexBuffer *> m_VertexBuffers;
  // std::vector<VertexLayout> m_VertexLayouts;

  GLIndexBuffer *m_pIndexBuffer;
  GLuint m_ID;
};

#endif
