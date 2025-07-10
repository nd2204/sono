#ifndef SN_GL_SHADERS_H
#define SN_GL_SHADERS_H

#include "core/common/types.h"
#include <glad/glad.h>

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

  void SetVec4(const char *uniform, f32 x, f32 y, f32 z, f32 w) const override;
  void SetVec4(const char *uniform, const Vec4 &v4) const override;
  void SetVec3(const char *uniform, f32 x, f32 y, f32 z) const override;
  void SetVec3(const char *uniform, const Vec3 &v3) const override;
  void SetVec2(const char *uniform, f32 x, f32 y) const override;
  void SetVec2(const char *uniform, const Vec2 &v2) const override;

  void SetFloat(const char *uniform, f32 v) const override;
  void SetInt(const char *uniform, i32 v) const override;
  void SetBool(const char *uniform, b8 v) const override;

  void SetMat4(const char *uniform, f32 *mat4) const override;
  void SetMat4(const char *uniform, const Mat4 &mat4) const override;

  void SetMat4x3(const char *uniform, f32 *mat4x3) const override;
  void SetMat4x2(const char *uniform, f32 *mat4x2) const override;

  void SetMat3x4(const char *uniform, f32 *mat3x4) const override;
  void SetMat3(const char *uniform, f32 *mat3) const override;
  void SetMat3(const char *uniform, const Mat3 &mat3) const override;
  void SetMat3x2(const char *uniform, f32 *mat3x2) const override;

  void SetMat2(const char *uniform, f32 *mat2) const override;
  void SetMat2x4(const char *uniform, f32 *mat2x4) const override;
  void SetMat2x3(const char *uniform, f32 *mat2x3) const override;

  void Bind() const;
  void Unbind() const;
  GLuint GetID() const;
  operator GLuint() const;

private:
  void LinkProgram() const;
  void AttachShader(Shader *shader);

private:
  GLuint m_ID;
  u8 m_AttachedShaders;
};

#endif // !SN_GL_SHADERS_H
