#ifndef SN_SHADER_H
#define SN_SHADER_H

#include <string>
#include "core/common/types.h"

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

  virtual Shader *GetVertexShader() { return m_VertexShader; };

  virtual Shader *GetFragmentShader() { return m_FragmentShader; };

protected:
  Shader *m_VertexShader;
  Shader *m_FragmentShader;
};

#endif // !SN_SHADER_H
