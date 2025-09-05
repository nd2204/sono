#ifndef SN_RENDER_PIPELINE_H
#define SN_RENDER_PIPELINE_H

#include <render/shader/shader.h>
#include "core/common/types.h"
#include "core/math/vec4.h"
#include "core/math/vec3.h"
#include "core/math/vec2.h"
#include "core/math/mat4.h"
#include "core/math/mat3.h"

struct PipelineDesc {
  Shader *vertex;
  Shader *fragment;
};

class RenderPipeline {
public:
  explicit RenderPipeline(const PipelineDesc &desc)
    : m_Desc(desc) {}

  virtual ~RenderPipeline() = default;

  virtual void Bind() const = 0;

  virtual void Unbind() const = 0;

  virtual void SetUniform(const char *uniform, const Vec4 &v4) const = 0;
  virtual void SetUniform(const char *uniform, const Vec3 &v3) const = 0;
  virtual void SetUniform(const char *uniform, const Vec2 &v2) const = 0;
  virtual void SetUniform(const char *uniform, f32 x, f32 y, f32 z, f32 w) const = 0;
  virtual void SetUniform(const char *uniform, f32 x, f32 y, f32 z) const = 0;
  virtual void SetUniform(const char *uniform, f32 x, f32 y) const = 0;

  virtual void SetUniform(const char *uniform, f32 v) const = 0;
  virtual void SetUniform(const char *uniform, i32 v) const = 0;

  virtual void SetUniform(const char *uniform, const MatBase<4, 4, f32> &mat4) const = 0;
  virtual void SetUniform(const char *uniform, const MatBase<4, 3, f32> &mat4x3) const = 0;
  virtual void SetUniform(const char *uniform, const MatBase<4, 2, f32> &mat4x2) const = 0;

  virtual void SetUniform(const char *uniform, const MatBase<3, 4, f32> &mat3x4) const = 0;
  virtual void SetUniform(const char *uniform, const Mat3 &mat3) const = 0;
  virtual void SetUniform(const char *uniform, const MatBase<3, 2, f32> &mat3x2) const = 0;

  virtual void SetUniform(const char *uniform, const MatBase<2, 2, f32> &mat2) const = 0;
  virtual void SetUniform(const char *uniform, const MatBase<2, 4, f32> &mat2x4) const = 0;
  virtual void SetUniform(const char *uniform, const MatBase<2, 3, f32> &mat2x3) const = 0;

  virtual void SetVec4(const char *uniform, f32 x, f32 y, f32 z, f32 w) const = 0;
  virtual void SetVec4(const char *uniform, const Vec4 &v4) const = 0;
  virtual void SetVec3(const char *uniform, f32 x, f32 y, f32 z) const = 0;
  virtual void SetVec3(const char *uniform, const Vec3 &v3) const = 0;
  virtual void SetVec2(const char *uniform, f32 x, f32 y) const = 0;
  virtual void SetVec2(const char *uniform, const Vec2 &v2) const = 0;

  virtual void SetFloat(const char *uniform, f32 v) const = 0;
  virtual void SetInt(const char *uniform, i32 v) const = 0;
  virtual void SetBool(const char *uniform, b8 v) const = 0;

  virtual void SetMat4(const char *uniform, f32 *mat4) const = 0;
  virtual void SetMat4(const char *uniform, const Mat4 &mat4) const = 0;
  virtual void SetMat4x3(const char *uniform, f32 *mat4x3) const = 0;
  virtual void SetMat4x2(const char *uniform, f32 *mat4x2) const = 0;

  virtual void SetMat3x4(const char *uniform, f32 *mat3x4) const = 0;
  virtual void SetMat3(const char *uniform, f32 *mat3) const = 0;
  virtual void SetMat3(const char *uniform, const Mat3 &mat3) const = 0;
  virtual void SetMat3x2(const char *uniform, f32 *mat3x2) const = 0;

  virtual void SetMat2(const char *uniform, f32 *mat2) const = 0;
  virtual void SetMat2x4(const char *uniform, f32 *mat2x4) const = 0;
  virtual void SetMat2x3(const char *uniform, f32 *mat2x3) const = 0;

protected:
  PipelineDesc m_Desc;
};

#endif // !SN_RENDER_PIPELINE_H
