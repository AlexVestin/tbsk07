#version 150

in vec3 exColor;
out vec4 outColor;
in vec2 texCoord;
uniform sampler2D texUnit;
void main(void)
{
 outColor = vec4(exColor,1.0) * texture(texUnit, texCoord);
}
