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


class AnimationShader;

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
	Geometry(Model* model, GLuint program);
	Geometry(const char* modelPath, GLuint program);

	// Desctructor

	virtual ~Geometry();
	void setUpGeometryBuffers();
	void createShader();
	void draw(float t, GLfloat* tranMatrix, GLfloat* camMatrix, GLfloat* camPos, int drawType);
	
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

	int		getInstanceCount() { return instanceCount; };
	GLuint  getVAO() { return vao; };
	Model*  getModel() { return model; };
	GLfloat getSpecularExponent() { return specularExp; };
	GLuint  getTex() { return tex; };

private:
	int instanceCount;

	GLuint  tex;
	GLuint  vao;
	GLuint  vbo;
	GLuint  nbo;
	GLuint  ivbo;
	GLfloat specularExp;
	Model*  model;
	GLuint  program;

};