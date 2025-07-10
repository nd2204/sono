#include "gl_shader.h"
#include "core/common/logger.h"
#include "core/common/snassert.h"

#include <cstdint>
#include <fstream>
#include <glad/glad.h>
#include <iostream>
#include <sstream>
#include <unordered_map>

#ifdef SN_NDEBUG
#define ASSERT_CURRENT_PROGRAM(program)
#else
#define ASSERT_CURRENT_PROGRAM(program)                                                            \
  do {                                                                                             \
    GLint _currentProgram = 0;                                                                     \
    glGetIntegerv(GL_CURRENT_PROGRAM, &_currentProgram);                                           \
    SN_ASSERT_F(                                                                                   \
      (program)->GetID() == (u32)_currentProgram,                                                  \
      "Erroneous usage of shader program (current: %d, expected: %d)\n"                            \
      "Did you forget to call GLRenderPipeline::Use()?",                                           \
      _currentProgram, (program)->GetID()                                                          \
    );                                                                                             \
  } while (0)
#endif

const std::unordered_map<u32, const char *> ShaderTypeStr = {
  {GL_VERTEX_SHADER,   "GL_VERTEX_SHADER"  },
  {GL_FRAGMENT_SHADER, "GL_FRAGMENT_SHADER"},
  {GL_GEOMETRY_SHADER, "GL_GEOMETRY_SHADER"}
};

// clang-format off
static inline GLenum ShaderStageToOpenGLShader(ShaderStage stage) {
  switch (stage) {
  case ShaderStage::VERTEX:      return GL_VERTEX_SHADER;
  case ShaderStage::FRAGMENT:    return GL_FRAGMENT_SHADER;
  case ShaderStage::GEOMETRY:    return GL_GEOMETRY_SHADER;
  case ShaderStage::COMPUTE:     SN_ASSERT(false, "ShaderStage::COMPUTE is not supported on OpenGL"); break;
  case ShaderStage::TESSELATION: SN_ASSERT(false, "ShaderStage::TESSELATION is not supported on OpenGL"); break;
  }
  return 0;
}
// clang-format on

// ================================================================================
// GLShader Implementation
// ================================================================================
GLShader::GLShader()
  : m_ShaderID(0) {}
// --------------------------------------------------------------------------------
GLShader::GLShader(const ShaderDesc *desc)
  : m_ShaderID(0) {
  if (!desc) return;
  m_Stage = desc->stage;
  if (!desc->src.empty()) {
    m_Src = desc->src;
    m_ShaderID = _Compile(desc->src.c_str(), ShaderStageToOpenGLShader(desc->stage));
  }
}
// --------------------------------------------------------------------------------
void GLShader::CompileFromSrc(const std::string &src, ShaderStage stage) {
  this->m_ShaderID = _Compile(src.c_str(), ShaderStageToOpenGLShader(stage));
  m_Stage = stage;
}
// --------------------------------------------------------------------------------
void GLShader::CompileFromFile(const std::string &filePath, ShaderStage stage) {
  std::ifstream srcFile(filePath);
  std::stringstream srcStream;
  std::string code;

  m_Stage = stage;

  if (srcFile.is_open()) {
    srcStream << srcFile.rdbuf();
    srcFile.close();

    code = srcStream.str();

    LOG_DEBUG_F("Compiling shader from file: %s", filePath.c_str());
    this->m_ShaderID = _Compile(code.c_str(), ShaderStageToOpenGLShader(stage));
  } else {
    LOG_ERROR_F("Failed to open shader file: %s", filePath.c_str());
    this->m_ShaderID = 0;
  }
}
// --------------------------------------------------------------------------------
GLuint GLShader::GetID() const { return m_ShaderID; }
// --------------------------------------------------------------------------------
ShaderStage GLShader::GetStage() const { return m_Stage; }
// --------------------------------------------------------------------------------
void GLShader::DeleteShader() { glDeleteShader(m_ShaderID); }
// --------------------------------------------------------------------------------
GLShader::operator GLuint() const { return m_ShaderID; }
// --------------------------------------------------------------------------------
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
// --------------------------------------------------------------------------------
GLShader::~GLShader() { DeleteShader(); }

// ================================================================================
// GLRenderPipeline Implementation
// ================================================================================
GLRenderPipeline::GLRenderPipeline(Shader *s1, Shader *s2) {
  this->m_ID = glCreateProgram();
  AttachShader(s1);
  AttachShader(s2);
  LinkProgram();
}
// --------------------------------------------------------------------------------
void GLRenderPipeline::AttachShader(Shader *shader) {
  glAttachShader(this->m_ID, static_cast<GLShader *>(shader)->GetID());

  switch (shader->GetStage()) {
  case ShaderStage::VERTEX: {
    if ((m_AttachedShaders & VERTEX_SHADER_BIT) != 0)
      LOG_ERROR("Reattaching shader of type GL_VERTEX_SHADER");
    m_AttachedShaders |= VERTEX_SHADER_BIT;
    m_VertexShader = shader;
    break;
  }
  case ShaderStage::FRAGMENT:
    if ((m_AttachedShaders & FRAGMENT_SHADER_BIT) != 0)
      LOG_ERROR("Reattaching shader of type GL_FRAGMENT_SHADER");
    m_AttachedShaders |= FRAGMENT_SHADER_BIT;
    m_FragmentShader = shader;
    break;
  case ShaderStage::GEOMETRY:
    if ((m_AttachedShaders & GEOMETRY_SHADER_BIT) != 0)
      LOG_ERROR("Reattaching shader of type GL_GEOMETRY_SHADER");
    m_AttachedShaders |= GEOMETRY_SHADER_BIT;
    break;
  case ShaderStage::COMPUTE:
    SN_ASSERT(false, "ShaderStage::COMPUTE is not supported on OpenGL");
    break;
  case ShaderStage::TESSELATION:
    SN_ASSERT(false, "ShaderStage::TESSELATION is not supported on OpenGL");
    break;
  }
}
// --------------------------------------------------------------------------------
void GLRenderPipeline::SetVec4(const char *uniform, f32 x, f32 y, f32 z, f32 w) const {
  ASSERT_CURRENT_PROGRAM(this);
  glUniform4f(glGetUniformLocation(this->m_ID, uniform), x, y, z, w);
}
// --------------------------------------------------------------------------------
void GLRenderPipeline::SetVec4(const char *uniform, const Vec4 &v) const {
  ASSERT_CURRENT_PROGRAM(this);
  glUniform4f(glGetUniformLocation(this->m_ID, uniform), v.x, v.y, v.z, v.w);
}
// --------------------------------------------------------------------------------
void GLRenderPipeline::SetVec3(const char *uniform, f32 x, f32 y, f32 z) const {
  ASSERT_CURRENT_PROGRAM(this);
  glUniform3f(glGetUniformLocation(this->m_ID, uniform), x, y, z);
}
// --------------------------------------------------------------------------------
void GLRenderPipeline::SetVec3(const char *uniform, const Vec3 &v) const {
  ASSERT_CURRENT_PROGRAM(this);
  glUniform3f(glGetUniformLocation(this->m_ID, uniform), v.x, v.y, v.z);
}
// --------------------------------------------------------------------------------
void GLRenderPipeline::SetVec2(const char *uniform, f32 x, f32 y) const {
  ASSERT_CURRENT_PROGRAM(this);
  glUniform2f(glGetUniformLocation(this->m_ID, uniform), x, y);
}
// --------------------------------------------------------------------------------
void GLRenderPipeline::SetVec2(const char *uniform, const Vec2 &v) const {
  ASSERT_CURRENT_PROGRAM(this);
  glUniform2f(glGetUniformLocation(this->m_ID, uniform), v.x, v.y);
}
// --------------------------------------------------------------------------------
void GLRenderPipeline::SetFloat(const char *uniform, f32 v) const {
  ASSERT_CURRENT_PROGRAM(this);
  glUniform1f(glGetUniformLocation(this->m_ID, uniform), v);
}
// --------------------------------------------------------------------------------
void GLRenderPipeline::SetBool(const char *uniform, b8 v) const {
  ASSERT_CURRENT_PROGRAM(this);
  glUniform1i(glGetUniformLocation(this->m_ID, uniform), v);
}
// --------------------------------------------------------------------------------
void GLRenderPipeline::SetInt(const char *uniform, i32 v) const {
  ASSERT_CURRENT_PROGRAM(this);
  glUniform1i(glGetUniformLocation(this->m_ID, uniform), v);
}
// --------------------------------------------------------------------------------
void GLRenderPipeline::SetMat4(const char *uniform, f32 *mat4) const {
  ASSERT_CURRENT_PROGRAM(this);
  glUniformMatrix4fv(glGetUniformLocation(this->m_ID, uniform), 1, GL_FALSE, mat4);
}
// --------------------------------------------------------------------------------
void GLRenderPipeline::SetMat4(const char *uniform, const Mat4 &mat4) const {
  ASSERT_CURRENT_PROGRAM(this);
  glUniformMatrix4fv(glGetUniformLocation(this->m_ID, uniform), 1, GL_FALSE, mat4.ValuePtr());
}
// --------------------------------------------------------------------------------
void GLRenderPipeline::SetMat4x3(const char *uniform, f32 *mat4x3) const {
  ASSERT_CURRENT_PROGRAM(this);
  glUniformMatrix4x3fv(glGetUniformLocation(this->m_ID, uniform), 1, GL_FALSE, mat4x3);
}
// --------------------------------------------------------------------------------
void GLRenderPipeline::SetMat4x2(const char *uniform, f32 *mat4x2) const {
  ASSERT_CURRENT_PROGRAM(this);
  glUniformMatrix4x2fv(glGetUniformLocation(this->m_ID, uniform), 1, GL_FALSE, mat4x2);
}
// --------------------------------------------------------------------------------
void GLRenderPipeline::SetMat3x4(const char *uniform, f32 *mat3x4) const {
  ASSERT_CURRENT_PROGRAM(this);
  glUniformMatrix3x4fv(glGetUniformLocation(this->m_ID, uniform), 1, GL_FALSE, mat3x4);
}
// --------------------------------------------------------------------------------
void GLRenderPipeline::SetMat3(const char *uniform, f32 *mat3) const {
  ASSERT_CURRENT_PROGRAM(this);
  glUniformMatrix3fv(glGetUniformLocation(this->m_ID, uniform), 1, GL_FALSE, mat3);
}
// --------------------------------------------------------------------------------
void GLRenderPipeline::SetMat3(const char *uniform, const Mat3 &mat3) const {
  ASSERT_CURRENT_PROGRAM(this);
  glUniformMatrix3fv(glGetUniformLocation(this->m_ID, uniform), 1, GL_FALSE, mat3.ValuePtr());
}
// --------------------------------------------------------------------------------
void GLRenderPipeline::SetMat3x2(const char *uniform, f32 *mat3x2) const {
  ASSERT_CURRENT_PROGRAM(this);
  glUniformMatrix3x2fv(glGetUniformLocation(this->m_ID, uniform), 1, GL_FALSE, mat3x2);
}
// --------------------------------------------------------------------------------
void GLRenderPipeline::SetMat2(const char *uniform, f32 *mat2) const {
  ASSERT_CURRENT_PROGRAM(this);
  glUniformMatrix2fv(glGetUniformLocation(this->m_ID, uniform), 1, GL_FALSE, mat2);
}
// --------------------------------------------------------------------------------
void GLRenderPipeline::SetMat2x4(const char *uniform, f32 *mat2x4) const {
  ASSERT_CURRENT_PROGRAM(this);
  glUniformMatrix2x4fv(glGetUniformLocation(this->m_ID, uniform), 1, GL_FALSE, mat2x4);
}
// --------------------------------------------------------------------------------
void GLRenderPipeline::SetMat2x3(const char *uniform, f32 *mat2x3) const {
  ASSERT_CURRENT_PROGRAM(this);
  glUniformMatrix2x3fv(glGetUniformLocation(this->m_ID, uniform), 1, GL_FALSE, mat2x3);
}
// --------------------------------------------------------------------------------
void GLRenderPipeline::Bind() const { glUseProgram(this->m_ID); }
// --------------------------------------------------------------------------------
void GLRenderPipeline::Unbind() const { glUseProgram(0); }
// --------------------------------------------------------------------------------
GLuint GLRenderPipeline::GetID() const { return this->m_ID; }
// --------------------------------------------------------------------------------
GLRenderPipeline::operator GLuint() const { return this->m_ID; }
// --------------------------------------------------------------------------------
void GLRenderPipeline::LinkProgram() const {
  i32 success;
  char infoLog[512];

  glLinkProgram(this->m_ID);
  glGetProgramiv(this->m_ID, GL_LINK_STATUS, &success);
  if (!success) {
    glGetProgramInfoLog(this->m_ID, 512, NULL, infoLog);
  }
}
// --------------------------------------------------------------------------------
GLRenderPipeline::~GLRenderPipeline() { glDeleteProgram(this->m_ID); }
