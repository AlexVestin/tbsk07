#version 150

in vec4 iColor;
out vec4 out_Color;

void main(void) 
{
	out_Color = iColor;//;vec4( (length(iColor.xyz) / 3.0) * vec3(1.), 1.0);
	//out_Color = vec4(1.0, 1.0, 0.3, 1.0);
}
