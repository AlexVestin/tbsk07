#pragma once
#include <iostream>
#include <string>
#include <loadobj.h>
#include "VectorUtils3.h"
#include <vector>
#include "Geometry.h"
#include "glew.h"
#include "GL_utilities.h"

#include <GL/gl.h>

class Geometry;

class AnimationShader
{
public:
	// Constructors
	AnimationShader();
	AnimationShader(std::string customCodeSnippet);
	AnimationShader(
		GLuint lightNo, 
		vec3   lightSourcesColorsArr[], 
		GLint  isDirectional[], 
		vec3   lightSourcesDirectionsPositions[]
	);
	AnimationShader(
		GLuint lightNo,
		vec3   lightSourcesColorsArr[],
		GLint  isDirectional[],
		vec3   lightSourcesDirectionsPositions[],
		std::string customCodeSnippet
	);

	// Public methods
	
	// Compiles the shader.
	void load();
	void load(std::string customCodeSnippet);

	// Sets the shadingMode that we'll use.
	void setShadingMode(GLint shadingMode);

	// Sets the lightSources for the shader.
	void setLightSources(
		GLuint lightNo,
		vec3   lightSourcesColorsArr[],
		GLint  isDirectional[],
		vec3   lightSourcesDirectionsPositions[]
	);

	// Draws the given object.
	void draw(Geometry *model, float t, GLfloat* tranMatrix, GLfloat* camMatrix, GLfloat* camPos);

	GLuint getProgram() { return program; };

private:
	// The type of shading (Flat, Lambert or Phong).
	GLint shadingMode;
	// Number of light sources.
	GLuint lightNo;
	// Color values for the light sources.
	vec3 *lightSourcesColorsArr;
	// If they're directional or not.
	GLint *isDirectional;
	// Their direction/position.
	vec3 *lightSourcesDirectionsPositions;
	// Reference to the shader program.
	GLuint  program;

	// uploads the light and shadingMode data into the shader.
	void uploadValues();
};

