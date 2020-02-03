#version 150

out vec4 out_Color;
in vec3 o_Color;
void main(void) 
{
	out_Color = vec4(o_Color, 1.0);
}
