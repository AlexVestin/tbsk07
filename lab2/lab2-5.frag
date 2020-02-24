#version 150

in vec4 iColor;
in vec2 texCoord;
out vec4 out_Color;
uniform float time;
uniform sampler2D texUnit;
in vec3 normals;
// Phong shader - vertex shader
void main(void) 
{
	const vec3 light = vec3(0.58, 0.58, 0.58);
	float diff = max(dot(normalize(normals), light), 0.0);
	out_Color = vec4(diff);// * texture(texUnit, texCoord);
}
