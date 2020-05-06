#version 150

uniform sampler2D skyTex;

in vec2 texCoord;
in vec3 transNormal;
out vec4 out_Color;

void main(void)
{ 
	out_Color = texture(skyTex, texCoord);
}
