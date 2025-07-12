#ifndef SN_RENDER_OP_H
#define SN_RENDER_OP_H

#include "render_context.h"
#include "vertex_array.h"
#include "texture.h"

class RenderSystem;

enum class PrimitiveType { TRIANGLES, TRIANGLE_STRIPS, TRIANGLE_FAN, LINES, LINE_STRIPS, POINTS };

class RenderCommand {
public:
  virtual ~RenderCommand() = default;
  virtual void Execute(RenderSystem &renderSys) const = 0;
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
    VertexArray *va, u32 vertCount, Texture *tex = nullptr,
    PrimitiveType topology = PrimitiveType::TRIANGLES
  );
  void Execute(RenderSystem &renderSys) const override;

private:
  VertexArray *m_VAO;
  Texture *m_Tex;
  u32 m_NumVertices;
  PrimitiveType m_Topology;
};

class DrawIndexedCommand : public RenderCommand {
public:
  DrawIndexedCommand(
    VertexArray *va, u32 idxCount, Texture *tex = nullptr,
    PrimitiveType topology = PrimitiveType::TRIANGLES
  );
  void Execute(RenderSystem &renderSys) const override;

private:
  VertexArray *m_VAO;
  Texture *m_Tex;
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
