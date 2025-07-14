#version 330 core
out vec4 FragColor;

in vec3 vNorm;
in vec3 vFragPos;
  
uniform vec3 uObjectColor;
uniform vec3 uLightPos;
uniform vec3 uLightColor;

void main() {
  float ambientStrength = 0.1;
  vec3 ambient = ambientStrength * uLightColor;

  vec3 norm = normalize(vNorm);
  vec3 lightDir = normalize(uLightPos - vFragPos);
  vec3 diffuse = max(dot(norm, lightDir), 0.0f) * uLightColor;

  vec3 result = (ambient + diffuse) * uObjectColor;
  FragColor = vec4(result, 1.0);
}
