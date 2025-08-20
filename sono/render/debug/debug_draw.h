#ifndef SN_DEBUG_DRAW_H
#define SN_DEBUG_DRAW_H

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

private:
  RenderSystem *m_RenderSys;
};

template <>
inline DebugDraw *Singleton<DebugDraw>::m_sInstance = nullptr;

#endif // !SN_DEBUG_DRAW_H
