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


#include "Demos.h"
#include "Camera.h"

#define _CRT_SECURE_NO_WARNINGS

#define frameFreq 20

// World objects

std::vector<Geometry*> demos;
AnimationShader* as;
float startTime = 0;

void init(void)
{
	dumpInfo();
	// GL inits
	glClearColor(0.0, 0.0, 0.0, 0);
	glDisable(GL_DEPTH_TEST);
	glEnable(GL_CULL_FACE);
	glEnable(GL_PROGRAM_POINT_SIZE);
	glEnable(GL_POINT_SPRITE);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	printError("GL inits");
	// Load the models.
	demos.push_back(DemoOne());
	//demos.push_back(DemoTwo());
	//demos.push_back(DemoThree());
}

void draw() {
	GLfloat t = (GLfloat)glutGet(GLUT_ELAPSED_TIME);
	Camera::handleKeyPress();
	int index = (int)(t / 10000.) % demos.size();
	
	mat4 trans = T(0, 0, 0);
	mat4 rot = Mult(Rx(-M_PI / 2), Rz(t / 2500)); // The teapot object is on the side.
	mat4 tot = Mult(trans, rot);
	demos[index]->draw(t - startTime, Camera::getMatrix().m, &Camera::pos.x, &tot.m);

	//as->draw(g, t - startTime, tot.m, Camera::getMatrix().m, &Camera::pos.x);
	
	g->draw(t - startTime, tot.m, Camera::getMatrix().m, &Camera::pos.x);

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
	glutInitWindowSize(1280, 720);
	glutCreateWindow("GL3 multiple objects example");
#ifdef WIN32
	glewInit();
#endif

	startTime = (GLfloat)glutGet(GLUT_ELAPSED_TIME);
	init();
	Camera::setDirection(0, 0);
	glutPassiveMotionFunc(Camera::onMouseMove);
	glutMouseFunc(Camera::onClick);
	
	glutDisplayFunc(display);
	glutTimerFunc(frameFreq, &OnTimer, 0);
	glutMainLoop();
	return 0;
}