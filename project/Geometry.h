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
	void draw(float t, GLfloat* cameraMatrix, GLfloat* camPos);
	
	template <typename T = GLfloat>
	int createBuffer(std::vector<typename T> data, GLuint attribLocation, GLuint elementSize) {
		GLuint buf;
		glGenBuffers(1, &buf);
		glBindBuffer(GL_ARRAY_BUFFER, buf);
		glBufferData(GL_ARRAY_BUFFER, data.size() * sizeof(T), &data[0], GL_STATIC_DRAW);

		glEnableVertexAttribArray(attribLocation);
		glVertexAttribPointer(attribLocation, elementSize, GL_FLOAT, GL_FALSE, 0, 0);
		glVertexAttribDivisor(attribLocation, 1);
		return buf;
	}

	void setUpInstanceBuffers(std::vector<GLfloat>& startPositions);
	void setUpInstanceBuffers(std::vector<GLfloat>& startPositions, std::vector<GLfloat>& endPositions);
	void setUpInstanceBuffers(std::vector<GLfloat>& buf1, std::vector<GLfloat>& buf2, std::vector<GLfloat>& buf3);



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