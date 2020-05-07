#version 150

in vec3 inPosition;
uniform mat4 projMatrix;
uniform mat4 viewMatrix;

void main(void) {
	gl_Position = projMatrix * viewMatrix * vec4(inPosition, 1.0);
}
