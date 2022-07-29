// Version of GLSL and profile (core)
#version 330 core
// Location not fully understood (maybe when inputting parameters it's location relative to that?), but creates a vector with 3 values for input
layout (location = 0) in vec3 posInput;
layout (location = 1) in vec2 texCoordInput;

out vec2 texCoord;
out vec3 col;

uniform mat4 projection;
uniform mat4 view;
uniform mat4 model;

void main() {
	gl_Position = projection * view * model * vec4(posInput, 1.0f);
	texCoord = texCoordInput;
}