#version 330 core

uniform vec3 uAxisColor;

out vec4 FragColor;

void main() {
  FragColor = vec4(uAxisColor, 1.0f);
}
