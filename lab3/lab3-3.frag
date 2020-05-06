#version 150

uniform sampler2D texUnit;
const vec3 light = vec3(0.58, 0.58, 0.58);
const vec3 amb   = vec3(0.0, 0.0, -0.15);

in vec2 texCoord;
in vec3 transNormal;
out vec4 out_Color;


void main(void)
{ 
	float prim  = clamp(dot(transNormal, light), 0.0, 1.0);
	float sec   = clamp(dot(transNormal, amb), 0.0, 1.0);
	float tot   = prim + sec;
	tot		    = clamp(tot, 0.05, 1.0);
	
	vec4  tex   = texture(texUnit, texCoord) * tot;

	out_Color   = clamp(tex, 0.05, 1.0);
	
}
