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

	static const std::string Tube(float radius = 2.0, float tdiv = 0.02, float distance = 20.0) {

		std::ostringstream ss;
		ss << "float radius = " << radius << ";\n";
		ss << "float tdiv = " << tdiv << ";\n";
		ss << "float distance = " << distance<< ";\n";
		ss << "pos = vec3(sin(dt / tdiv) * radius, cos(dt / tdiv) * radius, progress * distance);\n";

		std::cout << ss.str() << std::endl;
		return ss.str();
	}

	static const std::string NoiseMovement(float speed = 12.0, float amp = 20.0) {
		std::ostringstream ss;
		ss << "float noiseT = time / " << speed << ";\n";
		ss << "float noise_x = noiseVec2(pos.xy * noise(noiseT)) - 0.5;\n";
		ss << "float noise_y = noiseVec2(pos.xz * noise(noiseT)) - 0.5;\n";
		ss << "float noise_z = noiseVec2(pos.yz * noise(noiseT)) - 0.5;\n";
		ss << "pos += noiseAmt *" <<  amp << "* vec3(noise_x, noise_y, noise_z);\n";
		return ss.str();
	}

	static const std::string Merge(float weight = 0.4) {
		std::ostringstream ss;
		ss << "float weight = " << weight << ";\n";
		ss << "pos = weight * (1. - progress) * _pos + progress * pos * (1. / weight);";
		return ss.str();
	}

	static const std::string AddForce(float x = 0.33, float y = 0.33, float z = 0.33) {
		std::ostringstream ss;
		ss << "vec3 _af = vec3(" << x << "* time , " << y << " * time , " << z << " * time);\n";
		ss << "pos += _af;";
		return ss.str();
	}


	static const std::string MultiplyForce(float x = 0.33, float y = 0.33, float z = 0.33) {
		std::ostringstream ss;
		ss << "vec3 _mf = vec3(" << x << "* time , " << y << " * time , " << z << " * time);\n";
		ss << "pos *= _mf;";
		return ss.str();
	}

	static const std::string Gravity(float force = 100.0) {
		std::ostringstream ss;

		ss << "float _gpull = "<< force << " * progress * progress;\n";
		ss << "pos.y -= _gpull ;\n";
		return ss.str();
	}



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

