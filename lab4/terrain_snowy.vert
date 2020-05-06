#version 150

in  vec3 inPosition;
in  vec3 inNormal;
in vec2 inTexCoord;
out vec2 texCoord;
out float height;
// NY
uniform mat4 projMatrix;
uniform mat4 mdlMatrix;
uniform mat4 viewMatrix;


out vec3 FragPos;  
out vec3 Normal;

void main(void)
{
	texCoord = inTexCoord;
	gl_Position = projMatrix * viewMatrix * mdlMatrix * vec4(inPosition, 1.0);
	// stuff
	FragPos = vec3(mdlMatrix * vec4(inPosition, 1.0));
	Normal = inNormal;
	height = inPosition.y;
}
