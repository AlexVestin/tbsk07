#version 150

out vec4 outColor;
in vec2 texCoords;
uniform sampler2D tex11;

void main() {
    outColor = texture(tex11, texCoords*300.);
}