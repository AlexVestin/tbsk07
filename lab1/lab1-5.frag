#version 150

in vec4 iColor;
out vec4 out_Color;

void main(void) 
{
	out_Color = iColor;
	//out_Color = vec4(1.0, 1.0, 0.3, 1.0);
}
