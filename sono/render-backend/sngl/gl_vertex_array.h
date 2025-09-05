#ifndef GLVAO_H
#define GLVAO_H

#include "core/common/types.h"
#include "glad/glad.h"

#include "render/vertex_array.h"
#include "render/vertex_layout.h"
#include "gl_buffer_base.h"

class GLVertexArray : public VertexArray {
public:
  GLVertexArray();

  void Bind() const override;

  void Unbind() const override;

  void AddVertexBuffer(Buffer *buffer, const VertexLayout &layout) override;

  void SetIndexBuffer(Buffer *buffer) override;

  GLuint GetID() const;

  const GLBuffer *GetCurrentIndexBuffer() const;

  const std::vector<GLBuffer *> &GetVertexBuffers() const;

  GLBuffer *operator[](usize index);

  const GLBuffer *operator[](usize index) const;

  ~GLVertexArray();

private:
  std::vector<GLBuffer *> m_VertexBuffers;
  // std::vector<VertexLayout> m_VertexLayouts;
  GLBuffer *m_pIndexBuffer;
  GLuint m_ID;
};

#endif
