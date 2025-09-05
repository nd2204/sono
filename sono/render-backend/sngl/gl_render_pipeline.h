#ifndef SN_GL_RENDER_PIPELINE_H
#define SN_GL_RENDER_PIPELINE_H

#include <render/render_pipeline.h>
#include <core/common/types.h>
#include <glad/glad.h>

class GLRenderPipeline : public RenderPipeline {
public:
  GLRenderPipeline(const PipelineDesc &desc);

  ~GLRenderPipeline();

  virtual void SetUniform(const char *uniform, const Vec4 &v4) const override;
  virtual void SetUniform(const char *uniform, const Vec3 &v3) const override;
  virtual void SetUniform(const char *uniform, const Vec2 &v2) const override;
  virtual void SetUniform(const char *uniform, f32 x, f32 y, f32 z, f32 w) const override;
  virtual void SetUniform(const char *uniform, f32 x, f32 y, f32 z) const override;
  virtual void SetUniform(const char *uniform, f32 x, f32 y) const override;

  virtual void SetUniform(const char *uniform, f32 v) const override;
  virtual void SetUniform(const char *uniform, i32 v) const override;

  virtual void SetUniform(const char *uniform, const MatBase<4, 4, f32> &mat4) const override;
  virtual void SetUniform(const char *uniform, const MatBase<4, 3, f32> &mat4x3) const override;
  virtual void SetUniform(const char *uniform, const MatBase<4, 2, f32> &mat4x2) const override;

  virtual void SetUniform(const char *uniform, const MatBase<3, 4, f32> &mat3x4) const override;
  virtual void SetUniform(const char *uniform, const Mat3 &mat3) const override;
  virtual void SetUniform(const char *uniform, const MatBase<3, 2, f32> &mat3x2) const override;

  virtual void SetUniform(const char *uniform, const MatBase<2, 2, f32> &mat2) const override;
  virtual void SetUniform(const char *uniform, const MatBase<2, 4, f32> &mat2x4) const override;
  virtual void SetUniform(const char *uniform, const MatBase<2, 3, f32> &mat2x3) const override;

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

  // void SetUniform(UniformType type, const char *uniform, const void *data) const override;

  void Bind() const override;
  void Unbind() const override;
  GLuint GetID() const;
  operator GLuint() const;

private:
  GLuint m_ID;
  u8 m_AttachedShaders;
};

#endif // !SN_GL_RENDER_PIPELINE_H
