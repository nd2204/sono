#include <render-backend/sngl/gl_common.h>
#include <render-backend/sngl/gl_render_pipeline.h>
#include <render-backend/sngl/gl_shader.h>

#include <format>
#include <string>

#ifdef NDEBUG
#define ASSERT_CURRENT_PROGRAM(program)
#else
#define ASSERT_CURRENT_PROGRAM(program)                                                            \
  do {                                                                                             \
    GLint _currentProgram = 0;                                                                     \
    glGetIntegerv(GL_CURRENT_PROGRAM, &_currentProgram);                                           \
    SN_ASSERT_F(                                                                                   \
      (program)->GetID() == (u32)_currentProgram,                                                  \
      "In GLRenderPipeline::%s\n"                                                                  \
      "Erroneous usage of shader program (current: %d, expected: %d)\n"                            \
      "A previous call to GLRenderPipeline::Use() is required",                                    \
      __FUNCTION__, _currentProgram, (program)->GetID()                                            \
    );                                                                                             \
  } while (0)
#endif

GLRenderPipeline::GLRenderPipeline(const PipelineDesc &desc)
  : RenderPipeline(desc) {
  m_ID = glCreateProgram();
  GL_CALL(glAttachShader, m_ID, static_cast<GLShader *>(desc.vertex)->GetID());
  GL_CALL(glAttachShader, m_ID, static_cast<GLShader *>(desc.fragment)->GetID());

  i32 success;
  char infoLog[512];

  GL_CALL(glLinkProgram, m_ID);
  glGetProgramiv(m_ID, GL_LINK_STATUS, &success);
  if (!success) {
    glGetProgramInfoLog(m_ID, 512, NULL, infoLog);
    ENGINE_ERROR("%s", infoLog);
    ASSERT(false);
  }

  if (m_Desc.label.empty()) {
    m_Desc.label = std::format("OpenGL_Pipeline_{}", m_ID);
  }

  // TODO: remove these as soon as the resources system established
  ((GLShader *)m_Desc.vertex)->DeleteShader();
  ((GLShader *)m_Desc.fragment)->DeleteShader();
}
// --------------------------------------------------------------------------------
void GLRenderPipeline::SetUniform(const char *uniform, const Vec4 &v4) const {
  SetVec4(uniform, v4);
}
// --------------------------------------------------------------------------------
void GLRenderPipeline::SetUniform(const char *uniform, const Vec3 &v3) const {
  SetVec3(uniform, v3);
}
// --------------------------------------------------------------------------------
void GLRenderPipeline::SetUniform(const char *uniform, const Vec2 &v2) const {
  SetVec2(uniform, v2);
}
// --------------------------------------------------------------------------------
void GLRenderPipeline::SetUniform(const char *uniform, f32 x, f32 y, f32 z, f32 w) const {
  SetVec4(uniform, x, y, z, w);
}
// --------------------------------------------------------------------------------
void GLRenderPipeline::SetUniform(const char *uniform, f32 x, f32 y, f32 z) const {
  SetVec3(uniform, x, y, z);
}
// --------------------------------------------------------------------------------
void GLRenderPipeline::SetUniform(const char *uniform, f32 x, f32 y) const {
  SetVec2(uniform, x, y);
}
// --------------------------------------------------------------------------------
void GLRenderPipeline::SetUniform(const char *uniform, f32 v) const { SetFloat(uniform, v); }
// --------------------------------------------------------------------------------
void GLRenderPipeline::SetUniform(const char *uniform, i32 v) const { SetInt(uniform, v); }
// --------------------------------------------------------------------------------
void GLRenderPipeline::SetUniform(const char *uniform, const MatBase<4, 4, f32> &mat4) const {
  SetMat4(uniform, mat4);
}
// --------------------------------------------------------------------------------
void GLRenderPipeline::SetUniform(const char *uniform, const MatBase<4, 3, f32> &mat4x3) const {
  SetMat4x3(uniform, mat4x3);
}
// --------------------------------------------------------------------------------
void GLRenderPipeline::SetUniform(const char *uniform, const MatBase<4, 2, f32> &mat4x2) const {
  SetMat4x2(uniform, mat4x2);
}
// --------------------------------------------------------------------------------
void GLRenderPipeline::SetUniform(const char *uniform, const MatBase<3, 4, f32> &mat3x4) const {
  SetMat3x4(uniform, mat3x4);
}
// --------------------------------------------------------------------------------
void GLRenderPipeline::SetUniform(const char *uniform, const MatBase<3, 3, f32> &mat3) const {
  SetMat3(uniform, mat3);
}
// --------------------------------------------------------------------------------
void GLRenderPipeline::SetUniform(const char *uniform, const MatBase<3, 2, f32> &mat3x2) const {
  SetMat3x2(uniform, mat3x2);
}
// --------------------------------------------------------------------------------
void GLRenderPipeline::SetUniform(const char *uniform, const MatBase<2, 2, f32> &mat2) const {
  SetMat2(uniform, mat2);
}
// --------------------------------------------------------------------------------
void GLRenderPipeline::SetUniform(const char *uniform, const MatBase<2, 4, f32> &mat2x4) const {
  SetMat2x4(uniform, mat2x4);
}
// --------------------------------------------------------------------------------
void GLRenderPipeline::SetUniform(const char *uniform, const MatBase<2, 3, f32> &mat2x3) const {
  SetMat2x3(uniform, mat2x3);
}

// --------------------------------------------------------------------------------
void GLRenderPipeline::SetVec4(const char *uniform, f32 x, f32 y, f32 z, f32 w) const {
  ASSERT_CURRENT_PROGRAM(this);
  GL_CALL(glUniform4f, glGetUniformLocation(m_ID, uniform), x, y, z, w);
}
// --------------------------------------------------------------------------------
void GLRenderPipeline::SetVec4(const char *uniform, const Vec4 &v) const {
  ASSERT_CURRENT_PROGRAM(this);
  GL_CALL(glUniform4fv, glGetUniformLocation(m_ID, uniform), 1, v.ValuePtr());
}
// --------------------------------------------------------------------------------
void GLRenderPipeline::SetVec3(const char *uniform, f32 x, f32 y, f32 z) const {
  ASSERT_CURRENT_PROGRAM(this);
  GL_CALL(glUniform3f, glGetUniformLocation(m_ID, uniform), x, y, z);
}
// --------------------------------------------------------------------------------
void GLRenderPipeline::SetVec3(const char *uniform, const Vec3 &v) const {
  ASSERT_CURRENT_PROGRAM(this);
  GL_CALL(glUniform3f, glGetUniformLocation(m_ID, uniform), v.x, v.y, v.z);
}
// --------------------------------------------------------------------------------
void GLRenderPipeline::SetVec2(const char *uniform, f32 x, f32 y) const {
  ASSERT_CURRENT_PROGRAM(this);
  GL_CALL(glUniform2f, glGetUniformLocation(m_ID, uniform), x, y);
}
// --------------------------------------------------------------------------------
void GLRenderPipeline::SetVec2(const char *uniform, const Vec2 &v) const {
  ASSERT_CURRENT_PROGRAM(this);
  GL_CALL(glUniform2f, glGetUniformLocation(m_ID, uniform), v.x, v.y);
}
// --------------------------------------------------------------------------------
void GLRenderPipeline::SetFloat(const char *uniform, f32 v) const {
  ASSERT_CURRENT_PROGRAM(this);
  GL_CALL(glUniform1f, glGetUniformLocation(m_ID, uniform), v);
}
// --------------------------------------------------------------------------------
void GLRenderPipeline::SetBool(const char *uniform, b8 v) const {
  ASSERT_CURRENT_PROGRAM(this);
  GL_CALL(glUniform1i, glGetUniformLocation(m_ID, uniform), v);
}
// --------------------------------------------------------------------------------
void GLRenderPipeline::SetInt(const char *uniform, i32 v) const {
  ASSERT_CURRENT_PROGRAM(this);
  GL_CALL(glUniform1i, glGetUniformLocation(m_ID, uniform), v);
}
// --------------------------------------------------------------------------------
void GLRenderPipeline::SetMat4(const char *uniform, const MatBase<4, 4, f32> &mat4) const {
  ASSERT_CURRENT_PROGRAM(this);
  GL_CALL(glUniformMatrix4fv, glGetUniformLocation(m_ID, uniform), 1, GL_FALSE, mat4.ValuePtr());
}
// --------------------------------------------------------------------------------
void GLRenderPipeline::SetMat4x3(const char *uniform, const MatBase<4, 3, f32> &mat4x3) const {
  ASSERT_CURRENT_PROGRAM(this);
  GL_CALL(
    glUniformMatrix4x3fv, glGetUniformLocation(m_ID, uniform), 1, GL_FALSE, mat4x3.ValuePtr()
  );
}
// --------------------------------------------------------------------------------
void GLRenderPipeline::SetMat4x2(const char *uniform, const MatBase<4, 2, f32> &mat4x2) const {
  ASSERT_CURRENT_PROGRAM(this);
  GL_CALL(
    glUniformMatrix4x2fv, glGetUniformLocation(m_ID, uniform), 1, GL_FALSE, mat4x2.ValuePtr()
  );
}
// --------------------------------------------------------------------------------
void GLRenderPipeline::SetMat3x4(const char *uniform, const MatBase<3, 4, f32> &mat3x4) const {
  ASSERT_CURRENT_PROGRAM(this);
  GL_CALL(
    glUniformMatrix3x4fv, glGetUniformLocation(m_ID, uniform), 1, GL_FALSE, mat3x4.ValuePtr()
  );
}
// --------------------------------------------------------------------------------
void GLRenderPipeline::SetMat3(const char *uniform, const MatBase<3, 3, f32> &mat3) const {
  ASSERT_CURRENT_PROGRAM(this);
  GL_CALL(glUniformMatrix3fv, glGetUniformLocation(m_ID, uniform), 1, GL_FALSE, mat3.ValuePtr());
}
// --------------------------------------------------------------------------------
void GLRenderPipeline::SetMat3x2(const char *uniform, const MatBase<3, 2, f32> &mat3x2) const {
  ASSERT_CURRENT_PROGRAM(this);
  GL_CALL(
    glUniformMatrix3x2fv, glGetUniformLocation(m_ID, uniform), 1, GL_FALSE, mat3x2.ValuePtr()
  );
}
// --------------------------------------------------------------------------------
void GLRenderPipeline::SetMat2(const char *uniform, const MatBase<2, 2, f32> &mat2) const {
  ASSERT_CURRENT_PROGRAM(this);
  GL_CALL(glUniformMatrix2fv, glGetUniformLocation(m_ID, uniform), 1, GL_FALSE, mat2.ValuePtr());
}
// --------------------------------------------------------------------------------
void GLRenderPipeline::SetMat2x4(const char *uniform, const MatBase<2, 4, f32> &mat2x4) const {
  ASSERT_CURRENT_PROGRAM(this);
  GL_CALL(
    glUniformMatrix2x4fv, glGetUniformLocation(m_ID, uniform), 1, GL_FALSE, mat2x4.ValuePtr()
  );
}
// --------------------------------------------------------------------------------
void GLRenderPipeline::SetMat2x3(const char *uniform, const MatBase<2, 3, f32> &mat2x3) const {
  ASSERT_CURRENT_PROGRAM(this);
  GL_CALL(
    glUniformMatrix2x3fv, glGetUniformLocation(m_ID, uniform), 1, GL_FALSE, mat2x3.ValuePtr()
  );
}
// --------------------------------------------------------------------------------
void GLRenderPipeline::Bind() const { GL_CALL(glUseProgram, m_ID); }
// --------------------------------------------------------------------------------
void GLRenderPipeline::Unbind() const { GL_CALL(glUseProgram, 0); }
// --------------------------------------------------------------------------------
GLuint GLRenderPipeline::GetID() const { return m_ID; }
// --------------------------------------------------------------------------------
GLRenderPipeline::operator GLuint() const { return m_ID; }
// --------------------------------------------------------------------------------
GLRenderPipeline::~GLRenderPipeline() { GL_CALL(glDeleteProgram, m_ID); }
