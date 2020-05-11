#pragma once
#include <iostream>
#include <string>
#include <loadobj.h>

#include "glew.h"
#include <GL/gl.h>

class Geometry {
public:
	Geometry(Model* model);
	Geometry(const char* modelPath);

	// Desctructor

	virtual ~Geometry();
	void testFunction();
	void setUpBuffers();

private:
	int myInt;
	GLuint vao;
	GLuint vbo;
	GLuint nbo;
	GLuint ivbo;
	Model* model;
};