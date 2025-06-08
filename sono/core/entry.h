#ifndef ENTRY_H
#define ENTRY_H

#include "snscreen.h"
#include <cstdlib>

int main(int argc, char **argv) {
  // Initialize the SonoEngine library
  if (!glfwInit())
    exit(EXIT_FAILURE);

  glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
  glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
  glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
#ifdef MACOS_BUILD
  glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif

  // Create a screen/window with specified dimensions and title
  Sono::SNScreen screen(800, 600, "Hello Sono");

  // Enable vertical synchronization
  screen.EnableVsync(true);

  // Set a callback for framebuffer size changes
  screen.SetFrameBufferSizeCallback(
    [](Sono::SNScreen &window, i32 width, i32 height) {
      glViewport(0, 0, width, height);
    }
  );

  // Main loop to keep the application running
  while (!screen.ShouldClose()) {
    // Process input events
    // screen.ProcessInput();

    // Swap buffers and poll events
    screen.SwapBuffers();
    screen.PollEvents();
  }

  // Shutdown the Sono library
  // Sono::Shutdown();

  return 0;
}

#endif // !ENTRY_H