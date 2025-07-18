#version 330 core
out vec4 FragColor;

struct Material {
  vec3 ambient;
  vec3 diffuse;
  vec3 specular;
  float shininess;
};

struct Light {
  vec3 position;

  vec3 ambient;
  vec3 diffuse;
  vec3 specular;
};

in vec3 vNorm;
in vec3 vFragPos;
in Light vLight;

uniform Material uMaterial;
uniform vec3 uObjectColor;

void main() {
  // Calculate ambient
  vec3 ambient = uMaterial.ambient * vLight.ambient;

  // Calculate diffuse
  vec3 norm = normalize(vNorm);
  vec3 lightDir = normalize(vLight.position - vFragPos);
  float diff = max(dot(norm, lightDir), 0.0f);
  vec3 diffuse = vLight.diffuse * (diff * uMaterial.diffuse);

  // Calculate specular
  vec3 viewDir = normalize(-vFragPos);
  vec3 reflDir = reflect(-lightDir, norm);
  float spec = pow(max(dot(viewDir, reflDir), 0.0), uMaterial.shininess);
  vec3 specular = vLight.specular * (spec * uMaterial.specular);

  // Calculate final fragment color
  vec3 result = ambient + diffuse + specular;
  FragColor = vec4(result, 1.0);
}
