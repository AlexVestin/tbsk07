#version 150

out vec4 outColor;
in vec2 texCoord;
uniform sampler2D tex;

in vec3 Normal;
in vec3 FragPos;
uniform vec3 lightPos;
void main(void)
{
	vec3 lightColor = vec3(1, 1, 1);

	vec3 norm = normalize(Normal);
	vec3 lightDir = normalize(lightPos - FragPos);  
	float diff = max(dot(norm, lightDir), 0.0);
	vec3 diffuse = diff * lightColor;

	outColor =  texture(tex, texCoord); // vec4(diffuse, 1.0) *
}
