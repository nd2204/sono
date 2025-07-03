#ifndef SN_GL_SHADERS_H
#define SN_GL_SHADERS_H

#include "core/common/types.h"
#include "glad/glad.h"

#include "render/shader.h"

class GLShader : public Shader {
public:
  GLShader();

  GLShader(const ShaderDesc *desc);

  ~GLShader();

  virtual void CompileFromSrc(const std::string &src, ShaderStage stage) override;

  virtual void CompileFromFile(const std::string &filePath, ShaderStage stage) override;

  virtual ShaderStage GetStage() const override;

  GLuint GetID() const;

  void SetSource(const char **src);

  void DeleteShader();

  operator GLuint() const;

private:
  static GLuint _Compile(const char *src, u32 type);

private:
  GLuint m_ShaderID;
};

#define VERTEX_SHADER_BIT   1
#define FRAGMENT_SHADER_BIT 1 << 1
#define GEOMETRY_SHADER_BIT 1 << 2

class GLRenderPipeline : public RenderPipeline {
public:
  GLRenderPipeline(Shader *s1, Shader *s2);

  ~GLRenderPipeline();

  void SetVec4(const char *uniform, f32 x, f32 y, f32 z, f32 w) const;
  void SetVec3(const char *uniform, f32 x, f32 y, f32 z) const;
  void SetVec2(const char *uniform, f32 x, f32 y) const;

  void SetFloat(const char *uniform, f32 v) const;
  void SetInt(const char *uniform, i32 v) const;
  void SetBool(const char *uniform, u8 v) const;

  void SetMat4(const char *uniform, f32 *mat4) const;
  void SetMat4x3(const char *uniform, f32 *mat4x3) const;
  void SetMat4x2(const char *uniform, f32 *mat4x2) const;

  void SetMat3x4(const char *uniform, f32 *mat3x4) const;
  void SetMat3(const char *uniform, f32 *mat3) const;
  void SetMat3x2(const char *uniform, f32 *mat3x2) const;

  void SetMat2(const char *uniform, f32 *mat2) const;
  void SetMat2x4(const char *uniform, f32 *mat2x4) const;
  void SetMat2x3(const char *uniform, f32 *mat2x3) const;

  void Bind() const;

  void Unbind() const;

  u32 GetID() const;

  operator u32() const;

private:
  void LinkProgram() const;
  void AttachShader(Shader *shader);

private:
  u32 m_ID;
  u8 m_AttachedShaders;
};

#endif // !SN_GL_SHADERS_H
