#version 150

in vec3 in_Position;
in vec3 in_Normal;
in vec2 inTexCoord;

out vec2 texCoord;
out vec3 normals;

out vec4 iColor;
uniform mat4 rot, trans, total, view, proj;

void main(void)
{
	normals =  mat3(view) * mat3(total) * in_Normal;
	texCoord = inTexCoord;
	gl_Position =  proj * view * total * vec4(in_Position, 1.0);
}
