#version 150

in vec3 exColor;
out vec4 outColor;
in vec2 texCoord;
uniform vec3 color;
uniform sampler2D texUnit;
void main(void)
{
 outColor = vec4(color, 1.0) * vec4(exColor,1.0);//* vec4(139./255.,69./255.,19./255., 1.0);
}
