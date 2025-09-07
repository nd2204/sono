// in vec2 vTexCoord;
//
// uniform sampler2D uTexture0;
// uniform sampler2D uTexture1;

uniform vec3 uObjectColor;
uniform vec3 uLightColor;

out vec4 FragColor;

void main() {
  // FragColor = mix(texture(uTexture0, vTexCoord), texture(uTexture1, vTexCoord), 0.2f);
  FragColor = vec4(uObjectColor * uLightColor, 1.0f);
}
