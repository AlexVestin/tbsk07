#version 150

out vec4 outColor;
in vec2 texCoord;
uniform sampler2D tex;
uniform sampler2D snowTex;

uniform float maxheight;
in float height;

in vec3 Normal;
in vec3 FragPos;
uniform vec3 lightPos;
void main(void)
{
	vec3 lightColor = vec3(0, 1, 1);

	vec3 norm = normalize(Normal*-1.0);
	vec3 lightDir = normalize(lightPos - FragPos);  
	float diff = max(dot(norm, lightDir), 0.0);
	vec3 diffuse = diff * lightColor;


	float a = height / (maxheight*2.); 
	float dist =  1. / length(lightPos - FragPos);
	outColor = vec4(diffuse, 1.0) *  mix(texture(tex, texCoord), texture(snowTex, texCoord), a) * vec4(vec3(dist)*2.0, 1.0);
}
