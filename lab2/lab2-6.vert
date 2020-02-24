#version 150

in vec3 in_Position;
in vec3 in_Normal;
in vec2 inTexCoord;

out vec2 texCoord;
out vec3 normals;

uniform mat4 rot, trans, total, view, proj;
out vec3 exColor;
// Gouraud shader - vertex shader
void main(void)
{
	vec3 normals = in_Normal;// * mat3(total);

	const vec3 light = vec3(0.58, 0.58, 0.58);
	float shade;
	shade = dot(normalize(normals), light);
	shade = clamp(shade, 0, 1);
	exColor = vec3(shade);
	gl_Position =  proj * view * total * vec4(in_Position, 1.0);

	/*
	normals = in_Normal * mat3(total);
	texCoord = inTexCoord;
	gl_Position =  proj * view * total * vec4(in_Position, 1.0);
	*/
}
