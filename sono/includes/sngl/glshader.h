#ifndef SHADERS_H
#define SHADERS_H

#include "sono/types.h"

class GLShader {
public:
  GLShader(const char **src, u32 type);
  GLShader(const char *filename, u32 type);
  ~GLShader();

  u32 GetType();
  u32 GetId();
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
  void LinkProgram();
  void Use();

  operator u32() const;

  static GLShaderProgram &DefaultPipeLine();
};

#endif // !_SHADERS_H