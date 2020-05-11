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
#include "Camera.h"

#define _CRT_SECURE_NO_WARNINGS

#define frameFreq 20

// World objects
Geometry* g;


void init(void)
{
	dumpInfo();

	// GL inits
	glClearColor(0.0, 0.0, 0.0, 0);
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_CULL_FACE);
	printError("GL inits");

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
	Camera::handleKeyPress();
	g->draw(t, Camera::getMatrix().m, &Camera::pos.x);
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
	Camera::setDirection(0, 0);
	glutPassiveMotionFunc(Camera::onMouseMove);
	glutMouseFunc(Camera::onClick);
	
	glutDisplayFunc(display);
	glutTimerFunc(frameFreq, &OnTimer, 0);
	glutMainLoop();
	return 0;
}