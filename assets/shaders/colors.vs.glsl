struct Light {
  vec3 direction;
  vec3 ambient;
  vec3 diffuse;
  vec3 specular;
};

layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aNorm;
layout (location = 2) in vec2 aTexCoords;

out vec3 vNorm;
out vec3 vFragPos;
out vec2 vTexCoords;
out Light vLight;

uniform Light uLight;
uniform mat4 uModel;
uniform mat4 uView;
uniform mat4 uProj;

void main() {
  vFragPos = vec3(uView * uModel * vec4(aPos, 1.0));
  vLight = uLight;
  vLight.direction = vec3(uView * vec4(uLight.direction, 1.0));
  vNorm = mat3(transpose(inverse(uView * uModel))) * aNorm;
  vTexCoords = aTexCoords;

  gl_Position = uProj * uView * uModel * vec4(aPos, 1.0);
}
