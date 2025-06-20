#ifndef ENTRY_H
#define ENTRY_H

#include "global.h"
#include <memory>

int main(int argc, char **argv) {
  std::unique_ptr<Sono::Global> global = std::make_unique<Sono::Global>();
  global->Init();

  // Create a screen/window with specified dimensions and title
  Window screen(800, 600, "Hello Sono");

  // Enable vertical synchronization
  screen.EnableVsync(true);

  // Main loop to keep the application running
  while (!screen.ShouldClose()) {
    // Process input events
    // screen.ProcessInput();

    // Swap buffers and poll events
    screen.SwapBuffers();
    screen.PollEvents();
  }

  // Shutdown the Sono library
  global->Shutdown();
  return 0;
}

#endif // !ENTRY_H
