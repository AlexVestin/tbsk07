#version 150

in vec3 inPosition;
in vec3 inNormal;
in mat4 mdlMatrix;

// NY
uniform mat4 projMatrix;
uniform mat4 viewMatrix;
//uniform mat4 mdlMatrix;


out vec3 FragPos;  
out vec3 Normal;
out vec3 Position;

void main(void)
{
    vec3 offset = vec3(mdlMatrix[0][3], mdlMatrix[1][3], mdlMatrix[2][3]);

	gl_Position = projMatrix * viewMatrix *  mdlMatrix * vec4(inPosition , 1.0);

	// stuff
    FragPos = vec3(mdlMatrix * vec4(inPosition, 1.0));
	Position = inPosition;
	Normal = inNormal;
}
