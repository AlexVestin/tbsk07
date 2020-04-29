#version 150

in vec3 exColor;
out vec4 outColor;
in vec2 texCoord;
in vec3 pos;
uniform vec3 color;
uniform sampler2D texUnit;

// normals
in vec3 normals;

// Lighting
uniform vec3 viewPos;
uniform vec3 lightSourcesDirPosArr[4];
uniform vec3 lightSourcesColorArr[4];
uniform float specularExponent;
uniform bool isDirectional[4];
float specularStrength = 0.05;

void main(void) {

    vec3 viewDir = normalize(viewPos - pos);
    vec3 reflectDir = reflect(-lightSourcesDirPosArr[0], normals);  
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), 2);
    vec3 specular = specularStrength * spec * lightSourcesColorArr[0];  

    outColor = vec4(specular, 1.0) +  vec4(exColor,1.0);//* vec4(139./255.,69./255.,19./255., 1.0);
}
