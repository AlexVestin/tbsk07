#pragma once
#include "Geometry.h"

#define DEFAULT_SPEC_EXP 100.0f



Geometry::Geometry(GeometryAttributeBuffers attributes, GLuint program, GLuint drawType) : drawType(drawType), program(program) {
	glGenVertexArrays(1, &vao);
	glBindVertexArray(vao);
	drawFunc = DrawFunc::ELEMENTS;
	specularExp = DEFAULT_SPEC_EXP;
	tex = createParticleTexture();
	setUpInstanceBuffers(attributes);
		
	float dur = attributes.duration ? attributes.duration : 30.f;
	std::cout << "duration: " << dur << "instanceCount: " << instanceCount << std::endl;
	glUniform1f(glGetUniformLocation(program, "duration"), dur);
	glUniform1i(glGetUniformLocation(program, "instanceCount"), instanceCount);
	glUniform1i(glGetUniformLocation(program, "repeat"), attributes.repeat);

}

Geometry::Geometry(GeometryAttributeBuffers attributes, Model* model, GLuint program, GLuint drawType) : Geometry(attributes, program, drawType)  {
	this->model = model;
	setUpGeometryBuffers();
};

Geometry::Geometry(GeometryAttributeBuffers attributes, GLuint program, GLuint drawType, DrawFunc drawFunc) : Geometry(attributes, program, drawType) {
	this->drawFunc = drawFunc;
};

Geometry::Geometry(GeometryAttributeBuffers attributes, const char* modelPath, GLuint program, GLuint drawType): Geometry(attributes, program, drawType) {
	model		  = LoadModelPlus(modelPath);
	setUpGeometryBuffers();
};

void Geometry::setUpGeometryBuffers() {
	int i;
	glBindVertexArray(vao);
	// Set up buffer objects to copy model data to instance
	// Vertex buffer
	GLuint vertexLoc = glGetAttribLocation(program, "in_Position");
	glGenBuffers(1, &vbo);
	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glBufferData(GL_ARRAY_BUFFER, model->numVertices * 3 * sizeof(GLfloat), model->vertexArray, GL_STATIC_DRAW);
	glVertexAttribPointer(vertexLoc, 3, GL_FLOAT, GL_FALSE, 0, 0);
	glEnableVertexAttribArray(vertexLoc);
	glVertexAttribDivisor(vertexLoc, 0);

	// Normal buffers
	GLuint normalLoc = glGetAttribLocation(program, "in_Normal");
	glGenBuffers(1, &nbo);
	glBindBuffer(GL_ARRAY_BUFFER, nbo);
	glBufferData(GL_ARRAY_BUFFER, model->numVertices * 3 * sizeof(GLfloat), model->normalArray, GL_STATIC_DRAW);
	glVertexAttribPointer(normalLoc, 3, GL_FLOAT, GL_FALSE, 0, 0);
	glEnableVertexAttribArray(normalLoc);
	glVertexAttribDivisor(normalLoc, 0);

	// indices
	glGenBuffers(1, &ivbo);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ivbo);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, model->numIndices * sizeof(GLuint), model->indexArray, GL_STATIC_DRAW);
}

void Geometry::draw(float t, GLfloat* tranMatrix, GLfloat* camMatrix, GLfloat* camPos) {
	glUseProgram(program);
	glBindVertexArray(vao);
	
	// Upload and enable the texture.
	glUniform1i(glGetUniformLocation(program, "texUnit"), 0); // Texture unit 0
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, tex);

	// Update uniforms
	// We pass a camera (view) matrix.
	glUniformMatrix4fv(glGetUniformLocation(program, "camMatrix"), 1, GL_TRUE, camMatrix);

	// We pass its position (Is needed in the fragment shader).
	glUniform3fv(glGetUniformLocation(program, "camPos"), 1, camPos);

	//mat4 trans = T(0, 0, 0);
	//mat4 rot = Mult(Rx(-M_PI / 2), Rz(t / 2500)); // The teapot object is on the side.
	glUniformMatrix4fv(glGetUniformLocation(program, "tranMatrix"), 1, GL_TRUE, tranMatrix);
	
	glUniform1f(glGetUniformLocation(program, "specularExponent"), specularExp);
	glUniform1f(glGetUniformLocation(program, "time"), t / 1000.);

	//glDepthMask(false);
	
	bool drawAsPoints = drawType == GL_POINTS;
	glUniform1i(glGetUniformLocation(program, "drawnAsPoints"), drawType == GL_POINTS);
	
	if (drawFunc == DrawFunc::ELEMENTS) {
		glDrawElementsInstanced(drawType, model->numIndices, GL_UNSIGNED_INT, 0L, instanceCount);
	} else {
		glDrawArraysInstanced(drawType, 0, 1, instanceCount);
	}
}


GLuint Geometry::createParticleTexture() {
	GLuint tex;
	std::string path = __FILE__; //gets source code path, include file name
	path = path.substr(0, 1 + path.find_last_of('\\')) + "\\" + "particle2.tga";
	
	// LoadTGASimple only takes char* and not const char*, so we need to make it writeable
	char* writable = new char[path.size() + 1];
	std::copy(path.begin(), path.end(), writable);
	writable[path.size()] = '\0'; // don't forget the terminat
	LoadTGATextureSimple(writable, &tex);
	
	return tex;
}

void Geometry::setUpInstanceBuffers(GeometryAttributeBuffers& attributes) {
	glUseProgram(program);
	glBindVertexArray(vao);

	std::cout << glGetError() << std::endl; 

	if (!attributes.instanceCount) {
		throw "Error: instanceCount required";
	}


	instanceCount = attributes.instanceCount;
	if (!attributes.startPositions.empty()) {
		std::cout << "Instance set up" << glGetAttribLocation(program, "startPos") << std::endl;

		createBuffer(attributes.startPositions, glGetAttribLocation(program, "startPos"), 3);
	}

	if (!attributes.endPositions.empty()) {
		createBuffer(attributes.endPositions, glGetAttribLocation(program, "endPos"), 3);
	}

	if (!attributes.startTimes.empty()) {
		createBuffer(attributes.startTimes, glGetAttribLocation(program, "startTime"), 3);
	}

	if (!attributes.sizes.empty()) {
		createBuffer(attributes.sizes, glGetAttribLocation(program, "size"), 3);
	}

	if (!attributes.colors.empty()) {
		createBuffer(attributes.colors, glGetAttribLocation(program, "inCol"), 3);
	}
}

void Geometry::setUpInstanceBuffers(std::vector<GLfloat>& startPositions, std::vector<GLfloat>& endPositions) {
	glUseProgram(program);
	glBindVertexArray(vao);
	instanceCount = startPositions.size() / 3;
	createBuffer(startPositions, glGetAttribLocation(program, "startPosition"), 3);
	createBuffer(endPositions, glGetAttribLocation(program, "endPosition"), 3);
}


void Geometry::setUpInstanceBuffers(std::vector<GLfloat>& startPositions) {
	instanceCount = startPositions.size() / 3;
	createBuffer(startPositions, 2, 3);
}

// Destructor, todo cleanup
Geometry::~Geometry() {
	std::cout << "Bye!" << std::endl;
}

