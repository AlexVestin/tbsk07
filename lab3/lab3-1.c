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

#define near 1.0

#define far 30.0

#define right 0.5

#define left -0.5

#define top 0.5

#define bottom -0.5

#define modelno 4

// Globals
GLfloat projectionMatrix[] = { 2.0f*near / (right - left), 0.0f, (right + left) / (right - left), 0.0f,
							   0.0f, 2.0f*near / (top - bottom), (top + bottom) / (top - bottom), 0.0f,
							   0.0f, 0.0f, -(far + near) / (far - near), -2 * far*near / (far - near),
							   0.0f, 0.0f, -1.0f, 0.0f };

// Reference to a shader program.
GLint program;
// Model objects.

Model *windmill[modelno];

void init(void)
{
	dumpInfo();

	// GL inits
	glClearColor(0.0, 0.0, 0.0, 0);
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_CULL_FACE);
	printError("GL inits");

	// Load and compile shader
	program = loadShaders("lab3-1.vert", "lab3-1.frag");
	glUseProgram(program);
	printError("init shader");

	// Load the model.
	windmill[0]	= LoadModelPlus("windmill/windmill-walls.obj");
	windmill[1] = LoadModelPlus("windmill/windmill-balcony.obj");
	windmill[2] = LoadModelPlus("windmill/windmill-roof.obj");
	windmill[3]	= LoadModelPlus("windmill/blade.obj");
	
}


void display(void)
{
	GLfloat t = (GLfloat)glutGet(GLUT_ELAPSED_TIME);
	mat4 trans, rot;

	// We rotate the camera around the origin.
	vec3 p = /*MultVec3(Ry(t/2500), SetVector(0, 5, -20))*/ SetVector(25, 5, 0);
	mat4 camera = lookAt(
		p.x, p.y, p.z,
		0, 5, 0,
		0, 1, 0
	);

	// We pass a camera position matrix.
	glUniformMatrix4fv(glGetUniformLocation(program, "camMatrix"), 1, GL_TRUE, camera.m);
	// We also pass the world projection matrix
	glUniformMatrix4fv(glGetUniformLocation(program, "projMatrix"), 1, GL_TRUE, projectionMatrix);
	printError("pre display");

	// clear the screen
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	
	// We draw the windmill.
	trans = T(0, 0, 0);
	glUniformMatrix4fv(glGetUniformLocation(program, "tranMatrix"), 1, GL_TRUE, trans.m);
	for (int i = 0; i < modelno-1; i++) {
		DrawModel(windmill[i], program, "in_Position", "in_Normal", "inTexCoord");
	}
	//After that, we place the blades.
	trans = T(4.5, 9.25, 0);	
	for (int j = 0; j < 4; j++) {
		rot = Rx((t / 1000) + (j*3.1415/2));
		glUniformMatrix4fv(glGetUniformLocation(program, "tranMatrix"), 1, GL_TRUE, Mult(trans, rot).m);
		DrawModel(windmill[modelno - 1], program, "in_Position", "in_Normal", "inTexCoord");
	}
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
	glutCreateWindow("GL3 multiple objects example");
#ifdef WIN32
	glewInit();
#endif
	glutDisplayFunc(display);
	glutTimerFunc(20, &OnTimer, 0);
	init();
	glutMainLoop();
	return 0;
}
