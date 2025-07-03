#version 330 core

out vec4 fragColor;

in vec3 vColor; 
in vec2 vTexCoord;

uniform sampler2D uTexture0;
uniform sampler2D uTexture1;
uniform float uOpacity;

void main() {
  fragColor = mix(texture(uTexture0, vTexCoord), texture(uTexture1, vTexCoord), uOpacity);
}
