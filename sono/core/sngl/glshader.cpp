#include "glshader.h"
#include "../common/logger.h"
#include "../common/snassert.h"

#include <fstream>
#include <glad/glad.h>
#include <iostream>
#include <sstream>
#include <unordered_map>

#ifndef NDEBUG
#define ASSERT_CURRENT_PROGRAM(program)                                        \
  do {                                                                         \
    GLint _currentProgram = 0;                                                 \
    glGetIntegerv(GL_CURRENT_PROGRAM, &_currentProgram);                       \
    SN_ASSERT_F(                                                               \
      (program).GetId() == _currentProgram,                                    \
      "Erroneous usage of shader program (current: %d, expected: %d)\n"        \
      "Did you forget to call GLShaderProgram::Use()?",                        \
      _currentProgram,                                                         \
      (program).GetId()                                                        \
    );                                                                         \
  } while (0)

#else
#define ASSERT_CURRENT_PROGRAM(program)
#endif

const std::unordered_map<u32, const char *> ShaderTypeStr = {
  {GL_VERTEX_SHADER,   "GL_VERTEX_SHADER"  },
  {GL_FRAGMENT_SHADER, "GL_FRAGMENT_SHADER"},
  {GL_GEOMETRY_SHADER, "GL_GEOMETRY_SHADER"}
};

GLShader::GLShader(const char **src, u32 type)
  : m_Type(type) {
  this->m_Id = _Compile(*src, type);
  this->m_Type = type;
}

GLShader::GLShader(const char *srcPath, u32 type)
  : m_Type(type) {
  std::ifstream srcFile(srcPath);
  std::stringstream srcStream;
  std::string code;

  if (srcFile.is_open()) {
    srcStream << srcFile.rdbuf();
    srcFile.close();

    code = srcStream.str();

    LOG_DEBUG_F("Compiling shader from file: %s", srcPath);
    this->m_Id = _Compile(code.c_str(), type);
  } else {
    LOG_ERROR_F("Failed to open shader file: %s", srcPath);
    this->m_Id = 0;
  }
}

GLShader::~GLShader() { DeleteShader(); }

u32 GLShader::_Compile(const char *src, u32 type) {
  u32 shaderId = glCreateShader(type);
  i32 success;
  char infoLog[512];

  const char *sources[1] = {src};

  glShaderSource(shaderId, 1, sources, NULL);
  glCompileShader(shaderId);

  glGetShaderiv(shaderId, GL_COMPILE_STATUS, &success);

  if (!success) {
    glGetShaderInfoLog(shaderId, 512, NULL, infoLog);
    switch (type) {
    case GL_VERTEX_SHADER:
      LOG_ERROR_F("Vertex shader compilation failed: %s", infoLog);
      break;
    case GL_GEOMETRY_SHADER:
      LOG_ERROR_F("Geometry shader compilation failed: %s", infoLog);
      break;
    case GL_FRAGMENT_SHADER:
      LOG_ERROR_F("Fragment shader compilation failed: %s", infoLog);
      break;
    }
  } else {
    LOG_INFO_F("%s compilation success", ShaderTypeStr.at(type));
  }

  return shaderId;
}

u32 GLShader::GetType() const { return this->m_Type; }

u32 GLShader::GetId() const { return this->m_Id; }

void GLShader::DeleteShader() { glDeleteShader(this->m_Id); }

GLShader::operator u32() const { return this->m_Id; }

GLShaderProgram::GLShaderProgram(GLShader &s1, GLShader &s2) {
  this->m_Id = glCreateProgram();
  AttachShader(s1);
  AttachShader(s2);
  LinkProgram();
}

GLShaderProgram::~GLShaderProgram() { glDeleteProgram(this->m_Id); }

void GLShaderProgram::AttachShader(GLShader &shader) {
  glAttachShader(this->m_Id, shader);

  switch (shader.GetType()) {
  case GL_VERTEX_SHADER: {
    if ((m_AttachedShaders & VERTEX_SHADER_BIT) != 0)
      LOG_ERROR("Reattaching shader of type GL_VERTEX_SHADER");
    m_AttachedShaders |= VERTEX_SHADER_BIT;
    break;
  }
  case GL_FRAGMENT_SHADER:
    if ((m_AttachedShaders & FRAGMENT_SHADER_BIT) != 0)
      LOG_ERROR("Reattaching shader of type GL_FRAGMENT_SHADER");
    m_AttachedShaders |= FRAGMENT_SHADER_BIT;
    break;
  case GL_GEOMETRY_SHADER:
    if ((m_AttachedShaders & GEOMETRY_SHADER_BIT) != 0)
      LOG_ERROR("Reattaching shader of type GL_GEOMETRY_SHADER");
    m_AttachedShaders |= GEOMETRY_SHADER_BIT;
    break;
  }
}

void GLShaderProgram::SetVec4(
  const char *uniform, f32 x, f32 y, f32 z, f32 w
) const {
  ASSERT_CURRENT_PROGRAM(*this);
  glUniform4f(glGetUniformLocation(this->m_Id, uniform), x, y, z, w);
}

void GLShaderProgram::SetVec3(const char *uniform, f32 x, f32 y, f32 z) const {
  ASSERT_CURRENT_PROGRAM(*this);
  glUniform3f(glGetUniformLocation(this->m_Id, uniform), x, y, z);
}

void GLShaderProgram::SetVec2(const char *uniform, f32 x, f32 y) const {
  ASSERT_CURRENT_PROGRAM(*this);
  glUniform2f(glGetUniformLocation(this->m_Id, uniform), x, y);
}

void GLShaderProgram::SetFloat(const char *uniform, f32 v) const {
  ASSERT_CURRENT_PROGRAM(*this);
  glUniform1f(glGetUniformLocation(this->m_Id, uniform), v);
}

void GLShaderProgram::SetBool(const char *uniform, u8 v) const {
  ASSERT_CURRENT_PROGRAM(*this);
  glUniform1i(glGetUniformLocation(this->m_Id, uniform), v);
}

void GLShaderProgram::SetInt(const char *uniform, i32 v) const {
  ASSERT_CURRENT_PROGRAM(*this);
  glUniform1i(glGetUniformLocation(this->m_Id, uniform), v);
}

void GLShaderProgram::LinkProgram() const {
  i32 success;
  char infoLog[512];

  glLinkProgram(this->m_Id);
  glGetProgramiv(this->m_Id, GL_LINK_STATUS, &success);
  if (!success) {
    glGetProgramInfoLog(this->m_Id, 512, NULL, infoLog);
  }
}

void GLShaderProgram::Use() const { glUseProgram(this->m_Id); }

u32 GLShaderProgram::GetId() const { return this->m_Id; }

GLShaderProgram::operator u32() const { return this->m_Id; }

GLShaderProgram &GLShaderProgram::DefaultPipeLine() {
  static GLShader vsId("./assets/vertex.glsl", GL_VERTEX_SHADER);
  static GLShader fsId("./assets/fragment.glsl", GL_FRAGMENT_SHADER);
  static GLShaderProgram program(vsId, fsId);
  return program;
}