#version 150
#define pi 3.14159265359
out vec4 outColor;

in vec3 Normal;
in vec3 FragPos;

uniform vec3 lightPos;

uniform float iTime;

in vec3 Position;
vec4 mainImage() {
	vec3 n = normalize(Position);
	vec2 uv = vec2(atan(n.x / n.y) / (2*pi) + 0.5, n.z * 0.5 + 0.5) -0.5;

	float t = iTime * .1 + ((.25 + .05 * sin(iTime * .1))/(length(uv.xy) + .07)) * 2.2;
	float si = sin(t);
	float co = cos(t);
	mat2 ma = mat2(co, si, -si, co);

	float v1, v2, v3;
	v1 = v2 = v3 = 0.0;
	
	float s = 0.0;
	for (int i = 0; i < 90; i++)
	{
		vec3 p = s * vec3(uv, 0.0);
		p.xy *= ma;
		p += vec3(.22, .3, s - 1.5 - sin(iTime * .13) * .1);
		for (int i = 0; i < 8; i++)	p = abs(p) / dot(p,p) - 0.659;
		v1 += dot(p,p) * .0015 * (1.8 + sin(length(uv.xy * 13.0) + .5  - iTime * .2));
		v2 += dot(p,p) * .0013 * (1.5 + sin(length(uv.xy * 14.5) + 1.2 - iTime * .3));
		v3 += length(p.xy*10.) * .0003;
		s  += .035;
	}
	
	float len = length(uv);
	v1 *= smoothstep(.7, .0, len);
	v2 *= smoothstep(.5, .0, len);
	v3 *= smoothstep(.9, .0, len);
	
	vec3 col = vec3( v3 * (1.5 + sin(iTime * .2) * .4),
					(v1 + v3) * .3,
					 v2) + smoothstep(0.2, .0, len) * .85 + smoothstep(.0, .6, v3) * .3;


	return vec4(min(pow(abs(col), vec3(1.2)), 1.0), 1.0);
}

void main(void)
{
	vec3 lightColor = vec3(1, 1, 1);

	vec3 norm = normalize(Normal);
	vec3 lightDir = normalize(lightPos - FragPos);  
	float diff = max(dot(norm, lightDir), 0.0);
	vec3 diffuse = diff * lightColor;
	
	
	vec4 dist =  vec4(vec3(1. / length(lightPos - FragPos)), 1.0);

	outColor = vec4(diffuse, 1.0) * mainImage() * dist; // vec4(0, 0.6, 0.12, 1.0);

}
