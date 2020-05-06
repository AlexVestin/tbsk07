#version 150

uniform mat4 tranMatrix, projMatrix, camMatrix;
uniform mat4 rot, trans, total;

in vec3 in_Position;
in vec3 in_Normal;
in vec2 inTexCoord;

out vec3 transNormal;
out vec2 texCoord;

void main(void)
{
	mat3 normalMatrix = mat3(tranMatrix);
	gl_Position = projMatrix * camMatrix * tranMatrix * vec4(in_Position, 1.0);
	texCoord	= inTexCoord;
	transNormal = normalMatrix * in_Normal;
}
