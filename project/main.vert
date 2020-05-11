#version 150

in vec3 in_Position;
in vec3 in_Normal;
in vec3 startPos;
in vec3 endPos;
in float startTime;


float duration = 4.0;


uniform mat4 tranMatrix, projMatrix, camMatrix;
uniform mat4 rot, trans, total;

uniform float time; 

out vec3 transNormal;
out vec2 texCoord;
out vec3 fragPosition;

void main(void)
{
	vec3 diff = endPos - startPos;
	vec3 pos = in_Position +  startPos + mod((time - startTime), duration) * diff;


	mat3 normalMatrix = mat3(tranMatrix);
	gl_Position		  = projMatrix * camMatrix * tranMatrix * vec4(pos, 1.0);
	fragPosition	  = vec3(tranMatrix * vec4(pos, 1.0));  //The fragment's position in world space
	transNormal		  = normalMatrix * in_Normal;
}
