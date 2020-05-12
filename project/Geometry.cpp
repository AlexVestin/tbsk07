#pragma once
#include "Geometry.h"

#define DEFAULT_SPEC_EXP 100.0f

// Useful if loaded using LoadModelFromXXX
Geometry::Geometry(Model* model, GLuint program) : model(model) {
	this->program = program;
	glGenVertexArrays(1, &vao);
	setUpGeometryBuffers();
	specularExp = DEFAULT_SPEC_EXP;
	//createShader();
};

Geometry::Geometry(const char* modelPath, GLuint program) {
	model		  = LoadModelPlus(modelPath);
	this->program = program;
	glGenVertexArrays(1, &vao);
	setUpGeometryBuffers();
	specularExp = DEFAULT_SPEC_EXP;
	//createShader();
	tex = createParticleTexture();
};

void Geometry::setUpGeometryBuffers() {
	int i;
	glBindVertexArray(vao);
	// Set up buffer objects to copy model data to instance
	// Vertex buffer

	GLuint vertexLoc = glGetAttribLocation(program, "in_Position");
	std::cout << "vertexLoc: " << vertexLoc << std::endl;
	glGenBuffers(1, &vbo);
	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glBufferData(GL_ARRAY_BUFFER, model->numVertices * 3 * sizeof(GLfloat), model->vertexArray, GL_STATIC_DRAW);
	glVertexAttribPointer(vertexLoc, 3, GL_FLOAT, GL_FALSE, 0, 0);
	glEnableVertexAttribArray(vertexLoc);
	glVertexAttribDivisor(vertexLoc, 0);

	// Normal buffers
	/*GLuint normalLoc = glGetAttribLocation(program, "in_Normal");
	glGenBuffers(1, &nbo);
	glBindBuffer(GL_ARRAY_BUFFER, nbo);
	glBufferData(GL_ARRAY_BUFFER, model->numVertices * 3 * sizeof(GLfloat), model->normalArray, GL_STATIC_DRAW);
	glVertexAttribPointer(normalLoc, 3, GL_FLOAT, GL_FALSE, 0, 0);
	glEnableVertexAttribArray(normalLoc);
	glVertexAttribDivisor(normalLoc, 0);
	*/

	// indices
	glGenBuffers(1, &ivbo);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ivbo);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, model->numIndices * sizeof(GLuint), model->indexArray, GL_STATIC_DRAW);
}

/*
void Geometry::createShader() {
	program = loadShaders("main.vert", "main.frag");
	glUseProgram(program);

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
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, tex);

	glUniformMatrix4fv(glGetUniformLocation(program, "projMatrix"), 1, GL_TRUE, &Camera::projectionMatrix[0]);
		
	// Upload light data
	GLuint loc = glGetUniformLocation(program, "lightSourcesDirPosArr");

	glUniform3fv(loc, lightNo, &lightSourcesDirectionsPositions[0].x);
	glUniform3fv(glGetUniformLocation(program, "lightSourcesColorArr"), lightNo, &lightSourcesColorsArr[0].x);

	glUniform1iv(glGetUniformLocation(program, "isDirectional"), lightNo, isDirectional);
	glUniform1i(glGetUniformLocation(program, "lightSourcesNo"), lightNo);
}
*/

void Geometry::draw(float t, GLfloat* tranMatrix, GLfloat* camMatrix, GLfloat* camPos, int drawType) {
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

	glDepthMask(false);

	if(drawType == 0)
		glDrawElementsInstanced(GL_POINTS, model->numIndices, GL_UNSIGNED_INT, 0L, instanceCount);
	else if(drawType == 1)
		DrawModel(model, program, "in_Position", "in_Normal", "inTexCoord");
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
	
	//glUseProgram(program);
	//glUniform1i(glGetUniformLocation(program, "texUnit"), 0); // Texture unit 0
	return tex;
}

void Geometry::setUpInstanceBuffers(GeometryAttributeBuffers& attributes) {
	glUseProgram(program);
	glBindVertexArray(vao);

	if (attributes.startPositions.empty()) {
		if (!attributes.instanceCount) {
			throw "Error, setUpInstanceBuffer requires either startPositions or instanceCount to be used";
		}
		instanceCount = attributes.instanceCount;
	}
	else {
		instanceCount = attributes.startPositions.size() / 3;
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

