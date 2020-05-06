// Lab 4, terrain generation

#ifdef __APPLE__
	#include <OpenGL/gl3.h>
	// Linking hint for Lightweight IDE
	// uses framework Cocoa
#endif
#include "MicroGlut.h"
#include "GL_utilities.h"
#include "VectorUtils3.h"
#include "loadobj.h"
#include "LoadTGA.h"

mat4 projectionMatrix;


float pitch = 0;
float yaw = -90.0f;
float radius;
float lastx = 300;
float lasty = 300;
int firstMouse = 0;
vec3 lookAtPoint, pos = {0,0,8};


float toRad(float deg) {
	return deg * (M_PI / 180.0);
}


Model* GenerateTerrain(TextureData *tex)
{
	int vertexCount = tex->width * tex->height;
	int triangleCount = (tex->width-1) * (tex->height-1) * 2;
	int x, z;
	
	GLfloat *vertexArray = malloc(sizeof(GLfloat) * 3 * vertexCount);
	GLfloat *normalArray = malloc(sizeof(GLfloat) * 3 * vertexCount);
	GLfloat *texCoordArray = malloc(sizeof(GLfloat) * 2 * vertexCount);
	GLuint *indexArray = malloc(sizeof(GLuint) * triangleCount*3);
	
	printf("bpp %d %d\n", tex->bpp, vertexCount);
	for (x = 0; x < tex->width; x++)
		for (z = 0; z < tex->height; z++)
		{
			// Vertex array. You need to scale this properly
			vertexArray[(x + z * tex->width)*3 + 0] = x / 1.0;
			vertexArray[(x + z * tex->width)*3 + 1] = tex->imageData[(x + z * tex->width) * (tex->bpp/8)] / 100.0;
			vertexArray[(x + z * tex->width)*3 + 2] = z / 1.0;
			// Normal vectors. You need to calculate these.
			normalArray[(x + z * tex->width)*3 + 0] = 0.0;
			normalArray[(x + z * tex->width)*3 + 1] = 1.0;
			normalArray[(x + z * tex->width)*3 + 2] = 0.0;
			// Texture coordinates. You may want to scale them.
			texCoordArray[(x + z * tex->width)*2 + 0] = x; // (float)x / tex->width;
			texCoordArray[(x + z * tex->width)*2 + 1] = z; // (float)z / tex->height;
		}
	for (x = 0; x < tex->width-1; x++)
		for (z = 0; z < tex->height-1; z++)
		{
			// Triangle 1
			indexArray[(x + z * (tex->width-1))*6 + 0] = x + z * tex->width;
			indexArray[(x + z * (tex->width-1))*6 + 1] = x + (z+1) * tex->width;
			indexArray[(x + z * (tex->width-1))*6 + 2] = x+1 + z * tex->width;
			// Triangle 2
			indexArray[(x + z * (tex->width-1))*6 + 3] = x+1 + z * tex->width;
			indexArray[(x + z * (tex->width-1))*6 + 4] = x + (z+1) * tex->width;
			indexArray[(x + z * (tex->width-1))*6 + 5] = x+1 + (z+1) * tex->width;
		}
	
	// End of terrain generation
	
	// Create Model and upload to GPU:

	Model* model = LoadDataToModel(
			vertexArray,
			normalArray,
			texCoordArray,
			NULL,
			indexArray,
			vertexCount,
			triangleCount*3);

	return model;
}


// vertex array object
Model *m, *m2, *tm;
// Reference to shader program
GLuint program;
GLuint tex1, tex2;
TextureData ttex; // terrain

void init(void)
{
	// GL inits
	glClearColor(0.2,0.2,0.5,0);
	glEnable(GL_DEPTH_TEST);
	glDisable(GL_CULL_FACE);
	printError("GL inits");

	projectionMatrix = frustum(-0.1, 0.1, -0.1, 0.1, 0.2, 50.0);

	// Load and compile shader
	program = loadShaders("terrain.vert", "terrain.frag");
	glUseProgram(program);
	printError("init shader");
	
	glUniformMatrix4fv(glGetUniformLocation(program, "projMatrix"), 1, GL_TRUE, projectionMatrix.m);
	glUniform1i(glGetUniformLocation(program, "tex"), 0); // Texture unit 0
	LoadTGATextureSimple("maskros512.tga", &tex1);
	
// Load terrain data
	
	LoadTGATextureData("44-terrain.tga", &ttex);
	tm = GenerateTerrain(&ttex);
	printError("init terrain");
}

void display(void)
{
	// clear the screen
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	
	mat4 total, modelView, camMatrix;
	
	printError("pre display");
	
	glUseProgram(program);


	const float mv = 0.1;
	int movedSideways = 0;
	if (glutKeyIsDown('s')) {
		pos.x -= mv * lookAtPoint.x;
		pos.y -= mv * lookAtPoint.y;
		pos.z -= mv * lookAtPoint.z;
	}

	if (glutKeyIsDown('d')) {
		vec3 right;
		right.x = cosf(toRad(yaw + 90)) * cosf(toRad(pitch));
		right.y = sinf(toRad(pitch));
		right.z = sinf(toRad(yaw + 90)) * cosf(toRad(pitch));
		pos.x += mv * right.x;
		pos.y += mv * right.y;
		pos.z += mv * right.z;
		movedSideways = 1;
	}


	if (glutKeyIsDown('a')) {
		vec3 left;
		left.x = cosf(toRad(yaw - 90)) * cosf(toRad(pitch));
		left.y = sinf(toRad(pitch));
		left.z = sinf(toRad(yaw - 90)) * cosf(toRad(pitch));
		pos.x += mv * left.x;
		pos.y += mv * left.y;
		pos.z += mv * left.z;
		movedSideways = 1;
	}

	if (glutKeyIsDown('w')) {
		pos.x += mv * lookAtPoint.x;
		pos.y += mv * lookAtPoint.y;
		pos.z += mv * lookAtPoint.z;
	}

	vec3 combined;
	if(movedSideways) {

	}
	// Build matrix
	
	camMatrix = lookAt(pos.x, pos.y, pos.z, lookAtPoint.x + pos.x, lookAtPoint.y + pos.y, lookAtPoint.z + pos.z, 0.0, 1.0, 0.0);
	modelView = IdentityMatrix();
	total = Mult(camMatrix, modelView);
	glUniformMatrix4fv(glGetUniformLocation(program, "mdlMatrix"), 1, GL_TRUE, total.m);
	
	glBindTexture(GL_TEXTURE_2D, tex1);		// Bind Our Texture tex1
	DrawModel(tm, program, "inPosition", "inNormal", "inTexCoord");
	
	printError("display 2");
	
	glutSwapBuffers();

}

void timer(int i)
{
	glutTimerFunc(20, &timer, i);
	glutPostRedisplay();
}


void mouse(int x, int y)
{

	const float sens = 0.05f;
	yaw +=  (x - lastx) * sens;
	yaw = fmod(yaw, 360.0f);
    pitch +=  (lasty - y) * sens;

	if(pitch > 89.0f)
		pitch =  89.0f;
	if(pitch < -89.0f)
		pitch = -89.0f;

	//printf("%d %d\n", x, y);
	vec3 cameraFront;
	cameraFront.x = cosf(toRad(yaw)) * cosf(toRad(pitch));
    cameraFront.y = sinf(toRad(pitch));
    cameraFront.z = sinf(toRad(yaw)) * cosf(toRad(pitch));
	lookAtPoint = Normalize(cameraFront);
	
	lastx = x;
	lasty = y;

}

int main(int argc, char **argv)
{

	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_DEPTH);
	glutInitContextVersion(3, 2);
	glutInitWindowSize (600, 600);
	glutCreateWindow ("TSBK07 Lab 4");
	glutDisplayFunc(display);
	init ();
	glutTimerFunc(20, &timer, 0);

	glutPassiveMotionFunc(mouse);

	glutMainLoop();
	exit(0);
}
