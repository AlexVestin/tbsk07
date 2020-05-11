
#pragma once
#include "Geometry.h"


#define FLAT_SHADING 0
#define LAMBERT_SHADING 1
#define PHONG_SHADING 2




// Useful if loaded using LoadModelFromXXX
Geometry::Geometry(Model* model) : model(model) {
	glGenVertexArrays(1, &vao);
	setUpGeometryBuffers();
	createShader();

};


Geometry::Geometry(const char* modelPath) {
	model = LoadModelPlus(modelPath);
	glGenVertexArrays(1, &vao);
	setUpGeometryBuffers();
	createShader();
};

void Geometry::setUpGeometryBuffers() {
	program = loadShaders("main.vert", "main.frag");
	glUseProgram(program);

	projectionMatrix = std::vector<GLfloat>(
		{ 2.0f * near / (right - left), 0.0f, (right + left) / (right - left), 0.0f,
	    0.0f, 2.0f * near / (top - bottom), (top + bottom) / (top - bottom), 0.0f,
		0.0f, 0.0f, -(far + near) / (far - near), -2 * far * near / (far - near),
		0.0f, 0.0f, -1.0f, 0.0f }
	);

	int i;
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


void Geometry::createShader() {


	// Light sources and their variables.
	GLuint lightNo = 2;

	// Specular exponents for each object.
	GLfloat specularExponent[] = { 100.0 };

	// Values for light sources.
	vec3 lightSourcesColorsArr[] = {
		{1.0f, 1.0f, 1.0f},  // White light
		{1.0f, 0.0f, 0.0f}	 // Red light
	};

	GLint isDirectional[] = { 1, 1 };

	vec3 lightSourcesDirectionsPositions[] = {
		{1.0f, 1.0f, 0.0f},
		{0.0f,-1.0f, 1.0f}
	};

	glUseProgram(program);
	glUniformMatrix4fv(glGetUniformLocation(program, "projMatrix"), 1, GL_TRUE, &projectionMatrix[0]);
		
	// Upload light data
	GLuint loc = glGetUniformLocation(program, "lightSourcesDirPosArr");

	glUniform3fv(loc, lightNo, &lightSourcesDirectionsPositions[0].x);
	glUniform3fv(glGetUniformLocation(program, "lightSourcesColorArr"), lightNo, &lightSourcesColorsArr[0].x);


	glUniform1iv(glGetUniformLocation(program, "isDirectional"), lightNo, isDirectional);
	glUniform1i(glGetUniformLocation(program, "lightSourcesNo"), lightNo);
}



void Geometry::draw(float t, GLfloat* camMatrix) {
	glUseProgram(program);
	glBindVertexArray(vao);
		
	GLfloat specularExponent[] = { 100.0 };

	// Update unfiforms
	
	glUniformMatrix4fv(glGetUniformLocation(program, "camMatrix"), 1, GL_TRUE, camMatrix);
	glUniform1i(glGetUniformLocation(program, "shadingMode"), FLAT_SHADING);
	mat4 trans = T(0, 0, 0);
	mat4 rot = Mult(Rx(-M_PI / 2), Rz(t / 2500)); // The teapot object is on the side.
	glUniformMatrix4fv(glGetUniformLocation(program, "tranMatrix"), 1, GL_TRUE, Mult(trans, rot).m);
	glUniform1f(glGetUniformLocation(program, "specularExponent"), specularExponent[0]);


	glDrawElementsInstanced(GL_TRIANGLES, model->numIndices, GL_UNSIGNED_INT, 0L, instanceCount);

}

void Geometry::setUpInstanceBuffers(std::vector<GLfloat>& startPositions) {
		
	instanceCount = startPositions.size() / 3;

	glBindVertexArray(vao);
	GLuint startPositionsBuffer;
	glGenBuffers(1, &startPositionsBuffer);
	glBindBuffer(GL_ARRAY_BUFFER, startPositionsBuffer);
	glBufferData(GL_ARRAY_BUFFER,  startPositions.size() * sizeof(GLfloat), &startPositions[0], GL_STATIC_DRAW);

	glEnableVertexAttribArray(2);
	glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, 0, 0);
	glVertexAttribDivisor(2, 1);
}


// Destructor, todo cleanup
Geometry::~Geometry() {
	std::cout << "Bye!" << std::endl;
}

void Geometry::testFunction() {
	std::cout << myInt << std::endl;
}

