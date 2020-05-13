#version 330 core

in vec3 in_Position;
in vec3 in_Normal;
in vec3 startPos;
in vec3 endPos;
in float startTime;
in float size;
in vec3 inCol;
in vec2 inTexCoord;

#define PI 3.14159265359

float duration = 20.0;

#define NUM_OCTAVES 5

// Noise functions from
// https://gist.github.com/patriciogonzalezvivo/670c22f3966e662d2f83
float rand(float n){return fract(sin(n) * 43758.5453123);}

float rand(vec2 n) { 
	return fract(sin(dot(n, vec2(12.9898, 4.1414))) * 43758.5453);
}


float noise(float p){
	float fl = floor(p);
	float fc = fract(p);
	return mix(rand(fl), rand(fl + 1.0), fc);
}


float noise(vec2 n) {
	const vec2 d = vec2(0.0, 1.0);
	vec2 b = floor(n), f = smoothstep(vec2(0.0), vec2(1.0), fract(n));
	return mix(mix(rand(b), rand(b + d.yx), f.x), mix(rand(b + d.xy), rand(b + d.yy), f.x), f.y);
}

float noiseVec2(vec2 n) {
	const vec2 d = vec2(0.0, 1.0);
    vec2 b = floor(n), f = smoothstep(vec2(0.0), vec2(1.0), fract(n));
	return mix(mix(rand(b), rand(b + d.yx), f.x), mix(rand(b + d.xy), rand(b + d.yy), f.x), f.y);
}

float fbm(float x) {
	float v = 0.0;
	float a = 0.5;
	float shift = float(100);
	for (int i = 0; i < NUM_OCTAVES; ++i) {
		v += a * noise(x);
		x = x * 2.0 + shift;
		a *= 0.5;
	}
	return v;
}


float fbm(vec2 x) {
	float v = 0.0;
	float a = 0.5;
	vec2 shift = vec2(100);
	// Rotate to reduce axial bias
    mat2 rot = mat2(cos(0.5), sin(0.5), -sin(0.5), cos(0.50));
	for (int i = 0; i < NUM_OCTAVES; ++i) {
		v += a * noise(x);
		x = rot * x * 2.0 + shift;
		a *= 0.5;
	}
	return v;
}

float mod289(float x){return x - floor(x * (1.0 / 289.0)) * 289.0;}
vec4 mod289(vec4 x){return x - floor(x * (1.0 / 289.0)) * 289.0;}
vec4 perm(vec4 x){return mod289(((x * 34.0) + 1.0) * x);}

float noise(vec3 p){
    vec3 a = floor(p);
    vec3 d = p - a;
    d = d * d * (3.0 - 2.0 * d);

    vec4 b = a.xxyy + vec4(0.0, 1.0, 0.0, 1.0);
    vec4 k1 = perm(b.xyxy);
    vec4 k2 = perm(k1.xyxy + b.zzww);

    vec4 c = k2 + a.zzzz;
    vec4 k3 = perm(c);
    vec4 k4 = perm(c + 1.0);

    vec4 o1 = fract(k3 * (1.0 / 41.0));
    vec4 o2 = fract(k4 * (1.0 / 41.0));

    vec4 o3 = o2 * d.z + o1 * (1.0 - d.z);
    vec2 o4 = o3.yw * d.x + o3.xz * (1.0 - d.x);

    return o4.y * d.y + o4.x * (1.0 - d.y);
}


float fbm(vec3 x) {
	float v = 0.0;
	float a = 0.5;
	vec3 shift = vec3(100);
	for (int i = 0; i < NUM_OCTAVES; ++i) {
		v += a * noise(x);
		x = x * 2.0 + shift;
		a *= 0.5;
	}
	return v;
}

uniform mat4 tranMatrix, projMatrix, camMatrix;
uniform mat4 rot, trans, total;
uniform int instanceCount;
uniform float time; 
uniform bool repeat;

out vec3 transNormal;
out vec2 texCoord;
out vec3 fragPosition;
out vec4 col;

// --- BREAKPOINT-DEFINES --- //	


void main(void) {
	col = vec4(inCol.xyz, 1.0);
	vec3 diff = endPos - startPos;
	float dt = time - startTime;
	if(!repeat) {
		dt =  dt > 0 ? dt : 0;
	} 
	
	
	float progress =  mod(dt, duration) / duration;

	// Make a value that goes [0 ... 1.0 ... 0]
	float noiseAmt = progress < 0.5 ? progress * 2. : 2. - progress * 2.0;		
	vec3 pos = (in_Position + startPos) + progress * diff;
	vec3 _pos = pos;

	// --- BREAKPOINT-SNIPPETS --- //


	mat3 normalMatrix = mat3(tranMatrix);
	gl_Position		  = projMatrix * camMatrix * tranMatrix * vec4(pos, 1.0);
	fragPosition	  = vec3(tranMatrix * vec4(pos, 1.0));  //The fragment's position in world space
	transNormal		  = normalMatrix * in_Normal;
	texCoord		  = inTexCoord;
	gl_PointSize =  size;

	// --- BREAKPOINT-FINAL --- //	

}
