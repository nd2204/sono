#include <render-backend/sngl/gl_shader.h>
#include <render/shader/shader.h>
#include <core/common/logger.h>
#include <core/common/snassert.h>
#include <glad/glad.h>
#include <unordered_map>

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
b8 GLShader::Compile() {
  SN_ASSERT(!m_Desc.src.empty(), "shader sources is empty");

  GLenum type = ShaderStageToOpenGLShader(m_Desc.stage);
  m_ShaderID = glCreateShader(type);

  i32 success;
  char infoLog[512];

  // glShaderSource expected a const char * const pointer
  // TODO: Make the version string configurable from render system
  const char *sources[] = {"#version 330 core\n", m_Desc.src.c_str()};
  glShaderSource(m_ShaderID, 2, sources, NULL);
  glCompileShader(m_ShaderID);

  glGetShaderiv(m_ShaderID, GL_COMPILE_STATUS, &success);
  if (!success) {
    glGetShaderInfoLog(m_ShaderID, 512, NULL, infoLog);
    switch (type) {
      case GL_VERTEX_SHADER:
        ENGINE_ERROR("Vertex shader compilation failed: %s", infoLog);
        break;
      case GL_GEOMETRY_SHADER:
        ENGINE_ERROR("Geometry shader compilation failed: %s", infoLog);
        break;
      case GL_FRAGMENT_SHADER:
        ENGINE_ERROR("Fragment shader compilation failed: %s", infoLog);
        break;
    }
  }
  return success;
}
// --------------------------------------------------------------------------------
GLuint GLShader::GetID() const { return m_ShaderID; }
// --------------------------------------------------------------------------------
void GLShader::DeleteShader() { glDeleteShader(m_ShaderID); }
// --------------------------------------------------------------------------------
GLShader::operator GLuint() const { return m_ShaderID; }
// --------------------------------------------------------------------------------
GLShader::~GLShader() { DeleteShader(); }
