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
	AnimationShader(
		GLuint lightNo, 
		vec3   lightSourcesColorsArr[], 
		GLint  isDirectional[], 
		vec3   lightSourcesDirectionsPositions[]
	);

	// Destructor
	~AnimationShader() {
		delete[] lightSourcesColorsArr;
		delete[] isDirectional;
		delete[] lightSourcesDirectionsPositions;
	}

	// Public methods
	
	void setShadingMode(GLint shadingMode);

	void setLightSources(
		GLuint lightNo,
		vec3   lightSourcesColorsArr[],
		GLint  isDirectional[],
		vec3   lightSourcesDirectionsPositions[]
	);

	void draw(Geometry model, float t, GLfloat* camMatrix, GLfloat* camPos);

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
};

