#version 330 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aNorm;

struct Light {
  vec3 position;

  vec3 ambient;
  vec3 diffuse;
  vec3 specular;
};

out vec3 vNorm;
out vec3 vFragPos;
out Light vLight;

uniform Light uLight;
uniform mat4 uModel;
uniform mat4 uView;
uniform mat4 uProj;

void main() {
  gl_Position = uProj * uView * uModel * vec4(aPos, 1.0);
  vFragPos = vec3(uView * uModel * vec4(aPos, 1.0));
  vLight = uLight;
  vLight.position = vec3(uView * vec4(uLight.position, 1.0));
  vNorm = mat3(transpose(inverse(uView * uModel))) * aNorm;
}
