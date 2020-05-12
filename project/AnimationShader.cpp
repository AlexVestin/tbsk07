#include "AnimationShader.h"

// The default shading mode
GLint DEFAULT_MODE = 2;
// The default values of our light sources.
GLuint  DEFAULT_LIGHT_NO			 = 2;
GLfloat DEFAULT_SPEC_EXP[]			 = { 100.0f };
vec3	DEFAULT_COLORS[]			 = { { 1.0f, 1.0f, 1.0f }, { 1.0f, 0.0f, 0.0f } };
GLint	DEFAULT_DIRECTIONAL[]		 = { 1, 1 };
vec3	DEFAULT_DIRECTION_POSITION[] = { { 1.0f, 1.0f, 0.0f }, { 0.0f,-1.0f, 1.0f } };

AnimationShader::AnimationShader() {

}

AnimationShader::AnimationShader(GLuint lightNo, vec3 lightSourcesColorsArr[], GLint isDirectional[], vec3 lightSourcesDirectionsPositions[]) {

}