#ifndef GLVAO_H
#define GLVAO_H

#include "core/common/types.h"
#include "glad/glad.h"
#include "glebo.h"
#include "glvbo.h"

#include <memory>
#include <vector>

struct GLVertexAttribute {
  u32 count;
  u32 type;
  u32 tsize;
  b8 normalized;

  /// @brief Constructs a vertex attribute with the given parameters.
  /// @param count Number of components per vertex attribute (e.g., 3 for vec3).
  /// @param type OpenGL data type (e.g., GL_FLOAT, GL_INT).
  /// @param tsize Size of the data type in bytes (e.g., sizeof(float)).
  /// @param normalized Whether the data should be normalized (e.g., GL_TRUE or
  /// GL_FALSE).
  GLVertexAttribute(u32 count, u32 type, u32 tsize, b8 normalized)
    : count(count)
    , type(type)
    , tsize(tsize)
    , normalized(normalized) {}

  /// @brief Default constructor for GLVertexAttribute.
  GLVertexAttribute()
    : count(0)
    , type(0)
    , tsize(0)
    , normalized(0) {}
};

class GLVertexLayout {
public:
  GLVertexLayout();

  /// @brief Pushes a vertex attribute into the layout.
  /// @param attribute The vertex attribute to be pushed.
  void Push(const GLVertexAttribute &attribute);

  /// @brief Pushes a vertex attribute into the layout.
  /// @param attribute The vertex attribute to be pushed.
  void Push(GLVertexAttribute &&attribute);

  /// @return The stride of the vertex layout.
  u32 GetStride() const;

  /// @return A constant reference to the vector of vertex attributes.
  const std::vector<GLVertexAttribute> &GetAttributes() const;

private:
  std::vector<GLVertexAttribute> m_Attributes;
  u32 m_Stride;
  static u32 s_MaxAttributeCount;

  void UpdateStride(const GLVertexAttribute &attribute);
};

class GLVertexArray {
public:
  GLVertexArray();

  void Bind() const;
  void Unbind() const;

  /// @brief Sets the vertex buffer and its layout for this VAO.
  /// @param vbo The vertex buffer object to bind.
  /// @param layout The vertex layout describing the attributes.
  void SetVertexBuffer(const GLVertexBuffer &vbo, const GLVertexLayout &layout);

  /// @brief Sets the element buffer object (EBO) for this VAO.
  /// @param ebo The element buffer object to bind.
  void SetELementBuffer(const GLElementBuffer &ebo);

  /// @brief Draws the elements using the bound VAO and EBO.
  /// This function assumes that the VAO and EBO are already bound.
  void Draw(u32 mode = GL_TRIANGLES);

  ~GLVertexArray();

private:
  u32 m_ID;
  std::shared_ptr<GLElementBuffer> m_pEBO;
};

#endif