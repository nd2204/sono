#ifndef SN_SHADER_H
#define SN_SHADER_H

#include "core/common/types.h"
#include "core/math/vec4.h"
#include "core/math/vec3.h"
#include "core/math/vec2.h"
#include "core/math/mat4.h"
#include "core/math/mat3.h"

#include <string>

enum class ShaderStage { VERTEX, FRAGMENT, COMPUTE, GEOMETRY, TESSELATION };

struct ShaderDesc {
  std::string src;
  std::string entryPoint = "main";
  ShaderStage stage;
};

class Shader {
public:
  virtual ~Shader() = default;

  virtual void CompileFromSrc(const std::string &src, ShaderStage stage) = 0;

  virtual void CompileFromFile(const std::string &filePath, ShaderStage stage) = 0;

  virtual ShaderStage GetStage() const { return m_Stage; };

protected:
  std::string m_Src;
  ShaderStage m_Stage;
};

class RenderPipeline {
public:
  virtual ~RenderPipeline() = default;

  virtual void Bind() const = 0;

  virtual void Unbind() const = 0;

  virtual Shader *GetVertexShader() { return m_VertexShader; };

  virtual Shader *GetFragmentShader() { return m_FragmentShader; };

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
  Shader *m_VertexShader;
  Shader *m_FragmentShader;
};

#endif // !SN_SHADER_H
