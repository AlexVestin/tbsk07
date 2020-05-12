#version 330

in vec3 in_Position;
in vec3 in_Normal;
in vec3 startPos;
in vec3 endPos;
in float startTime;
in float size;
in vec3 inCol;

#define PI 3.14159265359

float duration = 300.;


uniform mat4 tranMatrix, projMatrix, camMatrix;
uniform mat4 rot, trans, total;

uniform float time; 

out vec3 transNormal;
out vec2 texCoord;
out vec3 fragPosition;
out vec4 col;

// https://gist.github.com/patriciogonzalezvivo/670c22f3966e662d2f83
float rand(float n){return fract(sin(n) * 43758.5453123);}

float noise(float p){
	float fl = floor(p);
  float fc = fract(p);
	return mix(rand(fl), rand(fl + 1.0), fc);
}

float rand2(vec2 n) { 
	return fract(sin(dot(n, vec2(12.9898, 4.1414))) * 43758.5453);
}

float noise2(vec2 n) {
	const vec2 d = vec2(0.0, 1.0);
    vec2 b = floor(n), f = smoothstep(vec2(0.0), vec2(1.0), fract(n));
	return mix(mix(rand2(b), rand2(b + d.yx), f.x), mix(rand2(b + d.xy), rand2(b + d.yy), f.x), f.y);
}


void main(void)
{
	col = vec4(inCol.xyz, 1.0);
	vec3 diff = endPos - startPos;
	float dt = time - startTime;
	dt =  dt > 0 ? dt : 0;
	float progress =  mod(dt, duration) / duration;


	vec3 pos = (in_Position + startPos) + progress * diff;
	// TODO: make a string builder that inserts here?
	// ------------------------------------------------------------------------------------
	// Make a value that goes [0 ... 1.0 ... 0]
	float noiseAmt = progress < 0.5 ? progress * 2. : 2. - progress * 2.0;
		
	float noiseT = time/2.0;
	float noise_x = noise2(pos.xy* noise(noiseT)) - 0.5;
	float noise_y = noise2(pos.xz* noise(noiseT)) - 0.5;
	float noise_z = noise2(pos.yz* noise(noiseT)) - 0.5;

	pos += noiseAmt * 25.* vec3(noise_x, noise_y, noise_z);
	// -----------------------------------------------------------------------------------------------------------



	mat3 normalMatrix = mat3(tranMatrix);
	gl_Position		  = projMatrix * camMatrix * tranMatrix * vec4(pos, 1.0);
	fragPosition	  = vec3(tranMatrix * vec4(pos, 1.0));  //The fragment's position in world space
	transNormal		  = normalMatrix * in_Normal;
	gl_PointSize = size;
}
