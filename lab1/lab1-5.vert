#version 150

in vec3 in_Position;
in vec3 in_Color;
out vec4 iColor;

uniform mat4 myMatrix;

void main(void)
{
	iColor = vec4(in_Color, 1.0);
	gl_Position =  myMatrix * vec4(in_Position, 1.0);
}
