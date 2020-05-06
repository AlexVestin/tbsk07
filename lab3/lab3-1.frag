#version 150

uniform sampler2D texUnit;
const vec3 light = vec3(0.58, 0.58, 0.58);

in vec2 texCoord;
in vec3 transNormal;
out vec4 out_Color;


void main(void)
{ 
	float shade = clamp(dot(transNormal, light), 0.0, 1.0);
	out_Color   = vec4(shade, shade, shade, 1.0);
}
