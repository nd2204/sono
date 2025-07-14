#version 330 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aNorm;

out vec3 vNorm;
out vec3 vFragPos;

uniform mat4 uModel;
uniform mat4 uView;
uniform mat4 uProj;

void main() {
  gl_Position = uProj * uView * uModel * vec4(aPos, 1.0);
  vFragPos = vec3(uModel * vec4(aPos, 1.0));
  vNorm = aNorm;
}
