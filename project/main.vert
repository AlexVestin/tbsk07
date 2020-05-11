#version 150

in vec3  in_Position;
in vec3  in_Normal;
in vec3 offset;

uniform mat4 tranMatrix, projMatrix, camMatrix;
uniform mat4 rot, trans, total;

out vec3 transNormal;
out vec2 texCoord;
out vec3 fragPosition;

void main(void)
{
	vec3 pos = in_Position + offset;
	mat3 normalMatrix = mat3(tranMatrix);
	gl_Position		  = projMatrix * camMatrix * tranMatrix * vec4(pos, 1.0);
	fragPosition	  = vec3(tranMatrix * vec4(pos, 1.0));  //The fragment's position in world space
	transNormal		  = normalMatrix * in_Normal;
}
