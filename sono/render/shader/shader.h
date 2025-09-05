#ifndef SN_SHADER_H
#define SN_SHADER_H

#include "core/common/logger.h"
#include <string>
#include <fstream>
#include <sstream>

enum class ShaderStage { VERTEX, FRAGMENT, COMPUTE, GEOMETRY, TESSELATION };

inline std::string LoadShaderSrcFromFile(const char *filePath) {
  std::ifstream file(filePath);
  std::stringstream buffer;
  buffer << file.rdbuf();
  LOG_INFO_F("Loaded shader sources from %s", filePath);
  return buffer.str();
}

struct ShaderDesc {
  std::string src;
  std::string entryPoint = "main";
  ShaderStage stage;
};

class Shader {
public:
  Shader(const ShaderDesc &desc)
    : m_Desc(desc) {};

  virtual ~Shader() {}

  virtual void Compile() = 0;

  virtual ShaderStage GetStage() const { return m_Desc.stage; };

protected:
  ShaderDesc m_Desc;
};

#endif // !SN_SHADER_H
