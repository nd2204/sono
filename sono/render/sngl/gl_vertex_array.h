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

  void Bind() const;

  void Unbind() const;

  virtual void AddVertexBuffer(IBuffer *buffer) override;

  virtual void SetIndexBuffer(IBuffer *buffer) override;

  virtual void SetVertexLayout(VertexLayout *buffer) override;

  void Draw(GLenum mode = GL_TRIANGLES);

  void DrawIndexed(GLenum mode = GL_TRIANGLES);

  ~GLVertexArray();

private:
  u32 m_ID;
  std::vector<GLVertexBuffer *> m_VertexBuffers;
  VertexLayout *m_pVertexLayout;
  GLIndexBuffer *m_pIndexBuffer;
};

#endif
