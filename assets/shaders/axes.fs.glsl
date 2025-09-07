uniform vec3 uAxisColor;

out vec4 FragColor;

void main() {
  FragColor = vec4(uAxisColor, 1.0f);
}
