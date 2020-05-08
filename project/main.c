// Lab 1-1.
// This is the same as the first simple example in the course book,
// but with a few error checks.
// Remember to copy your file to a new on appropriate places during the lab so you keep old results.
// Note that the files "lab1-1.frag", "lab1-1.vert" are required.

// Should work as is on Linux and Mac. MS Windows needs GLEW or glee.
// See separate Visual Studio version of my demos.
#ifdef __APPLE__
#include <OpenGL/gl3.h>
// Linking hint for Lightweight IDE
// uses framework Cocoa
#endif

#include "MicroGlut.h"
#include "GL_utilities.h"
#include "VectorUtils3.h"
#include <math.h>
#include <loadobj.h>
#include <LoadTGA.h>

#define _CRT_SECURE_NO_WARNINGS

#define FLAT_SHADING 0

#define LAMBERT_SHADING 1

#define PHONG_SHADING 2

#define near 1.0

#define far 300.0

#define right 0.5

#define left -0.5

#define top 0.5

#define bottom -0.5

#define modelno 4

#define frameFreq 20

#define pi 3.1415

#define LENGTH(x) (sizeof(x) / sizeof((x)[0]))

// Globals
GLfloat projectionMatrix[] = { 2.0f*near / (right - left), 0.0f, (right + left) / (right - left), 0.0f,
							   0.0f, 2.0f*near / (top - bottom), (top + bottom) / (top - bottom), 0.0f,
							   0.0f, 0.0f, -(far + near) / (far - near), -2 * far*near / (far - near),
							   0.0f, 0.0f, -1.0f, 0.0f };

// Reference to a shader program.
GLint  program;

// Camera coordinates.
vec3 camPos, camDir;

// World objects.
Model *teapot;

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

// vertex array object
unsigned int groundVAO;

// Returns the camera coordinates
mat4 getCameraCoord() {
	vec3 focusPoint = VectorAdd(camDir, camPos);
	mat4 camera = lookAt(
		camPos.x, camPos.y, camPos.z,
		focusPoint.x, focusPoint.y, focusPoint.z,
		0, 1, 0
	);
	return camera;
}

// Updates and returns the camera coordinates.
mat4 updateCameraCoord() {
	vec3 ortDirX, ortDirY;
	GLfloat turnValue = frameFreq / (7500 * pi), moveValue = frameFreq / 1500.0;
	// First we see if the user is moving the camera.
	if (glutKeyIsDown('q')) {
		camDir = MultVec3(Ry(turnValue), camDir);
	}
	if (glutKeyIsDown('e')) {
		camDir = MultVec3(Ry(-turnValue), camDir);
	}

	if (glutKeyIsDown('w')) {
		camPos = VectorAdd(camPos, ScalarMult(camDir, moveValue));
	}
	if (glutKeyIsDown('s')) {
		camPos = VectorAdd(camPos, ScalarMult(camDir, -moveValue));
	}

	ortDirX = MultVec3(Ry(pi / 2), camDir);
	if (glutKeyIsDown('a')) {
		camPos = VectorAdd(camPos, ScalarMult(ortDirX, moveValue));
	}
	if (glutKeyIsDown('d')) {
		camPos = VectorAdd(camPos, ScalarMult(ortDirX, -moveValue));
	}

	ortDirY = SetVector(0, 0.5, 0);
	if (glutKeyIsDown('i')) {
		camPos = VectorAdd(camPos, ScalarMult(ortDirY, moveValue));
	}
	if (glutKeyIsDown('o')) {
		camPos = VectorAdd(camPos, ScalarMult(ortDirY, -moveValue));
	}

	// Then we create and return the camera coordinates.
	return getCameraCoord();
}

// A help method that creates an object from given arrays.
void createObjFromVert(unsigned int *VAO, GLfloat vertices[], GLfloat normals[], GLfloat texCoord[], size_t vertNo, size_t normalNo) {
	// vertex/normal buffer objects, used for uploading the geometry
	unsigned int vertexBufferObjID, normalBufferObjID, texCoordBufferObjID;
	// Allocate and activate Vertex Array Object
	glGenVertexArrays(1, VAO);
	glBindVertexArray(*VAO);
	// Allocate Vertex Buffer Objects
	glGenBuffers(1, &vertexBufferObjID);
	glGenBuffers(1, &normalBufferObjID);
	glGenBuffers(1, &texCoordBufferObjID);

	// VBO for texture data
	if (texCoord != NULL)
	{
		glEnable(GL_TEXTURE_2D);
		glBindBuffer(GL_ARRAY_BUFFER, texCoordBufferObjID);
		glBufferData(GL_ARRAY_BUFFER, vertNo * 2 * sizeof(GLfloat), texCoord, GL_STATIC_DRAW);
		glVertexAttribPointer(glGetAttribLocation(program, "inTexCoord"), 2, GL_FLOAT, GL_FALSE, 0, 0);
		glEnableVertexAttribArray(glGetAttribLocation(program, "inTexCoord"));
	}
	else {
		glDisable(GL_TEXTURE_2D);
	}

	// VBO for vertex data
	glBindBuffer(GL_ARRAY_BUFFER, vertexBufferObjID);
	glBufferData(GL_ARRAY_BUFFER, vertNo * sizeof(GLfloat), vertices, GL_STATIC_DRAW);
	glVertexAttribPointer(glGetAttribLocation(program, "in_Position"), 3, GL_FLOAT, GL_FALSE, 0, 0);
	glEnableVertexAttribArray(glGetAttribLocation(program, "in_Position"));

	// VBO for normal data
	glBindBuffer(GL_ARRAY_BUFFER, normalBufferObjID);
	glBufferData(GL_ARRAY_BUFFER, normalNo * sizeof(GLfloat), normals, GL_STATIC_DRAW);
	glVertexAttribPointer(glGetAttribLocation(program, "in_Normal"), 3, GL_FLOAT, GL_FALSE, 0, 0);
	glEnableVertexAttribArray(glGetAttribLocation(program, "in_Normal"));
}

// A help method that draws a VAO.
void drawVAO(unsigned int *VAO, int indiceNo, GLfloat freq) {
	glBindVertexArray(*VAO);				 // Select VAO
	glVertexAttrib1f(glGetAttribLocation(program, "texFrequency"), freq); // Change texture frequency
	glDrawArrays(GL_TRIANGLES, 0, indiceNo); // draw object
	glVertexAttrib1f(glGetAttribLocation(program, "texFrequency"), 1.0); // Reset texture frequency.
}

void init(void)
{
	dumpInfo();

	// GL inits
	glClearColor(0.0, 0.0, 0.0, 0);
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_CULL_FACE);
	printError("GL inits");

	// Init the camera coordinates
	camPos = SetVector(25, 5, 0);
	camDir = SetVector(-1, 0, 0);

	// Load and compile shaders
	program = loadShaders("main.vert", "main.frag");
	
	glUseProgram(program);
	printError("init shader");

	// Upload light data
	glUniform3fv(glGetUniformLocation(program, "lightSourcesDirPosArr"), lightNo, &lightSourcesDirectionsPositions[0].x);
	glUniform3fv(glGetUniformLocation(program, "lightSourcesColorArr"), lightNo, &lightSourcesColorsArr[0].x);
	glUniform1iv(glGetUniformLocation(program, "isDirectional"), lightNo, isDirectional);
	glUniform1i(glGetUniformLocation(program, "lightSourcesNo"), lightNo);
	
	// Load and activate textures

	// Make the texture loop
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

	// Load the models.
	teapot = LoadModelPlus("teapot.obj");

}

void draw() {
	mat4 trans, rot;

	glUniform1i(glGetUniformLocation(program, "shadingMode"), FLAT_SHADING);

	trans = T(1, 1, 0);
	glUniformMatrix4fv(glGetUniformLocation(program, "tranMatrix"), 1, GL_TRUE, trans.m);
	glUniform1f(glGetUniformLocation(program, "specularExponent"), specularExponent[0]);
	DrawModel(teapot, program, "in_Position", "in_Normal", "inTexCoord");
}

void display(void)
{
	glUseProgram(program);
	// We pass a camera (view) matrix.
	glUniformMatrix4fv(glGetUniformLocation(program, "camMatrix"), 1, GL_TRUE, updateCameraCoord().m);
	// We pass its position (Is needed in the fragment shader).
	glUniform3fv(glGetUniformLocation(program, "camPos"), 1, &camPos);
	// We also pass the world projection matrix
	glUniformMatrix4fv(glGetUniformLocation(program, "projMatrix"), 1, GL_TRUE, projectionMatrix);

	printError("pre display");

	// clear the screen
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	// Draw the world
	draw();

	printError("display");

	glutSwapBuffers();
}

void OnTimer(int value)
{
	glutPostRedisplay();
	glutTimerFunc(20, &OnTimer, value);
}

int main(int argc, char *argv[])
{
	glutInit(&argc, argv);
	glutInitContextVersion(3, 2);
	glutInitDisplayMode(GLUT_RGB | GLUT_DOUBLE | GLUT_DEPTH);
	glutInitWindowSize(768, 768);
	glutCreateWindow("GL3 multiple objects example");
#ifdef WIN32
	glewInit();
#endif
	glutDisplayFunc(display);
	glutTimerFunc(frameFreq, &OnTimer, 0);
	init();
	glutMainLoop();
	return 0;
}