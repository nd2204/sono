#version 330 core

out vec4 fragColor;

in vec3 vColor; 
in vec2 vTexCoord;

uniform sampler2D texture1;
uniform sampler2D texture2;
uniform float uOpacity;

void main() {
  fragColor = mix(texture(texture1, vTexCoord), texture(texture2, vTexCoord), uOpacity);
}