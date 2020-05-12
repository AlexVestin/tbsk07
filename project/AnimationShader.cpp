#include "AnimationShader.h"

#define FLAT_SHADING 0
#define LAMBERT_SHADING 1
#define PHONG_SHADING 2

// The default shading mode
GLint	DEFAULT_SHADING = FLAT_SHADING;
// The default values of our light sources.
GLuint  DEFAULT_LIGHT_NO			 = 2;
vec3	DEFAULT_COLORS[]			 = { { 1.0f, 1.0f, 1.0f }, { 1.0f, 0.0f, 0.0f } };
GLint	DEFAULT_DIRECTIONAL[]		 = { 1, 1 };
vec3	DEFAULT_DIRECTION_POSITION[] = { { 1.0f, 1.0f, 0.0f }, { 0.0f,-1.0f, 1.0f } };

// The default constructor.
AnimationShader::AnimationShader() {
	shadingMode						= DEFAULT_SHADING;
	lightNo							= DEFAULT_LIGHT_NO;
	lightSourcesColorsArr			= DEFAULT_COLORS;
	isDirectional					= DEFAULT_DIRECTIONAL;
	lightSourcesDirectionsPositions = DEFAULT_DIRECTION_POSITION;
	load();
}

// The constructor for when you want controll of the light.
AnimationShader::AnimationShader(GLuint lightNo, vec3 lightSourcesColorsArr[], GLint isDirectional[], vec3 lightSourcesDirectionsPositions[]) {
	shadingMode							  = DEFAULT_SHADING;
	this->lightNo						  = lightNo;
	this->lightSourcesColorsArr			  = lightSourcesColorsArr;
	this->isDirectional					  = isDirectional;
	this->lightSourcesDirectionsPositions = lightSourcesDirectionsPositions;
	load();
}

// These constructors are for when you want to customize the shaders.
AnimationShader::AnimationShader(std::string customCodeSnippet) : AnimationShader() {
}

AnimationShader::AnimationShader(GLuint lightNo, vec3 lightSourcesColorsArr[], GLint isDirectional[], vec3 lightSourcesDirectionsPositions[], std::string customCodeSnippet) :
				 AnimationShader(lightNo, lightSourcesColorsArr, isDirectional, lightSourcesColorsArr) {
}

void AnimationShader::setShadingMode(GLint shadingMode) {
	this->shadingMode = shadingMode;
	uploadValues();
}

void AnimationShader::setLightSources(GLuint lightNo, vec3 lightSourcesColorsArr[], GLint isDirectional[], vec3 lightSourcesDirectionsPositions[]) {
	this->lightNo = lightNo;
	this->lightSourcesColorsArr = lightSourcesColorsArr;
	this->isDirectional = isDirectional;
	this->lightSourcesDirectionsPositions = lightSourcesDirectionsPositions;
	uploadValues();
}

/*
void AnimationShader::draw(Geometry *geom, float t, GLfloat* tranMatrix, GLfloat* camMatrix, GLfloat* camPos) {
	glUseProgram(program);
	glBindVertexArray(geom->getVAO());

	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, geom->getTex());
	
	// Update uniforms
	// We pass a camera (view) matrix.
	glUniformMatrix4fv(glGetUniformLocation(program, "camMatrix"), 1, GL_TRUE, camMatrix);

	// We pass its position (Is needed in the fragment shader).
	glUniform3fv(glGetUniformLocation(program, "camPos"), 1, camPos);

	// Then we pass the transformation matrix
	glUniformMatrix4fv(glGetUniformLocation(program, "tranMatrix"), 1, GL_TRUE, tranMatrix);
	
	// Then we pass the specular exponent (how shiny it is) and the elapsed time.
	glUniform1f(glGetUniformLocation(program, "specularExponent"), geom->getSpecularExponent());
	glUniform1f(glGetUniformLocation(program, "time"), t / 1000.);

	glDepthMask(false);
	glDrawElementsInstanced(GL_POINTS, geom->getModel()->numIndices, GL_UNSIGNED_INT, 0L, geom->getInstanceCount());
}
*/
void AnimationShader::load() {
	program = loadShaders("main.vert", "main.frag");
	glUseProgram(program);
	glUniformMatrix4fv(glGetUniformLocation(program, "projMatrix"), 1, GL_TRUE, &Camera::projectionMatrix[0]);

	uploadValues();
}

void AnimationShader::load(std::string customCodeSnippet) {
	load();
}

void AnimationShader::uploadValues() {

	glUniform1i(glGetUniformLocation(program, "shadingMode"), shadingMode);
	
	GLuint loc = glGetUniformLocation(program, "lightSourcesDirPosArr");
	glUniform3fv(loc, lightNo, &lightSourcesDirectionsPositions[0].x);

	glUniform3fv(glGetUniformLocation(program, "lightSourcesColorArr"), lightNo, &lightSourcesColorsArr[0].x);

	glUniform1iv(glGetUniformLocation(program, "isDirectional"), lightNo, isDirectional);

	glUniform1i(glGetUniformLocation(program, "lightSourcesNo"), lightNo);
}


