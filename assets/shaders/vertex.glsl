#version 330 core

layout (location = 0) in vec3 aPos;
// layout (location = 1) in vec2 aTexCoord;

// out vec2 vTexCoord;

uniform mat4 uTransform;
uniform mat4 uModel;
uniform mat4 uView;
uniform mat4 uProj;

void main() {
  gl_Position = uProj * uView * uModel * vec4(aPos, 1.0);
  // vTexCoord = aTexCoord;
}
