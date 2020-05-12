#pragma once
#include <iostream>
#include <string>
#include <loadobj.h>
#include "VectorUtils3.h"
#include <vector>
#include "Geometry.h"
#include "glew.h"
#include "GL_utilities.h"
#include <string>
#include <fstream>
#include <streambuf>
#include <sstream>

#include <GL/gl.h>
class Geometry;

class Geometry;

struct AnimationShaderInput {
	std::vector<std::string> vertexDefines;
	std::vector<std::string> vertexSnippets;
	std::vector<std::string> vertexFinals;
};


class AnimationShader
{
public:
	// Constructors
	AnimationShader();
	AnimationShader(AnimationShaderInput customCodeSnippet);

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
	void load(AnimationShaderInput asi);

	// Sets the shadingMode that we'll use.
	void setShadingMode(GLint shadingMode);

	// Sets the lightSources for the shader.
	void setLightSources(
		GLuint lightNo,
		vec3   lightSourcesColorsArr[],
		GLint  isDirectional[],
		vec3   lightSourcesDirectionsPositions[]
	);

	static constexpr char* NoiseMovement = 
			"float noiseT = time / 2.0;"
			"float noise_x = noiseVec2(pos.xy * noise(noiseT)) - 0.5;\n"
			"float noise_y = noiseVec2(pos.xz * noise(noiseT)) - 0.5;\n"
			"float noise_z = noiseVec2(pos.yz * noise(noiseT)) - 0.5;\n"
			"pos += noiseAmt * 25. * vec3(noise_x, noise_y, noise_z);\n";

	// Draws the given object.
	void draw(Geometry *model, float t, GLfloat* tranMatrix, GLfloat* camMatrix, GLfloat* camPos);

	GLuint getProgram() { return program; };

private:
	std::string AnimationShader::prepareVertex(AnimationShaderInput asi);


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

