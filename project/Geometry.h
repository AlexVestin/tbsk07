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


enum class DrawFunc {
	ARRAY,
	ELEMENTS
};


struct GeometryAttributeBuffers {
	int instanceCount;
	float duration;
	bool repeat;
	std::vector<GLfloat> startPositions;
	std::vector<GLfloat> endPositions;
	std::vector<GLfloat> sizes;
	std::vector<GLfloat> colors;
	std::vector<GLfloat> startTimes;
};

class Geometry {
public:
	Geometry(GeometryAttributeBuffers attributes, GLuint program, GLuint drawType = GL_TRIANGLES);
	Geometry(GeometryAttributeBuffers attributes, GLuint program, GLuint drawType, DrawFunc drawFunc);
	Geometry(GeometryAttributeBuffers attributes, Model* model, GLuint program, GLuint drawType = GL_TRIANGLES);
	Geometry(GeometryAttributeBuffers attributes, const char* modelPath, GLuint program, GLuint drawType = GL_TRIANGLES);

	// Desctructor

	virtual ~Geometry();
	void setUpGeometryBuffers();
	void draw(float t, GLfloat* tranMatrix, GLfloat* camMatrix, GLfloat* camPos);


	static const std::vector<GLfloat> RandomPositions(int instanceCount, float spread, float c = 0) {
		std::vector<GLfloat> positions(instanceCount);
		float rx = ((double)rand() / (RAND_MAX)) - 0.5;
		float ry = ((double)rand() / (RAND_MAX)) - 0.5;
		float rz = ((double)rand() / (RAND_MAX)) - 0.5;
		for (int i = 0; i < instanceCount; i++) {
			positions[i * 3] = rx*spread + c;
			positions[(i * 3) + 1] = ry* spread + c;
			positions[(i * 3)] = rx * spread + c;

		}
		return positions;
	}
	
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
	GLuint createParticleTexture();

	int		getInstanceCount() { return instanceCount; };
	GLuint  getVAO() { return vao; };
	Model*  getModel() { return model; };
	GLfloat getSpecularExponent() { return specularExp; };
	GLuint  getTex() { return tex; };

private:

	void setUpInstanceBuffers(std::vector<GLfloat>& startPositions);
	void setUpInstanceBuffers(std::vector<GLfloat>& startPositions, std::vector<GLfloat>& endPositions);
	// TODO should make an input object instead
	void setUpInstanceBuffers(GeometryAttributeBuffers& attributeBuffers);
	int instanceCount;
	DrawFunc drawFunc;
	GLuint drawType;
	GLuint  tex;
	GLuint  vao;
	GLuint  vbo;
	GLuint  nbo;
	GLuint  ivbo;
	GLfloat specularExp;
	Model*  model;
	GLuint  program;

};