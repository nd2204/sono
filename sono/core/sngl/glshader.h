#ifndef SHADERS_H
#define SHADERS_H

#include "../common/types.h"

class GLShader {
public:
  GLShader(const char **src, u32 type);
  GLShader(const char *filename, u32 type);
  ~GLShader();

  u32 GetType() const;
  u32 GetId() const;
  void DeleteShader();
  operator u32() const;

private:
  u32 m_Id;
  u32 m_Type;

  static u32 _Compile(const char *src, u32 type);
};

#define VERTEX_SHADER_BIT   1
#define FRAGMENT_SHADER_BIT 1 << 1
#define GEOMETRY_SHADER_BIT 1 << 2

class GLShaderProgram {
private:
  u32 m_Id;
  u8 m_AttachedShaders;

public:
  GLShaderProgram(GLShader &s1, GLShader &s2);
  ~GLShaderProgram();

  void AttachShader(GLShader &shader);
  void LinkProgram() const;
  void SetVec4(const char *uniform, f32 x, f32 y, f32 z, f32 w) const;
  void SetVec3(const char *uniform, f32 x, f32 y, f32 z) const;
  void SetVec2(const char *uniform, f32 x, f32 y) const;
  void SetFloat(const char *uniform, f32 v) const;
  void SetInt(const char *uniform, i32 v) const;
  void SetBool(const char *uniform, u8 v) const;
  // void SetMat4x4(const char *uniform, ) const;
  // void SetMat4x3(const char *uniform, ) const;
  // void SetMat4x2(const char *uniform, ) const;
  // void SetMat3x3(const char *uniform, ) const;
  // void SetMat3x2(const char *uniform, ) const;
  // void SetMat2x2(const char *uniform, ) const;
  void Use() const;
  u32 GetId() const;

  operator u32() const;

  static GLShaderProgram &DefaultPipeLine();
};

#endif // !_SHADERS_H