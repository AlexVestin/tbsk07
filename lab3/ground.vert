#version 150
uniform mat4 model, view, proj;
in vec2 inPosition;
in vec2 inTexCoords;
out vec2 texCoords;
void main(void)
{
    const vec2 madd=vec2(0.5,0.5);
    texCoords = inPosition.xy*madd+madd;
    
    //texCoords =  inTexCoords;
	gl_Position =  proj * view * model * vec4(inPosition.x, 0., inPosition.y, 1.0);
}
