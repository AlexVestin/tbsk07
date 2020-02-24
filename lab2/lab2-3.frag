#version 150

in vec4 iColor;
in vec2 texCoord;
out vec4 out_Color;
uniform float time;
uniform sampler2D texUnit;


void main(void) 
{
	out_Color = texture(texUnit, texCoord);
}
