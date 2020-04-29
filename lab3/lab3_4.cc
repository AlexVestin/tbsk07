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
#define far 50.0
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
	-1.f, -1.f,
	-1.f, 1.f,
	1.f,  1.f,
	-1.f, -1.f,
	1.f,  1.f,
	1.f,  -1.f,
};

	
GLfloat texCoords[] = {
	.0f, .0f,   
	.0f, 1.0f,   
	1.0f, 1.0f,   
	.0f, .0f,
	1.0f, 1.0f,
	1.0f, 0.0f
};

vec3 cameraPosition;
vec3 cameraDirection;
vec3 cameraUp;

// vertex array object
GLuint program, skyboxProgram;
GLuint myTex;

GLuint groundProgram, groundTex;
GLuint groundVertexArrayObjID, groundBufferObjID;

vec3 lightSourcesColorsArr[] = { {1.0f, 0.0f, 0.0f}, // Red light
                                 {0.0f, 1.0f, 0.0f}, // Green light
                                 {0.0f, 0.0f, 1.0f}, // Blue light
                                 {1.0f, 1.0f, 1.0f} }; // White light

vec3 lightSourcesDirectionsPositions[] = { {10.0f, 5.0f, 0.0f}, // Red light, positional
                                       {0.0f, 5.0f, 10.0f}, // Green light, positional
                                       {-1.0f, 0.0f, 0.0f}, // Blue light along X
                                       {0.0f, 0.0f, -1.0f} }; // White light along Z

GLfloat specularExponent[] = {100.0, 200.0, 60.0, 50.0, 300.0, 150.0};
GLint isDirectional[] = {0,0,1,1};
void init(void)
{
	// vertex buffer object, used for uploading the geometry
	unsigned int vertexBufferObjID;
	// Reference to shader program
	printError("init shader");
	cameraPosition = vec3(0, 2, 20);
	cameraDirection = vec3(0, 1, 0);
	cameraUp = vec3(0, 1, 0);
	mat4 lkAt = lookAtv(cameraPosition, cameraDirection, cameraUp);

	GLuint texCoordBufferId;
	groundProgram = loadShaders("ground.vert", "ground.frag");
	glUseProgram(groundProgram);
	glGenVertexArrays(1, &groundVertexArrayObjID);
	glGenBuffers(1, &groundBufferObjID);
    glGenBuffers(1, &texCoordBufferId);

    glBindVertexArray(groundVertexArrayObjID);
    
	// Vertex positions
	glBindBuffer(GL_ARRAY_BUFFER, groundBufferObjID);
    glBufferData(GL_ARRAY_BUFFER, 12*sizeof(GLfloat), groundVertices, GL_STATIC_DRAW);
    glVertexAttribPointer(glGetAttribLocation(groundProgram, "inPosition"), 2, GL_FLOAT, GL_FALSE, 0, 0); 
    glEnableVertexAttribArray(glGetAttribLocation(groundProgram, "inPosition"));	

	// Tex coords
	glBindBuffer(GL_ARRAY_BUFFER, texCoordBufferId);
    glBufferData(GL_ARRAY_BUFFER, 12*sizeof(GLfloat), texCoords, GL_STATIC_DRAW);
	glVertexAttribPointer(glGetAttribLocation(groundProgram, "inTexCoords"), 2, GL_FLOAT, GL_FALSE, 0, 0); 
    glEnableVertexAttribArray(glGetAttribLocation(groundProgram, "inTexCoords"));
	
	glUniformMatrix4fv(glGetUniformLocation(groundProgram, "view"), 1, GL_TRUE, lkAt.m);
	glUniformMatrix4fv(glGetUniformLocation(groundProgram, "proj"), 1, GL_TRUE, projectionMatrix);

	LoadTGATextureSimple("dirt.tga", &groundTex);
	glBindTexture(GL_TEXTURE_2D, groundTex);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_MIRRORED_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_MIRRORED_REPEAT);

	glUniform1i(glGetUniformLocation(groundProgram, "tex11"), 0);
	glActiveTexture(GL_TEXTURE0);

	dumpInfo();
	// GL inits
	glClearColor(0.2,0.2,0.5,0);
	printError("GL inits");
	glEnable(GL_DEPTH_TEST);
	
	glutInitDisplayMode(GLUT_RGB | GLUT_DOUBLE | GLUT_DEPTH);
	program = loadShaders("lab3_4.vert", "lab3_4.frag");
	glUseProgram(program);
	glUniformMatrix4fv(glGetUniformLocation(program, "view"), 1, GL_TRUE, lkAt.m);
	glUniformMatrix4fv(glGetUniformLocation(program, "proj"), 1, GL_TRUE, projectionMatrix);
    blades = LoadModelPlus("windmill/blade.obj");
    roof = LoadModelPlus("windmill/windmill-roof.obj");
    balcony = LoadModelPlus("windmill/windmill-balcony.obj");
	mill = LoadModelPlus("windmill/windmill-walls.obj");
	
	
	// skybox 
	skyboxProgram = loadShaders("skybox.vert", "skybox.frag");
	skybox = LoadModelPlus("skybox.obj");
	glUseProgram(skyboxProgram);
	LoadTGATextureSimple("SkyBox512.tga", &myTex);
	glBindTexture(GL_TEXTURE_2D, myTex);
	glUniform1i(glGetUniformLocation(skyboxProgram, "texUnit"), 0);
	glActiveTexture(GL_TEXTURE0);
	glUniformMatrix4fv(glGetUniformLocation(skyboxProgram, "view"), 1, GL_TRUE, lkAt.m);
	glUniformMatrix4fv(glGetUniformLocation(skyboxProgram, "proj"), 1, GL_TRUE, projectionMatrix);
	
	printError("init arrays");
}


void OnTimer(int value) {
    glutPostRedisplay();
    glutTimerFunc(20, &OnTimer, value);
}

void applyLights(GLuint program) {
	glUniform3fv(glGetUniformLocation(program, "viewPos"), 1, &cameraPosition.x);
	glUniform3fv(glGetUniformLocation(program, "lightSourcesDirPosArr"), 4, &lightSourcesDirectionsPositions[0].x);
	glUniform3fv(glGetUniformLocation(program, "lightSourcesColorArr"), 4, &lightSourcesColorsArr[0].x);
	for(int i = 0; i < 5; i++) {
		glUniform1f(glGetUniformLocation(program, "specularExponent"), specularExponent[i]);
	}
	glUniform1iv(glGetUniformLocation(program, "isDirectional"), 4, isDirectional);
}


void display(void)
{
	printError("pre display");
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	GLfloat t = (GLfloat)glutGet(GLUT_ELAPSED_TIME);
	float dt = t / 300.;
	

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

	if (glutKeyIsDown('k')) {
		cameraPosition.y += movementAmount;
		cameraDirection.y += movementAmount;
	}

	if (glutKeyIsDown('l')) {
		cameraPosition.y -= movementAmount;
		cameraDirection.y -= movementAmount;
	}
	// Skybox
	mat4 lkAt = lookAtv(cameraPosition, cameraDirection, cameraUp);
	lkAt.m[3] = 0;
	lkAt.m[7] = 0;
	lkAt.m[11] = 0;
	glDisable(GL_DEPTH_TEST);
	glUseProgram(skyboxProgram);
	glBindTexture(GL_TEXTURE_2D, myTex);
	glUniformMatrix4fv(glGetUniformLocation(skyboxProgram, "view"), 1, GL_TRUE, lkAt.m);
	DrawModel(skybox, skyboxProgram, "inPosition", "inNormal", "inTexCoord");
	

	// Draw textured ground
	float scale = 3000.;
	mat4 total = Mult(T(0, 0.0, 0), Mult(Ry(3.14 / 2.), S(scale, 0, scale)));
	glUseProgram(groundProgram);
	glBindTexture(GL_TEXTURE_2D, groundTex);
	glActiveTexture(GL_TEXTURE0);
	lkAt = lookAtv(cameraPosition, cameraDirection, cameraUp);
	glUniformMatrix4fv(glGetUniformLocation(groundProgram, "view"), 1, GL_TRUE, lkAt.m);
	glUniformMatrix4fv(glGetUniformLocation(groundProgram, "model"), 1, GL_TRUE, total.m);
	glBindVertexArray(groundVertexArrayObjID);
	glDrawArrays(GL_TRIANGLES, 0, 6);
	glEnable(GL_DEPTH_TEST);

	lkAt = lookAtv(cameraPosition, cameraDirection, cameraUp);
	mat4 trans, rot;
	total = T(0, 0, 0);
	glUseProgram(program);
	applyLights(program);

	glUniformMatrix4fv(glGetUniformLocation(program, "view"), 1, GL_TRUE, lkAt.m);
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
