#version 330 core

// output a vector4 (color)
out vec4 fragColor;

in vec2 texCoord;

uniform sampler2D unpoppedTexture;
uniform sampler2D poppedTexture;

void main() {
	fragColor = mix(texture(poppedTexture, texCoord), texture(unpoppedTexture, -texCoord), 0.5f);
} 