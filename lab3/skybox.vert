#version 150

in vec3 inPosition;
in vec3 inNormal;
in vec2 inTexCoord;
out vec2 texCoord;

uniform mat4 model, view, proj;
out vec3 exColor;

void main(void)
{
	gl_Position = proj *  view * vec4(inPosition, 1.0);	
	texCoord = inTexCoord;
}
