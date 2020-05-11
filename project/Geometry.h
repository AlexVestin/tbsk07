#pragma once
#include <iostream>
#include <string>
#include <loadobj.h>
#include "VectorUtils3.h"
#include <vector>
#include "glew.h"
#include "GL_utilities.h"

#include <GL/gl.h>


#define near 1.0
#define far 300.0
#define right 0.5
#define left -0.5
#define top 0.5
#define bottom -0.5


class Geometry {
public:
	Geometry(Model* model);
	Geometry(const char* modelPath);

	// Desctructor

	virtual ~Geometry();
	void testFunction();
	void setUpGeometryBuffers();
	void createShader();
	void draw(float t, GLfloat* cameraMatrix);
	void setUpInstanceBuffers(std::vector<GLfloat>& startPositions);

private:
	std::vector<GLfloat> projectionMatrix;
	int myInt;
	int instanceCount;
	GLuint vao;
	GLuint vbo;
	GLuint nbo;
	GLuint ivbo;
	Model* model;
	GLuint program;
};