#include "sngl/glshader.h"

#include <fstream>
#include <glad/glad.h>
#include <iostream>
#include <sstream>
#include <unordered_map>
#include "../logger.h"

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

    this->m_Id = _Compile(code.c_str(), type);
  } else {
    // TODO: Log this using logger
    this->m_Id = 0;
    std::cout << "Failed to read shader file [TYPE: " << ShaderTypeStr.at(type)
              << "]" << std::endl;
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
      std::cout << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n"
                << infoLog << std::endl;
      break;
    case GL_GEOMETRY_SHADER:
      std::cout << "ERROR::SHADER::GEOMETRY::COMPILATION_FAILED\n"
                << infoLog << std::endl;
      break;
    case GL_FRAGMENT_SHADER:
      std::cout << "ERROR::SHADER::FRAGMENT::COMPILATION_FAILED\n"
                << infoLog << std::endl;
      break;
    }
  }

  return shaderId;
}

u32 GLShader::GetType() { return this->m_Type; }

u32 GLShader::GetId() { return this->m_Id; }

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

void GLShaderProgram::LinkProgram() {
  i32 success;
  char infoLog[512];

  glLinkProgram(this->m_Id);
  glGetProgramiv(this->m_Id, GL_LINK_STATUS, &success);
  if (!success) {
    glGetProgramInfoLog(this->m_Id, 512, NULL, infoLog);
  }
}

void GLShaderProgram::Use() { glUseProgram(this->m_Id); }

GLShaderProgram::operator u32() const { return this->m_Id; }

GLShaderProgram &GLShaderProgram::DefaultPipeLine() {
  static const char *vsSrc = " \
#version 330 core\n\
layout (location = 0) in vec3 aPos;\n\
void main() {\n\
  gl_Position = vec4(aPos.x, aPos.y, aPos.z, 1.0);\n\
}";

  static const char *fsSrc = " \
#version 330 core\n\
out vec4 fragColor;\n\
void main() {\n\
  fragColor = vec4(1.0f, 0.5f, 0.2f, 1.0f);\n\
}";

  static GLShader vsId(&vsSrc, GL_VERTEX_SHADER);
  static GLShader fsId(&fsSrc, GL_FRAGMENT_SHADER);
  static GLShaderProgram program(vsId, fsId);
  return program;
}