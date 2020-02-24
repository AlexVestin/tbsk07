#version 150

in vec3 in_Position;
in vec3 in_Normal;
in vec2 inTexCoord;

out vec2 texCoord;
out vec4 iColor;
uniform mat4 proj;
uniform mat4 rot, trans, total;


void main(void)
{
	texCoord = inTexCoord;
	iColor = vec4(in_Normal, 1.0);
	gl_Position =  proj * total * vec4(in_Position, 1.0);
}
