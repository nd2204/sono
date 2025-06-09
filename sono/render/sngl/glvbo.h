#ifndef GLVBO_H
#define GLVBO_H

#include "core/common/types.h"
#include "glad/glad.h"

class GLVertexBuffer {
public:
  /// @brief Constructs a Vertex Buffer Object (VBO) with the given data and
  /// size.
  /// @param data Pointer to the vertex data.
  /// @param size Size of the vertex data in bytes.
  /// @param usage OpenGL usage hint (e.g., GL_STATIC_DRAW, GL_DYNAMIC_DRAW).
  /// default is GL_STATIC_DRAW.
  GLVertexBuffer(const void *data, u32 size, u32 usage = GL_STATIC_DRAW);

  /// @brief Constructs a Default Vertex Buffer Object (VBO).
  GLVertexBuffer();

  /// @brief Sets the vertex buffer data.
  /// @param data Pointer to the vertex data.
  /// @param size Size of the vertex data in bytes.
  /// @param usage OpenGL usage hint (e.g., GL_STATIC_DRAW, GL_DYNAMIC_DRAW).
  /// default is GL_STATIC_DRAW.
  void SetBufferData(const void *data, u32 size, u32 usage = GL_STATIC_DRAW);

  /// @brief Binds the vertex buffer.
  /// This makes the VBO active for subsequent OpenGL calls.
  void Bind() const;

  /// @brief Unbinds the vertex buffer.
  void Unbind() const;

  /// @brief Gets the OpenGL ID of the vertex buffer.
  /// @return The OpenGL ID of the vertex buffer.
  u32 GetID() const;

  operator u32() const;

  ~GLVertexBuffer();

private:
  u32 m_ID;
  usize m_Size;
};

#endif // !GLVBO_H