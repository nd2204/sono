#ifndef SN_DEBUG_DRAW_H
#define SN_DEBUG_DRAW_H

#include <render/vertex_array.h>
#include <render/buffer_base.h>
#include <core/common/singleton.h>
#include <core/system.h>

class RenderSystem;

class DebugDraw
  : public Singleton<DebugDraw>
  , ISystem {
public:
  DebugDraw(RenderSystem *render);

  void Init() override;
  void Shutdown() override;

  void Render();

private:
  RenderSystem *m_RenderSys;
  Buffer *m_Buffer;
  VertexArray *m_VertexArray;
};

#endif // !SN_DEBUG_DRAW_H
