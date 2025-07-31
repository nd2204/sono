#version 330 core

out vec4 FragColor;

// uniform vec3 gridColor;

void main() {
  vec3 gridColor = vec3(0.3, 0.3, 0.3);
  FragColor = vec4(gridColor, 1.0);
}
