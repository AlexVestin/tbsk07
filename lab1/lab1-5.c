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
#include <math.h>

#define ROTATEX 0
#define ROTATEY 0
#define ROTATEZ 1


const int nr_triangles  = 6*3;
GLfloat vertices[] = {

	//Right
	0.f, 0.5f, 0.f,
	0.5f, -.5f, 0.5f,
	0.5f, -0.5f, -0.5f,
	//Front
	0.f, 0.5f, 0.f,
	-0.5f, -0.5f, 0.5f,
	0.5f, -.5f, 0.5f,
	
	// Back
	0.f, 0.5f, 0.f,
	0.5f, -0.5f, -0.5f,
	-0.5f, -0.5f, -0.5f,

	//Left
	0.f,  0.5, 0.f,
	-0.5f, -0.5f, -0.5f,
	-0.5f, -0.5f, 0.5f,

	// Bottom
	-0.5, -0.5f, -0.5f,
	0.5, -0.5f, -0.5f,
	0.5, -0.5f, 0.5f,

	// other Bottom
	0.5, -0.5f, 0.5f,
	-0.5, -0.5f, -0.5f,
	-0.5, -0.5f, 0.5f,

};


GLfloat colors[] =
{
	0.1f, 0.8f, 1.0f,
	0.1f,0.25f,1.0f,
	1.0f,1.0f,0.5f,

	0.5f,0.1f,0.0f,
	0.5f, 0.95f,0.4f,
	0.5f,0.5f,0.1f,

	1.0f,0.f,0.0f,
	0.f, 1.0f, 0.f,
	0.f,0.f,1.f,

	1.0f, 1.f,0.0f,
	0.f, 1.0f, 1.f,
	1.f,0.f,1.f,

	1.0f,0.5f,0.5f,
	0.5f, 1.0f, 0.f,
	0.5f,0.5f, .5f,

	0.4f,0.3f,0.3f,
	0.7f, 0.7f, 0.7f,
	0.95f,0.95f,0.95f
};


// vertex array object
unsigned int vertexArrayObjID;
GLuint program;

void init(void)
{
	// vertex buffer object, used for uploading the geometry
	unsigned int vertexBufferObjID;
	// Reference to shader program

	dumpInfo();
	// GL inits
	glClearColor(0.2,0.2,0.5,0);
	printError("GL inits");
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_CULL_FACE);
	glutInitDisplayMode(GLUT_RGB | GLUT_DOUBLE | GLUT_DEPTH);



	// Load and compile shader
	program = loadShaders("lab1-5.vert", "lab1-5.frag");


	
	printError("init shader");
	
	// Upload geometry to the GPU:
	
	// Allocate and activate Vertex Array Object
	glGenVertexArrays(1, &vertexArrayObjID);
	glBindVertexArray(vertexArrayObjID);
	// Allocate Vertex Buffer Objects
	glGenBuffers(1, &vertexBufferObjID);
	
	// VBO for vertex data
	glBindBuffer(GL_ARRAY_BUFFER, vertexBufferObjID);
	
	glBufferData(GL_ARRAY_BUFFER, nr_triangles*3*sizeof(GLfloat), vertices, GL_STATIC_DRAW);
	glVertexAttribPointer(glGetAttribLocation(program, "in_Position"), 3 , GL_FLOAT, GL_FALSE, 0, 0); 
	glEnableVertexAttribArray(glGetAttribLocation(program, "in_Position"));


	
	GLuint colorBufferObjID;
	glGenBuffers(1, &colorBufferObjID);

	// VBO for vertex data
	glBindBuffer(GL_ARRAY_BUFFER, colorBufferObjID);
	glBufferData(GL_ARRAY_BUFFER, nr_triangles*3*sizeof(GLfloat), colors, GL_STATIC_DRAW);
	glVertexAttribPointer(glGetAttribLocation(program, "in_Color"), 3, GL_FLOAT, GL_FALSE, 0, 0); 
	glEnableVertexAttribArray(glGetAttribLocation(program, "in_Color"));
	
	// End of upload of geometry	
	printError("init arrays");
}


void OnTimer(int value) {
    glutPostRedisplay();
    glutTimerFunc(20, &OnTimer, value);
}


void display(void)
{
	printError("pre display");

	// clear the screen
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	

	
	GLfloat t = (GLfloat)glutGet(GLUT_ELAPSED_TIME);
	float dt = t / 300.;
	
	#if ROTATEX
	GLfloat myMatrix[] = { 
		cos(dt), 0.0f, sin(dt), 0.5f,
		0.0f, 1.0f, 0.0f, 0.0f,
		-sin(dt), 0.0f, cos(dt), 0.0f,
		0.0f, 0.0f, 0.0f, 1.0f 
	};
	#endif
	
	#if ROTATEY
	GLfloat myMatrix[] = { 
		cos(dt), -sin(dt), 0.0, 0.5f,
		sin(dt), cos(dt), 0.0f, 0.0f,
		0.0f, 0.0f, 1.0f, 0.0f,
		0.0f, 0.0f, 0.0f, 1.0f 
	};
	#endif

	#if ROTATEZ
	GLfloat myMatrix[] = { 
		1.0f, 0.0f, 0.0f, 0.0f,
		cos(dt), -sin(dt), 0.0, 0.0f,
		sin(dt), cos(dt), 0.0f, 0.0f,
		0.0f, 0.0f, 0.0f, 1.0f,
	};
	#endif
	


	glUniformMatrix4fv(glGetUniformLocation(program, "myMatrix"), 1, GL_TRUE, myMatrix);
	
	glBindVertexArray(vertexArrayObjID);	// Select VAO
	glDrawArrays(GL_TRIANGLES, 0, nr_triangles);	// draw object
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
