#ifndef SN_GL_SHADERS_H
#define SN_GL_SHADERS_H

#include <render/shader/shader.h>
#include <core/common/types.h>
#include <glad/glad.h>

class GLShader : public Shader {
public:
  GLShader(const ShaderDesc &desc);

  ~GLShader();

  b8 Compile() override;

  GLuint GetID() const;

  void SetSource(const char **src);

  void DeleteShader();

  operator GLuint() const;

private:
  GLuint m_ShaderID;
};

#endif // !SN_GL_SHADERS_H
