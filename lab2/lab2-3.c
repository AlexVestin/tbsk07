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

unsigned int bunnyVertexArrayObjID;
unsigned int bunnyVertexBufferObjID;
unsigned int bunnyIndexBufferObjID;
unsigned int bunnyNormalBufferObjID;
unsigned int bunnyTexCoordBufferObjID;

Model *m;

// vertex array object
unsigned int vertexArrayObjID;
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

	
		// Load and compile shader
	program = loadShaders("lab2-3.vert", "lab2-3.frag");
	m = LoadModel("bunnyplus.obj");



    glGenVertexArrays(1, &bunnyVertexArrayObjID);
    glGenBuffers(1, &bunnyVertexBufferObjID);
    glGenBuffers(1, &bunnyIndexBufferObjID);
    glGenBuffers(1, &bunnyNormalBufferObjID);
    glBindVertexArray(bunnyVertexArrayObjID);

    // VBO for vertex data
    glBindBuffer(GL_ARRAY_BUFFER, bunnyVertexBufferObjID);
    glBufferData(GL_ARRAY_BUFFER, m->numVertices*3*sizeof(GLfloat), m->vertexArray, GL_STATIC_DRAW);
    glVertexAttribPointer(glGetAttribLocation(program, "in_Position"), 3, GL_FLOAT, GL_FALSE, 0, 0); 
    glEnableVertexAttribArray(glGetAttribLocation(program, "in_Position"));
    
	// VBO for normal data
    glBindBuffer(GL_ARRAY_BUFFER, bunnyNormalBufferObjID);
    glBufferData(GL_ARRAY_BUFFER, m->numVertices*3*sizeof(GLfloat), m->normalArray, GL_STATIC_DRAW);
    glVertexAttribPointer(glGetAttribLocation(program, "in_Normal"), 3, GL_FLOAT, GL_FALSE, 0, 0);
    glEnableVertexAttribArray(glGetAttribLocation(program, "in_Normal"));
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, bunnyIndexBufferObjID);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, m->numIndices*sizeof(GLuint), m->indexArray, GL_STATIC_DRAW);

	glGenBuffers(1, &bunnyTexCoordBufferObjID);    

    if (m->texCoordArray != NULL){
        glBindBuffer(GL_ARRAY_BUFFER, bunnyTexCoordBufferObjID);
        glBufferData(GL_ARRAY_BUFFER, m->numVertices*2*sizeof(GLfloat), m->texCoordArray, GL_STATIC_DRAW);
        glVertexAttribPointer(glGetAttribLocation(program, "inTexCoord"), 2, GL_FLOAT, GL_FALSE, 0, 0);
        glEnableVertexAttribArray(glGetAttribLocation(program, "inTexCoord"));
    }

	GLuint myTex;
	LoadTGATextureSimple("maskros512.tga", &myTex);
	glBindTexture(GL_TEXTURE_2D, myTex);
	glUniform1i(glGetUniformLocation(program, "texUnit"), 0); // Texture unit 0
	glActiveTexture(GL_TEXTURE0);


	glUniformMatrix4fv(glGetUniformLocation(program, "proj"), 1, GL_TRUE, projectionMatrix);

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
	
	#if ROTATEZ
	GLfloat myMatrix[] = { 
		cos(dt), 0.0f, sin(dt), 0.f,
		0.0f, 1.0f, 0.0f, -0.0f,
		-sin(dt), 0.0f, cos(dt), 0.0f,
		0.0f, 0.0f, 0.0f, 1.0f 
	};
	#endif
	
	#if ROTATEX
	GLfloat myMatrix[] = { 
		cos(dt), -sin(dt), 0.0, 0.5f,
		sin(dt), cos(dt), 0.0f, 0.0f,
		0.0f, 0.0f, 1.0f, 0.0f,
		0.0f, 0.0f, 0.0f, 1.0f 
	};
	#endif

	#if ROTATEY
	GLfloat myMatrix[16] = { 
		1.0f, 0.0f, 0.0f, 0.0f,
		0.0f, cos(dt), -sin(dt), -0.5f,
		0.0f, sin(dt), cos(dt), 0.0f,
		0.0f, 0.0f, 0.0f, 1.0f,
	};
	#endif
	

	mat4 trans = T(0, 0, -2);
	mat4 rot = Ry((GLfloat)glutGet(GLUT_ELAPSED_TIME) / 3000.);
	mat4 total = Mult(trans, rot);

	glUniformMatrix4fv(glGetUniformLocation(program, "trans"),1, GL_TRUE, trans.m);
	glUniformMatrix4fv(glGetUniformLocation(program, "rot"),1, GL_TRUE, rot.m);
	glUniformMatrix4fv(glGetUniformLocation(program, "total"),1, GL_TRUE, total.m);
	
	glBindVertexArray(bunnyVertexArrayObjID);    // Select VAO

    glDrawElements(GL_TRIANGLES, m->numIndices, GL_UNSIGNED_INT, 0L);
	
	/*
	glBindVertexArray(vertexArrayObjID);	// Select VAO
	glDrawArrays(GL_TRIANGLES, 0, nr_triangles);	// draw object*/
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
