#version 330 core

in vec2 vTexCoord;

uniform sampler2D uTexture0;
uniform sampler2D uTexture1;

out vec4 fragColor;

void main() {
  fragColor = mix(texture(uTexture0, vTexCoord), texture(uTexture1, vTexCoord), 0.2f);
}
