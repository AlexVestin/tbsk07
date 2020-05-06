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
GLint  program, noTexProgram, noShaProgram;
// Reference to a texture.
GLuint groundTex,
	   whiteTex,
	   skyTex;
// Model objects.
Model *windmill[modelno];
Model *bunny, *cube, *skybox;
// Camera coordinates.
vec3 camPos, camDir;
// Ground vertices + normal.
GLfloat groundVert[] = {
	-100.0f, 0.0f,  100.0f,
	 100.0f, 0.0f,  100.0f,
	 100.0f, 0.0f, -100.0f,
	-100.0f, 0.0f,  100.0f,
	 100.0f, 0.0f, -100.0f,
	-100.0f, 0.0f, -100.0f
},
groundNormal[] = {
	0.0f, 1.0f, 0.0f,
	0.0f, 1.0f, 0.0f,
	0.0f, 1.0f, 0.0f,
	0.0f, 1.0f, 0.0f,
	0.0f, 1.0f, 0.0f,
	0.0f, 1.0f, 0.0f
},
groundTexCoord[] = {
	0.0f, 1.0f,
	1.0f, 1.0f,
	1.0f, 0.0f,
	0.0f, 1.0f,
	1.0f, 0.0f,
	0.0f, 0.0f
};
// vertex array object
unsigned int groundVAO;

mat4 getCameraCoord() {
	vec3 focusPoint = VectorAdd(camDir, camPos);
	mat4 camera = lookAt(
		camPos.x, camPos.y, camPos.z,
		focusPoint.x, focusPoint.y, focusPoint.z,
		0, 1, 0
	);
	return camera;
}

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

void drawVAO(unsigned int *VAO, int indiceNo, GLfloat freq) {
	glBindVertexArray(*VAO);				 // Select VAO
	glVertexAttrib1f(glGetAttribLocation(program, "texFrequency"), freq); // Change texture frequency
	glDrawArrays(GL_TRIANGLES, 0, indiceNo); // draw object
	glVertexAttrib1f(glGetAttribLocation(program, "texFrequency"), 1.0); // Reset texture frequency.
}

void drawSkybox() {
	mat4 trans;

	glUseProgram(noShaProgram);
	glVertexAttrib1f(glGetAttribLocation(noShaProgram, "texFrequency"), 1.0);

	// We must disable depth test and culling.
	glDisable(GL_DEPTH_TEST);
	glDisable(GL_CULL_FACE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

	// We acttivate and load the sky texture.
	glActiveTexture(GL_TEXTURE2);
	glBindTexture(GL_TEXTURE_2D, skyTex);

	// We reverse the camera matrix
	trans = InvertMat4(getCameraCoord());

	// Then we let the skybox translate with the camera instead, but keep the rotation
	trans = MultMat4(trans, MultMat4(getCameraCoord(), T(camPos.x, camPos.y -1.0, camPos.z)));
	glUniformMatrix4fv(glGetUniformLocation(noShaProgram, "tranMatrix"), 1, GL_TRUE, trans.m);
	
	// Then we draw the object with the given texture.
	DrawModel(skybox, noShaProgram, "in_Position", "in_Normal", "inTexCoord");

	// Then we reset to previous values.
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_CULL_FACE);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

}

void drawWorld() {
	GLfloat t = (GLfloat)glutGet(GLUT_ELAPSED_TIME);
	mat4 trans, rot;
	
	// First of all, we draw the skybox (can be done last in order to make it more efficient)
	drawSkybox();

	glUseProgram(program);
	glVertexAttrib1f(glGetAttribLocation(program, "texFrequency"), 1.0);

	// As we're switching shaders, we need to activate the right textures.
	glActiveTexture(GL_TEXTURE0);
	glActiveTexture(GL_TEXTURE1);

	// We draw the ground.
	glBindTexture(GL_TEXTURE_2D, groundTex); // We bind the ground texture

	trans = T(0, 0, 0);
	glUniformMatrix4fv(glGetUniformLocation(program, "tranMatrix"), 1, GL_TRUE, trans.m);
	drawVAO(&groundVAO, LENGTH(groundVert)/3, 10.0);
	
	// Then we draw the rest of the models
	glBindTexture(GL_TEXTURE_2D, whiteTex); // We bind the white texture

	trans = T(10, 0.5, 3);
	glUniformMatrix4fv(glGetUniformLocation(program, "tranMatrix"), 1, GL_TRUE, trans.m);
	DrawModel(bunny, program, "in_Position", "in_Normal", "inTexCoord");
	
	trans = T(-10, 0.5, 10);
	glUniformMatrix4fv(glGetUniformLocation(program, "tranMatrix"), 1, GL_TRUE, trans.m);
	DrawModel(cube, program, "in_Position", "in_Normal", "inTexCoord");

	// We draw the windmill.
	glUseProgram(noTexProgram); // The windmill has no texture coordinates, and must use another shader.
	trans = T(0, 0, 0);
	glUniformMatrix4fv(glGetUniformLocation(noTexProgram, "tranMatrix"), 1, GL_TRUE, trans.m);
	for (int i = 0; i < modelno - 1; i++) {
		DrawModel(windmill[i], noTexProgram, "in_Position", "in_Normal", "inTexCoord");
	}
	//After that, we place the blades.
	trans = T(4.5, 9.25, 0);
	for (int j = 0; j < 4; j++) {
		rot = Rx((t / 1000) + (j*pi / 2));
		glUniformMatrix4fv(glGetUniformLocation(noTexProgram, "tranMatrix"), 1, GL_TRUE, Mult(trans, rot).m);
		DrawModel(windmill[modelno - 1], noTexProgram, "in_Position", "in_Normal", "inTexCoord");
	}

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
	program		 = loadShaders("lab3-3.vert", "lab3-3.frag");
	noTexProgram = loadShaders("lab3-3.vert", "lab3-3_no_tex.frag");
	noShaProgram = loadShaders("lab3-3.vert", "lab3-3_no_shadow.frag");
	
	glUseProgram(program);
	printError("init shader");

	// Load and activate textures
	LoadTGATextureSimple("grass.tga", &groundTex);
	glActiveTexture(GL_TEXTURE0);
	glUniform1i(glGetUniformLocation(program, "texUnit"), 0); // Texture unit
	
	LoadTGATextureSimple("white.tga", &whiteTex);
	glActiveTexture(GL_TEXTURE1);
	glUniform1i(glGetUniformLocation(program, "texUnit"), 1);

	glUseProgram(noShaProgram);
	LoadTGATextureSimple("SkyBox512.tga", &skyTex);
	glActiveTexture(GL_TEXTURE2);
	glUniform1i(glGetUniformLocation(noShaProgram, "skyTex"), 2);
	glUseProgram(program);

	// Make the texture loop
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

	// Load the ground.
	createObjFromVert(&groundVAO, groundVert, groundNormal, groundTexCoord, LENGTH(groundVert), LENGTH(groundNormal));

	// Load the models.
	windmill[0] = LoadModelPlus("windmill/windmill-walls.obj");
	windmill[1] = LoadModelPlus("windmill/windmill-balcony.obj");
	windmill[2] = LoadModelPlus("windmill/windmill-roof.obj");
	windmill[3] = LoadModelPlus("windmill/blade.obj");
	bunny		= LoadModelPlus("bunnyplus.obj");
	cube		= LoadModelPlus("cubeplus.obj");
	skybox		= LoadModelPlus("skybox.obj");

}

void display(void)
{
	glUseProgram(program);
	// We pass a camera position matrix.
	glUniformMatrix4fv(glGetUniformLocation(program, "camMatrix"), 1, GL_TRUE, updateCameraCoord().m);
	// We also pass the world projection matrix
	glUniformMatrix4fv(glGetUniformLocation(program, "projMatrix"), 1, GL_TRUE, projectionMatrix);
	//Then we do the same for the other shader programs.
	glUseProgram(noTexProgram);
	glUniformMatrix4fv(glGetUniformLocation(noTexProgram, "camMatrix"), 1, GL_TRUE, getCameraCoord().m);
	glUniformMatrix4fv(glGetUniformLocation(noTexProgram, "projMatrix"), 1, GL_TRUE, projectionMatrix);
	
	glUseProgram(noShaProgram);
	glUniformMatrix4fv(glGetUniformLocation(noShaProgram, "camMatrix"), 1, GL_TRUE, getCameraCoord().m);
	glUniformMatrix4fv(glGetUniformLocation(noShaProgram, "projMatrix"), 1, GL_TRUE, projectionMatrix);
	
	printError("pre display");

	// clear the screen
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	
	drawWorld();

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
