
#pragma once
#include "Geometry.h"
#include <iostream>

// Useful if loaded using LoadModelFromXXX
Geometry::Geometry(Model* model) : model(model) {
	setUpBuffers();
};


Geometry::Geometry(const char* modelPath) {
	model = LoadModelPlus(modelPath);
	setUpBuffers();
};

void Geometry::setUpBuffers() {
	int i;
	glGenVertexArrays(1, &vao);
	glBindVertexArray(vao);

	// Set up buffer objects to copy model data to instance
	// Vertex buffer
	glGenBuffers(1, &vbo);
	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glBufferData(GL_ARRAY_BUFFER, model->numVertices * 3 * sizeof(GLfloat), model->vertexArray, GL_STATIC_DRAW);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);
	glEnableVertexAttribArray(0);
	glVertexAttribDivisor(0, 0);

	// Normal buffers
	glGenBuffers(1, &nbo);
	glBindBuffer(GL_ARRAY_BUFFER, nbo);
	glBufferData(GL_ARRAY_BUFFER, model->numVertices * 3 * sizeof(GLfloat), model->normalArray, GL_STATIC_DRAW);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, 0);
	glEnableVertexAttribArray(1);
	glVertexAttribDivisor(1, 0);

	// indices
	glGenBuffers(1, &ivbo);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ivbo);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, model->numIndices * sizeof(GLuint), model->indexArray, GL_STATIC_DRAW);
}

// Destructor, todo cleanup
Geometry::~Geometry() {
	std::cout << "Bye!" << std::endl;
}

void Geometry::testFunction() {
	std::cout << myInt << std::endl;
}

