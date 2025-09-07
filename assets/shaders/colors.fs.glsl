struct Material {
  sampler2D diffuse;
  sampler2D specular;
  float shininess;
};

struct Light {
  vec3 direction;
  vec3 ambient;
  vec3 diffuse;
  vec3 specular;
};

in vec3 vNorm;
in vec3 vFragPos;
in vec2 vTexCoords;
in Light vLight;

uniform sampler2D uEmission;
uniform Material uMaterial;
uniform vec3 uObjectColor;
uniform float uTime;

out vec4 FragColor;

void main() {
  // Calculate ambient
  vec3 ambient = vLight.ambient * texture(uMaterial.diffuse, vTexCoords).rgb;

  // Calculate diffuse
  vec3 norm = normalize(vNorm);
  // vec3 lightDir = normalize(vLight.position - vFragPos);
  vec3 lightDir = normalize(-vLight.direction);
  float diff = max(dot(norm, lightDir), 0.0f);
  vec3 diffuse = vLight.diffuse * diff * texture(uMaterial.diffuse, vTexCoords).rgb;

  // Calculate specular
  vec3 viewDir = normalize(-vFragPos);
  vec3 reflDir = reflect(-lightDir, norm);
  float spec = pow(max(dot(viewDir, reflDir), 0.0), uMaterial.shininess);
  vec3 specMap = texture(uMaterial.specular, vTexCoords).rgb;
  vec3 specular = vLight.specular * spec * specMap;

  // Calculate emission
  vec3 emission = vec3(0.0);
  if (specMap.r == 0.0) {
    emission = texture(uEmission, vTexCoords + vec2(0, uTime)).rgb;
    emission = emission * 0.5;
  }

  // Calculate final fragment color
  vec3 result = ambient + diffuse + specular + emission;
  FragColor = vec4(result, 1.0);
}
