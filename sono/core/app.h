#ifndef SN_APPLICATION_H
#define SN_APPLICATION_H

#include "render/render_system.h"
#include "render/render_window.h"

namespace Sono {

class App {
public:
  virtual ~App() {}

  virtual void Init() {}

  virtual void Shutdown() {}

  virtual void Update() = 0;

  virtual void OnImGuiFrame() {}

  virtual RenderWindow *CreateWindow(RenderSystem *rs) {
    m_ActiveWindow = rs->CreateRenderWindow(1600, 1000, "Sono Engine");
    return m_ActiveWindow;
  }

  virtual RenderWindow *GetWindow(RenderSystem *rs) { return m_ActiveWindow; }

protected:
  RenderWindow *m_ActiveWindow;
};

} // namespace Sono

#endif // !SN_APPLICATION_H
