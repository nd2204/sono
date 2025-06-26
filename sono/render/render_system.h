#ifndef SN_RENDER_SYSTEM_H
#define SN_RENDER_SYSTEM_H

#include "core/common/singleton.h"
#include "render_context.h"
#include "buffer_manager.h"
#include "snwindow.h"

class RenderSystem : public Singleton<RenderSystem> {
public:
  RenderSystem();

  virtual ~RenderSystem();

  virtual void Init() = 0;

  virtual void Shutdown() = 0;

  virtual Window *CreateRenderWindow(
    i32 width, i32 height, const char *Title, WindowMode mode = WIN_MODE_WINDOWED
  );

  virtual void SetRenderContext(RenderContext *ctx) = 0;

  virtual void BeginFrame() = 0;

  virtual void EndFrame() = 0;

protected:
  RenderContext *m_pRenderCtx;
  BufferManager *m_BufferManager;
};

#endif // !SN_RENDER_SYSTEM_H
