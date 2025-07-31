#ifndef SN_GL_COMMON_H

#include <core/common/logger.h>
#include "glad/glad.h"
#include <string>

inline GLenum _GlCheckError(const char *file, int line) {
  GLenum errorCode;
  while ((errorCode = glGetError()) != GL_NO_ERROR) {
    std::string error;
    switch (errorCode) {
      case GL_INVALID_ENUM:
        error = "INVALID_ENUM";
        break;
      case GL_INVALID_VALUE:
        error = "INVALID_VALUE";
        break;
      case GL_INVALID_OPERATION:
        error = "INVALID_OPERATION";
        break;
      // case GL_STACK_OVERFLOW:
      //   error = "STACK_OVERFLOW";
      //   break;
      // case GL_STACK_UNDERFLOW:
      //   error = "STACK_UNDERFLOW";
      //   break;
      case GL_OUT_OF_MEMORY:
        error = "OUT_OF_MEMORY";
        break;
      case GL_INVALID_FRAMEBUFFER_OPERATION:
        error = "INVALID_FRAMEBUFFER_OPERATION";
        break;
    }
    LOG_ERROR_F("%s | %s:%d", error.c_str(), file, line);
  }
  return errorCode;
}

#define GL_CHECK_ERROR() _GlCheckError(__FILE__, __LINE__)

#endif // !SN_GL_COMMON_H
