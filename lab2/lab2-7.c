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
#include "loadobj.h"

#include "VectorUtils3.h"
#include "GL_utilities.h"
#include <math.h>

#define ROTATEX 0
#define ROTATEY 1
#define ROTATEZ 0


#define near 1.0
#define far 30.0
#define right 0.5
#define left -0.5
#define top 0.5
#define bottom -0.5

GLfloat projectionMatrix[] = {    
	2.0f*near/(right-left), 0.0f, (right+left)/(right-left), 0.0f,
	0.0f, 2.0f*near/(top-bottom), (top+bottom)/(top-bottom), 0.0f,
	0.0f, 0.0f, -(far + near)/(far - near), -2*far*near/(far - near),
	0.0f, 0.0f, -1.0f, 0.0f 
};

Model *m;
Model *m2;

// vertex array object
GLuint program;

void init(void)
{
	// vertex buffer object, used for uploading the geometry
	unsigned int vertexBufferObjID;
	// Reference to shader program
	printError("init shader");


	dumpInfo();
	// GL inits
	glClearColor(0.2,0.2,0.5,0);
	printError("GL inits");
	glEnable(GL_DEPTH_TEST);
	glDisable(GL_CULL_FACE);
	
	glutInitDisplayMode(GLUT_RGB | GLUT_DOUBLE | GLUT_DEPTH);
	program = loadShaders("lab2-7.vert", "lab2-7.frag");
	m = LoadModelPlus("bunnyplus.obj");
	m2 = LoadModelPlus("teddy.obj");


	GLuint myTex;
	LoadTGATextureSimple("maskros512.tga", &myTex);
	glBindTexture(GL_TEXTURE_2D, myTex);
	glUniform1i(glGetUniformLocation(program, "texUnit"), 0); // Texture unit 0
	glActiveTexture(GL_TEXTURE0);	

	mat4 lkAt = lookAt(
		0, 0, 3,
		0, 0, 2,
		0, 1, 0
	);

	glUniformMatrix4fv(glGetUniformLocation(program, "view"), 1, GL_TRUE, lkAt.m);
	glUniformMatrix4fv(glGetUniformLocation(program, "proj"), 1, GL_TRUE, projectionMatrix);
	printError("init arrays");
}


void OnTimer(int value) {
    glutPostRedisplay();
    glutTimerFunc(20, &OnTimer, value);
}


void display(void)
{
	printError("pre display");
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	mat4 trans = T(0, 0, -2);
	mat4 rot = Ry((GLfloat)glutGet(GLUT_ELAPSED_TIME) / 3000.);
	mat4 total = Mult(trans, rot);
	glUniformMatrix4fv(glGetUniformLocation(program, "model"),1, GL_TRUE, total.m);
	
	DrawModel(m, program, "inPosition", "inNormal", "inTexCoord");

	trans = T(-2, 1, -4);
	rot = Ry(- (GLfloat)glutGet(GLUT_ELAPSED_TIME) / 1000.);
	total = Mult(trans, rot);
	glUniformMatrix4fv(glGetUniformLocation(program, "model"),1, GL_TRUE, total.m);
	DrawModel(m2, program, "inPosition", "inNormal", "inTexCoord");

	printError("display");
	glutSwapBuffers();
}

int main(int argc, char *argv[])
{
	glutInit(&argc, argv);
	glutInitContextVersion(3, 2);
	glutCreateWindow ("GL3 white triangle example");
	
	glutDisplayFunc(display); 
	init ();	  
	glutTimerFunc(20, &OnTimer, 0);
	glutMainLoop();

	return 0;
}
