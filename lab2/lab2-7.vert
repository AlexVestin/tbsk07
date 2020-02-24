#version 150

in vec3 inPosition;
in vec3 inNormal;
in vec2 inTexCoord;

out vec2 texCoord;
out vec3 normals;

uniform mat4 model, view, proj;
out vec3 exColor;
// Gouraud shader - vertex shader
void main(void)
{
	vec3 normals = inNormal;// * mat3(total);

	const vec3 light = vec3(0.58, 0.58, 0.58);
	float shade;
	shade = dot(normalize(normals), light);
	shade = clamp(shade, 0, 1);
	exColor = vec3(shade);
	gl_Position =  proj * view * model * vec4(inPosition, 1.0);
	normals = inNormal * mat3(model);
	texCoord = inTexCoord;
}
