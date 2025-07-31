#include "gl_shader.h"
#include "core/common/logger.h"
#include "core/common/snassert.h"
#include "render/shader/shader.h"

#include <glad/glad.h>
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
GLShader::GLShader(const ShaderDesc &desc)
  : Shader(desc)
  , m_ShaderID(0) {
  Compile();
}
// --------------------------------------------------------------------------------
void GLShader::Compile() {
  SN_ASSERT(!m_Desc.src.empty(), "shader sources is empty");

  GLenum type = ShaderStageToOpenGLShader(m_Desc.stage);
  m_ShaderID = glCreateShader(type);

  i32 success;
  char infoLog[512];

  // glShaderSource expected a const char * const pointer
  const char *sources[] = {m_Desc.src.c_str()};
  glShaderSource(m_ShaderID, 1, sources, NULL);
  glCompileShader(m_ShaderID);

  glGetShaderiv(m_ShaderID, GL_COMPILE_STATUS, &success);
  if (!success) {
    glGetShaderInfoLog(m_ShaderID, 512, NULL, infoLog);
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
}
// --------------------------------------------------------------------------------
GLuint GLShader::GetID() const { return m_ShaderID; }
// --------------------------------------------------------------------------------
void GLShader::DeleteShader() { glDeleteShader(m_ShaderID); }
// --------------------------------------------------------------------------------
GLShader::operator GLuint() const { return m_ShaderID; }
// --------------------------------------------------------------------------------
GLShader::~GLShader() { DeleteShader(); }

// ================================================================================
// GLRenderPipeline Implementation
// ================================================================================
GLRenderPipeline::GLRenderPipeline(const PipelineDesc &desc)
  : RenderPipeline(desc) {
  m_ID = glCreateProgram();
  glAttachShader(m_ID, static_cast<GLShader *>(desc.vertex)->GetID());
  glAttachShader(m_ID, static_cast<GLShader *>(desc.fragment)->GetID());

  i32 success;
  char infoLog[512];

  glLinkProgram(m_ID);
  glGetProgramiv(m_ID, GL_LINK_STATUS, &success);
  if (!success) {
    glGetProgramInfoLog(m_ID, 512, NULL, infoLog);
  }

  // TODO: remove these as soon as the resources system established
  ((GLShader *)m_Desc.vertex)->DeleteShader();
  ((GLShader *)m_Desc.fragment)->DeleteShader();
}
// --------------------------------------------------------------------------------
void GLRenderPipeline::SetUniform(const char *uniform, const Vec4 &v4) const {
  ASSERT_CURRENT_PROGRAM(this);
  glUniform4fv(glGetUniformLocation(m_ID, uniform), 1, v4.ValuePtr());
}
// --------------------------------------------------------------------------------
void GLRenderPipeline::SetUniform(const char *uniform, const Vec3 &v3) const {
  ASSERT_CURRENT_PROGRAM(this);
  glUniform3fv(glGetUniformLocation(m_ID, uniform), 1, v3.ValuePtr());
}
// --------------------------------------------------------------------------------
void GLRenderPipeline::SetUniform(const char *uniform, const Vec2 &v2) const {
  ASSERT_CURRENT_PROGRAM(this);
  glUniform2fv(glGetUniformLocation(m_ID, uniform), 1, v2.ValuePtr());
}
// --------------------------------------------------------------------------------
void GLRenderPipeline::SetUniform(const char *uniform, f32 x, f32 y, f32 z, f32 w) const {
  ASSERT_CURRENT_PROGRAM(this);
  glUniform4f(glGetUniformLocation(m_ID, uniform), x, y, z, w);
}
// --------------------------------------------------------------------------------
void GLRenderPipeline::SetUniform(const char *uniform, f32 x, f32 y, f32 z) const {
  ASSERT_CURRENT_PROGRAM(this);
  glUniform3f(glGetUniformLocation(m_ID, uniform), x, y, z);
}
// --------------------------------------------------------------------------------
void GLRenderPipeline::SetUniform(const char *uniform, f32 x, f32 y) const {
  ASSERT_CURRENT_PROGRAM(this);
  glUniform2f(glGetUniformLocation(m_ID, uniform), x, y);
}
// --------------------------------------------------------------------------------
void GLRenderPipeline::SetUniform(const char *uniform, f32 v) const {
  ASSERT_CURRENT_PROGRAM(this);
  glUniform1f(glGetUniformLocation(m_ID, uniform), v);
}
// --------------------------------------------------------------------------------
void GLRenderPipeline::SetUniform(const char *uniform, i32 v) const {
  ASSERT_CURRENT_PROGRAM(this);
  glUniform1i(glGetUniformLocation(m_ID, uniform), v);
}
// --------------------------------------------------------------------------------
void GLRenderPipeline::SetUniform(const char *uniform, const MatBase<4, 4, f32> &mat4) const {
  ASSERT_CURRENT_PROGRAM(this);
  glUniformMatrix4fv(glGetUniformLocation(m_ID, uniform), 1, GL_FALSE, mat4.ValuePtr());
}
// --------------------------------------------------------------------------------
void GLRenderPipeline::SetUniform(const char *uniform, const MatBase<4, 3, f32> &mat4x3) const {
  ASSERT_CURRENT_PROGRAM(this);
  glUniformMatrix4x3fv(glGetUniformLocation(m_ID, uniform), 1, GL_FALSE, mat4x3.ValuePtr());
}
// --------------------------------------------------------------------------------
void GLRenderPipeline::SetUniform(const char *uniform, const MatBase<4, 2, f32> &mat4x2) const {
  ASSERT_CURRENT_PROGRAM(this);
  glUniformMatrix4x2fv(glGetUniformLocation(m_ID, uniform), 1, GL_FALSE, mat4x2.ValuePtr());
}
// --------------------------------------------------------------------------------
void GLRenderPipeline::SetUniform(const char *uniform, const MatBase<3, 4, f32> &mat3x4) const {
  ASSERT_CURRENT_PROGRAM(this);
  glUniformMatrix3x4fv(glGetUniformLocation(m_ID, uniform), 1, GL_FALSE, mat3x4.ValuePtr());
}
// --------------------------------------------------------------------------------
void GLRenderPipeline::SetUniform(const char *uniform, const Mat3 &mat3) const {
  ASSERT_CURRENT_PROGRAM(this);
  glUniformMatrix3fv(glGetUniformLocation(m_ID, uniform), 1, GL_FALSE, mat3.ValuePtr());
}
// --------------------------------------------------------------------------------
void GLRenderPipeline::SetUniform(const char *uniform, const MatBase<3, 2, f32> &mat3x2) const {
  ASSERT_CURRENT_PROGRAM(this);
  glUniformMatrix3x2fv(glGetUniformLocation(m_ID, uniform), 1, GL_FALSE, mat3x2.ValuePtr());
}
// --------------------------------------------------------------------------------
void GLRenderPipeline::SetUniform(const char *uniform, const MatBase<2, 2, f32> &mat2) const {
  ASSERT_CURRENT_PROGRAM(this);
  glUniformMatrix2fv(glGetUniformLocation(m_ID, uniform), 1, GL_FALSE, mat2.ValuePtr());
}
// --------------------------------------------------------------------------------
void GLRenderPipeline::SetUniform(const char *uniform, const MatBase<2, 4, f32> &mat2x4) const {
  ASSERT_CURRENT_PROGRAM(this);
  glUniformMatrix2x4fv(glGetUniformLocation(m_ID, uniform), 1, GL_FALSE, mat2x4.ValuePtr());
}
// --------------------------------------------------------------------------------
void GLRenderPipeline::SetUniform(const char *uniform, const MatBase<2, 3, f32> &mat2x3) const {
  ASSERT_CURRENT_PROGRAM(this);
  glUniformMatrix2x3fv(glGetUniformLocation(m_ID, uniform), 1, GL_FALSE, mat2x3.ValuePtr());
}
// --------------------------------------------------------------------------------
void GLRenderPipeline::SetVec4(const char *uniform, f32 x, f32 y, f32 z, f32 w) const {
  ASSERT_CURRENT_PROGRAM(this);
  glUniform4f(glGetUniformLocation(m_ID, uniform), x, y, z, w);
}
// --------------------------------------------------------------------------------
void GLRenderPipeline::SetVec4(const char *uniform, const Vec4 &v) const {
  ASSERT_CURRENT_PROGRAM(this);
  glUniform4fv(glGetUniformLocation(m_ID, uniform), 1, v.ValuePtr());
}
// --------------------------------------------------------------------------------
void GLRenderPipeline::SetVec3(const char *uniform, f32 x, f32 y, f32 z) const {
  ASSERT_CURRENT_PROGRAM(this);
  glUniform3f(glGetUniformLocation(m_ID, uniform), x, y, z);
}
// --------------------------------------------------------------------------------
void GLRenderPipeline::SetVec3(const char *uniform, const Vec3 &v) const {
  ASSERT_CURRENT_PROGRAM(this);
  glUniform3f(glGetUniformLocation(m_ID, uniform), v.x, v.y, v.z);
}
// --------------------------------------------------------------------------------
void GLRenderPipeline::SetVec2(const char *uniform, f32 x, f32 y) const {
  ASSERT_CURRENT_PROGRAM(this);
  glUniform2f(glGetUniformLocation(m_ID, uniform), x, y);
}
// --------------------------------------------------------------------------------
void GLRenderPipeline::SetVec2(const char *uniform, const Vec2 &v) const {
  ASSERT_CURRENT_PROGRAM(this);
  glUniform2f(glGetUniformLocation(m_ID, uniform), v.x, v.y);
}
// --------------------------------------------------------------------------------
void GLRenderPipeline::SetFloat(const char *uniform, f32 v) const {
  ASSERT_CURRENT_PROGRAM(this);
  glUniform1f(glGetUniformLocation(m_ID, uniform), v);
}
// --------------------------------------------------------------------------------
void GLRenderPipeline::SetBool(const char *uniform, b8 v) const {
  ASSERT_CURRENT_PROGRAM(this);
  glUniform1i(glGetUniformLocation(m_ID, uniform), v);
}
// --------------------------------------------------------------------------------
void GLRenderPipeline::SetInt(const char *uniform, i32 v) const {
  ASSERT_CURRENT_PROGRAM(this);
  glUniform1i(glGetUniformLocation(m_ID, uniform), v);
}
// --------------------------------------------------------------------------------
void GLRenderPipeline::SetMat4(const char *uniform, f32 *mat4) const {
  ASSERT_CURRENT_PROGRAM(this);
  glUniformMatrix4fv(glGetUniformLocation(m_ID, uniform), 1, GL_FALSE, mat4);
}
// --------------------------------------------------------------------------------
void GLRenderPipeline::SetMat4(const char *uniform, const Mat4 &mat4) const {
  ASSERT_CURRENT_PROGRAM(this);
  glUniformMatrix4fv(glGetUniformLocation(m_ID, uniform), 1, GL_FALSE, mat4.ValuePtr());
}
// --------------------------------------------------------------------------------
void GLRenderPipeline::SetMat4x3(const char *uniform, f32 *mat4x3) const {
  ASSERT_CURRENT_PROGRAM(this);
  glUniformMatrix4x3fv(glGetUniformLocation(m_ID, uniform), 1, GL_FALSE, mat4x3);
}
// --------------------------------------------------------------------------------
void GLRenderPipeline::SetMat4x2(const char *uniform, f32 *mat4x2) const {
  ASSERT_CURRENT_PROGRAM(this);
  glUniformMatrix4x2fv(glGetUniformLocation(m_ID, uniform), 1, GL_FALSE, mat4x2);
}
// --------------------------------------------------------------------------------
void GLRenderPipeline::SetMat3x4(const char *uniform, f32 *mat3x4) const {
  ASSERT_CURRENT_PROGRAM(this);
  glUniformMatrix3x4fv(glGetUniformLocation(m_ID, uniform), 1, GL_FALSE, mat3x4);
}
// --------------------------------------------------------------------------------
void GLRenderPipeline::SetMat3(const char *uniform, f32 *mat3) const {
  ASSERT_CURRENT_PROGRAM(this);
  glUniformMatrix3fv(glGetUniformLocation(m_ID, uniform), 1, GL_FALSE, mat3);
}
// --------------------------------------------------------------------------------
void GLRenderPipeline::SetMat3(const char *uniform, const Mat3 &mat3) const {
  ASSERT_CURRENT_PROGRAM(this);
  glUniformMatrix3fv(glGetUniformLocation(m_ID, uniform), 1, GL_FALSE, mat3.ValuePtr());
}
// --------------------------------------------------------------------------------
void GLRenderPipeline::SetMat3x2(const char *uniform, f32 *mat3x2) const {
  ASSERT_CURRENT_PROGRAM(this);
  glUniformMatrix3x2fv(glGetUniformLocation(m_ID, uniform), 1, GL_FALSE, mat3x2);
}
// --------------------------------------------------------------------------------
void GLRenderPipeline::SetMat2(const char *uniform, f32 *mat2) const {
  ASSERT_CURRENT_PROGRAM(this);
  glUniformMatrix2fv(glGetUniformLocation(m_ID, uniform), 1, GL_FALSE, mat2);
}
// --------------------------------------------------------------------------------
void GLRenderPipeline::SetMat2x4(const char *uniform, f32 *mat2x4) const {
  ASSERT_CURRENT_PROGRAM(this);
  glUniformMatrix2x4fv(glGetUniformLocation(m_ID, uniform), 1, GL_FALSE, mat2x4);
}
// --------------------------------------------------------------------------------
void GLRenderPipeline::SetMat2x3(const char *uniform, f32 *mat2x3) const {
  ASSERT_CURRENT_PROGRAM(this);
  glUniformMatrix2x3fv(glGetUniformLocation(m_ID, uniform), 1, GL_FALSE, mat2x3);
}
// --------------------------------------------------------------------------------
void GLRenderPipeline::Bind() const { glUseProgram(m_ID); }
// --------------------------------------------------------------------------------
void GLRenderPipeline::Unbind() const { glUseProgram(0); }
// --------------------------------------------------------------------------------
GLuint GLRenderPipeline::GetID() const { return m_ID; }
// --------------------------------------------------------------------------------
GLRenderPipeline::operator GLuint() const { return m_ID; }
// --------------------------------------------------------------------------------
GLRenderPipeline::~GLRenderPipeline() { glDeleteProgram(m_ID); }
