#ifndef SN_RENDER_OP_H
#define SN_RENDER_OP_H

#include "shader/shader.h"
#include "vertex_array.h"
#include "core/math/mat4.h"
#include "render/resource/texture.h"
#include <vector>

class RenderSystem;

enum class PrimitiveType { TRIANGLES, TRIANGLE_STRIPS, TRIANGLE_FAN, LINES, LINE_STRIPS, POINTS };

class RenderCommand {
public:
  virtual ~RenderCommand() = default;
  virtual void Execute(RenderSystem &renderSys) const = 0;
};

class BindShaderCommand : public RenderCommand {
public:
  BindShaderCommand(RenderPipeline *shader);
  void Execute(RenderSystem &renderSys) const;

private:
  RenderPipeline *m_Pipeline;
};

template <typename T>
class SetUniformCommand : public RenderCommand {
public:
  SetUniformCommand(const char *uniform, const T &data)
    : m_Uniform(uniform)
    , m_Data(data) {}

  void Execute(RenderSystem &renderSys) const override;

private:
  const char *m_Uniform;
  const T &m_Data;
};

class SetViewportCommand : public RenderCommand {
public:
  SetViewportCommand(i32 xpos, i32 ypos, i32 width, i32 height);
  void Execute(RenderSystem &renderSys) const override;

private:
  i32 m_PosX, m_PosY, m_Width, m_Height;
};

class DrawCommand : public RenderCommand {
public:
  DrawCommand(
    const VertexArray *va, u32 vertCount, const Mat4 &model = Mat4::Identity,
    PrimitiveType topology = PrimitiveType::TRIANGLES
  );
  void Execute(RenderSystem &renderSys) const override;

private:
  const VertexArray *m_VAO;
  const Mat4 &m_Transform;
  u32 m_NumVertices;
  PrimitiveType m_Topology;
};

class DrawIndexedCommand : public RenderCommand {
public:
  DrawIndexedCommand(
    const VertexArray *va, u32 idxCount, const Mat4 &model = Mat4::Identity,
    PrimitiveType topology = PrimitiveType::TRIANGLES
  );
  void Execute(RenderSystem &renderSys) const override;

private:
  const VertexArray *m_VAO;
  const Mat4 &m_Transform;
  u32 m_NumIndex;
  PrimitiveType m_Topology;
};

class ClearCommand : public RenderCommand {
public:
  ClearCommand(const Vec4 &color = Vec4(0.0f));
  void Execute(RenderSystem &renderSys) const override;

private:
  Vec4 m_Color;
};

#endif // !SN_RENDER_OP_H
