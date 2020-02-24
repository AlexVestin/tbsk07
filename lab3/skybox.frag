#version 150

in vec3 exColor;
out vec4 outColor;
in vec2 texCoord;
uniform vec3 color;
uniform sampler2D texUnit;
void main(void) {
 outColor =  texture(texUnit, texCoord); //vec4(1.0, 0.0, 0., 1.0);// texture(texUnit, texCoord) * vec4(exColor,1.0);
}
