#ifndef ENTRY_H
#define ENTRY_H

#include <core/app.h>
#include <core/common/logger.h>
#include <core/common/time.h>
#include <core/global.h>
#include <core/math/transform.h>
#include <core/math/vec3.h>
#include <core/event/event_dispatcher.h>
#include <core/input/mouse.h>
#include <render/render_command.h>

inline void HandleEvent(const Event &ev) {
  // S_LOG_TRACE(ev.ToString());
  static InputSystem *is = InputSystem::GetPtr();
  static RenderSystem *rs = RenderSystem::GetPtr();

  switch (ev.type) {
    case EventType::KEY:
    case EventType::MOUSE_MOVE:
    case EventType::MOUSE_BUTTON:
    case EventType::MOUSE_SCROLL:
      is->InjectEvent(ev);
      break;
    case EventType::WINDOW_RESIZE: {
      auto &k = std::get<WindowResizeEvent>(ev.payload);
      rs->SetViewport(0, 0, k.width, k.height);
      break;
    }
    case EventType::TEXT:
    case EventType::QUIT:
      break;
  }
}

#define SONO_IMPLEMENT_MAIN(AppType, ...)                                                          \
  i32 main(void) {                                                                                 \
    std::unique_ptr<Sono::Global> global = std::make_unique<Sono::Global>();                       \
    global->Init();                                                                                \
    MemorySystem *ms = MemorySystem::GetPtr();                                                     \
    RenderSystem *rs = RenderSystem::GetPtr();                                                     \
    EventSystem *es = EventSystem::GetPtr();                                                       \
    InputSystem *is = InputSystem::GetPtr();                                                       \
    AppType app = AppType(__VA_ARGS__);                                                            \
    RenderWindow *win = app.CreateWindow(rs);                                                      \
    app.Init();                                                                                    \
    rs->InitImGui(win);                                                                            \
    {                                                                                              \
      PROFILE_SCOPE("MAIN_LOOP");                                                                  \
      while (!win->ShouldClose()) {                                                                \
        /* Poll for and process events */                                                          \
        {                                                                                          \
          PROFILE_SCOPE("MAIN_LOOP::Handle Events");                                               \
          glfwPollEvents();                                                                        \
          while (auto *ev = es->Pop()) {                                                           \
            HandleEvent(*ev);                                                                      \
            EventDispatcher::Dispatch(*ev);                                                        \
          }                                                                                        \
        }                                                                                          \
        {                                                                                          \
          PROFILE_SCOPE("MAIN_LOOP::RENDER::RenderOneFrame");                                      \
          app.Update();                                                                            \
          rs->BeginImGuiFrame();                                                                   \
          app.OnImGuiFrame();                                                                      \
          rs->EndImGuiFrame();                                                                     \
          rs->EndFrame();                                                                          \
        }                                                                                          \
        is->EndFrame();                                                                            \
        Time::Tick();                                                                              \
      }                                                                                            \
    }                                                                                              \
    rs->ShutdownImGui();                                                                           \
    global->Shutdown();                                                                            \
    return 0;                                                                                      \
  }

#endif // !ENTRY_H
