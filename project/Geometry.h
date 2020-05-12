#pragma once
#include <iostream>
#include "Camera.h"
#include <string>
#include <loadobj.h>
#include "VectorUtils3.h"
#include <vector>
#include "glew.h"
#include "GL_utilities.h"
#include "AnimationShader.h"
#include "LoadTGA.h"
#include <GL/gl.h>



struct GeometryAttributeBuffers {
	int instanceCount;
	std::vector<GLfloat> startPositions;
	std::vector<GLfloat> endPositions;
	std::vector<GLfloat> sizes;
	std::vector<GLfloat> colors;
	std::vector<GLfloat> startTimes;
};


class Geometry {
public:
	Geometry(Model* model);
	Geometry(const char* modelPath);

	// Desctructor

	virtual ~Geometry();
	void testFunction();
	void setUpGeometryBuffers();
	void createShader();
	void assignShader(AnimationShader shader);
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
	// TODO should make an input object instead
	void setUpInstanceBuffers(GeometryAttributeBuffers& attributeBuffers);

	GLuint createParticleTexture();


private:
	int myInt;
	int instanceCount;
<<<<<<< HEAD
	GLuint  vao;
	GLuint  vbo;
	GLuint  nbo;
	GLuint  ivbo;
	GLfloat specularExp;
	Model*  model;
	GLuint  program;
=======
	GLuint vao;
	GLuint vbo;
	GLuint nbo;
	GLuint ivbo;
	Model* model;
	GLuint program;
	GLuint tex;

>>>>>>> d87eb666f34384fed3b2ff626a487d93440c38e6
};