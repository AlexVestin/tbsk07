#version 150

in vec4 iColor;
in vec2 texCoord;
out vec4 out_Color;
uniform float time;
void main(void) 
{
	out_Color = vec4(texCoord.xy, 0.0, 1.0);
	//Top Right
	if(texCoord.x > 0.5 && texCoord.y > 0.5) out_Color = vec4(1.0, 1.0, 0.0, 1.0);
	// Top Left
	if(texCoord.x < 0.5 && texCoord.y > 0.5) out_Color = vec4(0.0, 1.0, 0.0, 1.0);
	// Bot left 
	if(texCoord.x < 0.5 && texCoord.y < 0.5) out_Color = vec4(0.0, 0.0, 1.0, 1.0);
	// Bot right
	if(texCoord.x > 0.5 && texCoord.y < 0.5) out_Color = vec4(1.0, 0.0, 0.0, 1.0);
	

	
	//out_Color = vec4(1.0, 1.0, 0.3, 1.0);
}
