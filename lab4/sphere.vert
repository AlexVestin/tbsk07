#version 150

in vec3 inPosition;
in vec3 inNormal;
in vec2 inTexCoord;

// NY
uniform mat4 projMatrix;
uniform mat4 viewMatrix;
uniform mat4 mdlMatrix;


out vec3 FragPos;  
out vec3 Normal;
out vec2 TexCoord;
out vec3 Position;

void main(void)
{
	mat3 normalMatrix1 = mat3(mdlMatrix);
	gl_Position = projMatrix * viewMatrix * mdlMatrix * vec4(inPosition, 1.0);

	// stuff
	FragPos = vec3(mdlMatrix * vec4(inPosition, 1.0));
	Position = inPosition;
	Normal = inNormal;
	TexCoord = inTexCoord;
}
