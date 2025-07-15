#version 330 core
out vec4 FragColor;

in vec3 vNorm;
in vec3 vFragPos;
  
uniform vec3 uObjectColor;
uniform vec3 uLightPos;
uniform vec3 uViewPos;
uniform vec3 uLightColor;

void main() {
  // Calculate ambient
  float ambientStrength = 0.1;
  vec3 ambient = ambientStrength * uLightColor;

  // Calculate diffuse
  vec3 norm = normalize(vNorm);
  vec3 lightDir = normalize(uLightPos - vFragPos);
  vec3 diffuse = max(dot(norm, lightDir), 0.0f) * uLightColor;

  // Calculate specular
  float specularStrength = 0.5;
  int shininess = 32;
  vec3 viewDir = normalize(uViewPos - vFragPos);
  vec3 reflDir = reflect(-lightDir, norm);
  float spec = pow(max(dot(viewDir, reflDir), 0.0), shininess);
  vec3 specular = specularStrength * spec * uLightColor;

  // Calculate final fragment color
  vec3 result = (ambient + diffuse + specular) * uObjectColor;
  FragColor = vec4(result, 1.0);
}
