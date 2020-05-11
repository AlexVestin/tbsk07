// Lab 1-1.
// This is the same as the first simple example in the course book,
// but with a few error checks.
// Remember to copy your file to a new on appropriate places during the lab so you keep old results.
// Note that the files "lab1-1.frag", "lab1-1.vert" are required.

// Should work as is on Linux and Mac. MS Windows needs GLEW or glee.
// See separate Visual Studio version of my demos.
#include <Windows.h>
#ifdef __APPLE__
#include <OpenGL/gl3.h>
	// Linking hint for Lightweight IDE
	// uses framework Cocoa
#endif


#include "MicroGlut.h"
#if defined(_WIN32)
#include "glew.h"
#endif
#include <GL/gl.h>


#include "GL_utilities.h"
#include "VectorUtils3.h"
#include <math.h>
#include <loadobj.h>
#include <LoadTGA.h>
#include "Geometry.h"

#define _CRT_SECURE_NO_WARNINGS


#define modelno 4
#define frameFreq 20
#define pi 3.1415
#define LENGTH(x) (sizeof(x) / sizeof((x)[0]))


// Camera coordinates.
vec3 camPos, camDir;

// World objects
Geometry* g;



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

void init(void)
{
	dumpInfo();

	// GL inits
	glClearColor(0.0, 0.0, 0.0, 0);
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_CULL_FACE);
	printError("GL inits");

	// Init the camera coordinates
	camPos = SetVector(10, 2, 0);
	camDir = SetVector(-1, 0, 0);

	// Load the models.
	g = new Geometry{ LoadModelPlus("teapot.obj") };
		
	const int numInstances = 200;
	std::vector<GLfloat> startPositions(numInstances * 3);
	std::vector<GLfloat> endPositions(numInstances * 3);
	std::vector<GLfloat> startTimes(numInstances);


	for (int i = 0; i < numInstances; i++) {
		startPositions[(i * 3)] = (float)i ;
		startPositions[(i * 3)+1] = (float)i * 0.0;
		startPositions[(i * 3)+2] = (float)i ;

		endPositions[(i * 3)] = (float)0;
		endPositions[(i * 3) + 1] = (float)0;
		endPositions[(i * 3) + 2] = (float)0;

		startTimes[i] = i / 100;
	}
	g->setUpInstanceBuffers(startPositions, endPositions, startTimes);
}

void draw() {
	GLfloat t = (GLfloat)glutGet(GLUT_ELAPSED_TIME);
	g->draw(t, updateCameraCoord().m, &camPos.x);
}

void display(void)
{	
	printError("pre display");
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
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
		
	// Allocate console, unsure if neeeded / there is a better solution
	AllocConsole();
	freopen("CONOUT$", "w", stdout);
	freopen("CONOUT$", "w", stderr);

	glutInit(&argc, argv);
	glutInitContextVersion(3, 2);
	glutInitDisplayMode(GLUT_RGB | GLUT_DOUBLE | GLUT_DEPTH);
	glutInitWindowSize(768, 768);
	glutCreateWindow("GL3 multiple objects example");
#ifdef WIN32
	glewInit();
#endif

	init();
	glutDisplayFunc(display);
	glutTimerFunc(frameFreq, &OnTimer, 0);
	glutMainLoop();
	return 0;
}