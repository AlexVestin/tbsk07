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
#include "LoadTGA.h"
#include "VectorUtils3.h"
#include "GL_utilities.h"
#include <math.h>

#define near 1.0
#define far 200.0
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

Model *blades;
Model *mill;
Model *balcony;
Model *roof;
Model *skybox;


GLuint vertexArrayObjID, vertexBufferObjectId;

const GLfloat groundVertices[] = {
	// Bottom
	-0.5, -0.5f, -0.5f,
	0.5, -0.5f, -0.5f,
	0.5, -0.5f, 0.5f,

	// other Bottom
	-0.5, -0.5f, 0.5f,
	-0.5, -0.5f, -0.5f,
	0.5, -0.5f, 0.5f,
};

vec3 cameraPosition;
vec3 cameraDirection;
vec3 cameraUp;

// vertex array object
GLuint program, skyboxProgram;


void init(void)
{
	// vertex buffer object, used for uploading the geometry
	unsigned int vertexBufferObjID;
	// Reference to shader program
	printError("init shader");
	mat4 lkAt = lookAtv(cameraPosition, cameraDirection, cameraUp);





	dumpInfo();
	// GL inits
	glClearColor(0.2,0.2,0.5,0);
	printError("GL inits");
	glEnable(GL_DEPTH_TEST);
	
	glutInitDisplayMode(GLUT_RGB | GLUT_DOUBLE | GLUT_DEPTH);
	program = loadShaders("lab3_3.vert", "lab3_3.frag");
	glUniformMatrix4fv(glGetUniformLocation(program, "view"), 1, GL_TRUE, lkAt.m);
	glUniformMatrix4fv(glGetUniformLocation(program, "proj"), 1, GL_TRUE, projectionMatrix);
    blades = LoadModelPlus("windmill/blade.obj");
    roof = LoadModelPlus("windmill/windmill-roof.obj");
    balcony = LoadModelPlus("windmill/windmill-balcony.obj");
	mill = LoadModelPlus("windmill/windmill-walls.obj");

	// skybox 
	skyboxProgram = loadShaders("skybox.vert", "skybox.frag");
	skybox = LoadModelPlus("skybox.obj");

	GLuint myTex;
	LoadTGATextureSimple("SkyBox512.tga", &myTex);
	glBindTexture(GL_TEXTURE_2D, myTex);
	glUniform1i(glGetUniformLocation(skyboxProgram, "texUnit"), 0);
	glActiveTexture(GL_TEXTURE0);
	glUniformMatrix4fv(glGetUniformLocation(skyboxProgram, "view"), 1, GL_TRUE, lkAt.m);
	glUniformMatrix4fv(glGetUniformLocation(skyboxProgram, "proj"), 1, GL_TRUE, projectionMatrix);
	
	cameraPosition = vec3(0, 2, 20);
	cameraDirection = vec3(0, 1, 0);
	cameraUp = vec3(0, 1, 0);

	glUniformMatrix4fv(glGetUniformLocation(skyboxProgram, "view"), 1, GL_TRUE, lkAt.m);
	glUniformMatrix4fv(glGetUniformLocation(skyboxProgram, "proj"), 1, GL_TRUE, projectionMatrix);


	glGenVertexArrays(1, &vertexArrayObjID);
	glBindVertexArray(vertexArrayObjID);
	// Allocate Vertex Buffer Objects
	glGenBuffers(1, &vertexBufferObjID);
	
	// VBO for vertex data
	glBindBuffer(GL_ARRAY_BUFFER, vertexBufferObjID);
	
	glBufferData(GL_ARRAY_BUFFER, 6*sizeof(GLfloat), groundVertices, GL_STATIC_DRAW);
	glVertexAttribPointer(glGetAttribLocation(program, "in_Position"), 3 , GL_FLOAT, GL_FALSE, 0, 0); 
	glEnableVertexAttribArray(glGetAttribLocation(program, "in_Position"));

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

	const double movementAmount = 0.2;
	if (glutKeyIsDown('d')) {
		cameraPosition.x += movementAmount;
		cameraDirection.x += movementAmount;
	}

	if (glutKeyIsDown('a')) {
		cameraPosition.x -= movementAmount;
		cameraDirection.x -= movementAmount;
	}

	if (glutKeyIsDown('s')) {
		cameraPosition.z += movementAmount;
		cameraDirection.z += movementAmount;
	}

	if (glutKeyIsDown('w')) {
		cameraPosition.z -= movementAmount;
		cameraDirection.z -= movementAmount;
	}

	if (glutKeyIsDown('n')) {
		cameraDirection.x -= movementAmount * 10.;
	}

	if (glutKeyIsDown('m')) {
		cameraDirection.x += movementAmount* 10.;
	}



	glDisable(GL_DEPTH_TEST);
	glDisable(GL_CULL_FACE);
	glUseProgram(skyboxProgram);
	
	//mat4 lkAt = lookAtv(vec3(0,0,0), cameraDirection, cameraUp);
	mat4 lkAt = lookAtv(cameraPosition, cameraDirection, cameraUp);
	lkAt.m[3] = 0;
	lkAt.m[7] = 0;
	lkAt.m[11] = 0;

	glDepthMask(GL_FALSE);
	glUniformMatrix4fv(glGetUniformLocation(skyboxProgram, "view"), 1, GL_TRUE, lkAt.m);
	DrawModel(skybox, skyboxProgram, "inPosition", "inNormal", "inTexCoord");
	glDepthMask(GL_TRUE);
	
	glEnable(GL_DEPTH_TEST);

	//glUniformMatrix4fv(glGetUniformLocation(program, "myMatrix"), 1, GL_TRUE, myMatrix);
	glBindVertexArray(vertexArrayObjID);	// Select VAO
	glDrawArrays(GL_TRIANGLES, 0, 6);	// draw object


	
	glUseProgram(program);
	lkAt = lookAtv(cameraPosition, cameraDirection, cameraUp);
	glUniformMatrix4fv(glGetUniformLocation(program, "view"), 1, GL_TRUE, lkAt.m);

	mat4 trans = T(0, 0, -10);
	mat4 rot = Ry(0);
	mat4 total = Mult(trans, rot);

	glUniformMatrix4fv(glGetUniformLocation(program, "model"),1, GL_TRUE, total.m);
    glUniform3f(glGetUniformLocation(program, "color"), 210./255., 180./255., 140./255.);
	DrawModel(mill, program, "inPosition", "inNormal", "inTexCoord");
    glUniform3f(glGetUniformLocation(program, "color"), 222./255.,182./255.,135./255.);
	DrawModel(roof, program, "inPosition", "inNormal", "inTexCoord");
    glUniform3f(glGetUniformLocation(program, "color"), 169./255.,69./255.,19./255.);
	DrawModel(balcony, program, "inPosition", "inNormal", "inTexCoord");

    glUniform3f(glGetUniformLocation(program, "color"), 222./255.,182./255.,135./255.);
	trans = T(0, 0, 1.1);
	rot = Rx(- (GLfloat)glutGet(GLUT_ELAPSED_TIME) / 1000.);
	total = Mult(Mult(trans,  Ry((GLfloat)90)), rot);
	glUniformMatrix4fv(glGetUniformLocation(program, "model"),1, GL_TRUE, total.m);
	DrawModel(blades, program, "inPosition", "inNormal", "inTexCoord");

	rot = Rx(-(GLfloat)glutGet(GLUT_ELAPSED_TIME) / 1000. + 3.14/ 2.);
	total = Mult(Mult(trans,  Ry((GLfloat)90)), rot);
    glUniformMatrix4fv(glGetUniformLocation(program, "model"),1, GL_TRUE, total.m);
	DrawModel(blades, program, "inPosition", "inNormal", "inTexCoord");

	rot = Rx(- (GLfloat)glutGet(GLUT_ELAPSED_TIME) / 1000. + 3.14);
	total = Mult(Mult(trans,  Ry((GLfloat)90)), rot);
    glUniformMatrix4fv(glGetUniformLocation(program, "model"),1, GL_TRUE, total.m);
	DrawModel(blades, program, "inPosition", "inNormal", "inTexCoord");

	rot = Rx(- (GLfloat)glutGet(GLUT_ELAPSED_TIME) / 1000. + 3.14*1.5 );
	total = Mult(Mult(trans,  Ry((GLfloat)90)), rot);
    glUniformMatrix4fv(glGetUniformLocation(program, "model"),1, GL_TRUE, total.m);
    DrawModel(blades, program, "inPosition", "inNormal", "inTexCoord");
	

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
