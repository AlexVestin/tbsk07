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

#define PI 3.14159265359
float pitch = 0;
float yaw = -90.0f;
float radius;
float lastx = 300;
float lasty = 300;
int firstMouse = 0;
bool leftMouseDown = false;
vec3 lookAtPoint, pos = {0,0,8};

float maxheight = 1.0;
float heightScale = 25.f;
float xScale = 4.0f;
float zScale = 4.0f;


const int amt = 2000;
GLuint vao, vbo, ivbo, nbo;
GLuint treeProgram;
GLuint dbVao, dbProgram, numNormals;


float toRad(float deg) {
	return deg * (M_PI / 180.0);
}


float getValue(TextureData* tex, int x, int z, float scale) {
	if(x < 0 || x > tex->width -1 || z < 0 || z > tex->height -1) return 0;
	return tex->imageData[(x + z * tex->width) * (tex->bpp/8)] / heightScale;
}

Model* GenerateTerrain(TextureData *tex) {
	int vertexCount = tex->width * tex->height;
	int triangleCount = (tex->width-1) * (tex->height-1) * 2;
	int x, z;
	
	GLfloat *vertexArray = malloc(sizeof(GLfloat) * 3 * vertexCount);
	GLfloat *normalArray = malloc(sizeof(GLfloat) * 3 * vertexCount);
	GLfloat *texCoordArray = malloc(sizeof(GLfloat) * 2 * vertexCount);
	GLuint *indexArray = malloc(sizeof(GLuint) * triangleCount*3);


	// Two points of three coordinates per triangle
	GLfloat* debugNormalArray = malloc(sizeof(GLfloat) * 2 * 3 * vertexCount);
	numNormals = vertexCount * 2;

	printf("bpp %d %d\n", tex->bpp, vertexCount);
	for (x = 0; x < tex->width; x++)
		for (z = 0; z < tex->height; z++)
		{
			// Vertex array. You need to scale this properly
			float h = getValue(tex, x, z, heightScale);
			vertexArray[(x + z * tex->width)*3 + 0] = x / xScale;
			vertexArray[(x + z * tex->width)*3 + 1] = h;
			vertexArray[(x + z * tex->width)*3 + 2] = z / zScale;
			// Normal vectors. You need to calculate these.
			
			if(h > maxheight) {
				maxheight = h;

			}
			// https://stackoverflow.com/questions/49640250/calculate-normals-from-heightmap
			// vec3 normal = vec3(2*(R-L), 2*(B-T), -4).Normalize();
			float L = getValue(tex, x-1, z, heightScale);
			float R = getValue(tex, x+1, z, heightScale);
			float T = getValue(tex, x, z-1, heightScale);
			float B = getValue(tex, x, z+1, heightScale);
			vec3 comb = { 2.*(R-L), -4 ,2. * (B-T) };
			vec3 normal = Normalize(comb);

			float interpolatedHeight = (L+R+T+B) / 4.0;
			debugNormalArray[(x + z * tex->width)*6 + 0] = (x) / xScale;
			debugNormalArray[(x + z * tex->width)*6 + 1] = h;
			debugNormalArray[(x + z * tex->width)*6 + 2] = (z) / zScale;


			float lineLengthDiv = 10.;	
			debugNormalArray[(x + z * tex->width)*6 + 3] = (x) / xScale - normal.x / lineLengthDiv;
			debugNormalArray[(x + z * tex->width)*6 + 4] = h - normal.y / lineLengthDiv;
			debugNormalArray[(x + z * tex->width)*6 + 5] = (z) /zScale - normal.z / lineLengthDiv;


			normalArray[(x + z * tex->width)*3 + 0] = normal.x;
			normalArray[(x + z * tex->width)*3 + 1] = normal.y;
			normalArray[(x + z * tex->width)*3 + 2] = normal.z;

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
	
	// End of terrain generatio	
	dbProgram = loadShaders("debug_normal.vert", "debug_normal.frag");
	glUseProgram(dbProgram);
	glUniformMatrix4fv(glGetUniformLocation(dbProgram, "projMatrix"), 1, GL_TRUE, projectionMatrix.m);

	// Set up debug normal-line buffers
	GLuint dbvbo;
	glGenVertexArrays(1, &dbVao);
	glBindVertexArray(dbVao);
	glGenBuffers(1, &dbvbo);
	glBindBuffer(GL_ARRAY_BUFFER, dbvbo);
	glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat) * 2 * 3 * vertexCount, debugNormalArray, GL_STATIC_DRAW);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0); 
	glEnableVertexAttribArray(0);
	
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
GLuint tex1, tex2, snowTex;
TextureData ttex; // terrain
TextureData stex; // snow


// sphere stuff
Model *sphere;
vec3 spherePosition = {0, 0, 0};
GLuint sphereProgram;



// tree stuff
Model *tree;


float lambda1(vec3 p1, vec3 p2, vec3 p3, float x, float z) {
	float num = (p2.z - p3.z)*(x - p3.x) + (p3.x - p2.x) * (z-p3.z);
	float den = (p2.z - p3.z)*(p1.x - p3.x) + (p3.x - p2.x)*(p1.z-p3.z);
	return num / den;
}

float lambda2(vec3 p1, vec3 p2, vec3 p3, float x, float z) {
	float num = (p3.z - p1.z)*(x - p3.x) + (p1.x - p3.x)*(z-p3.z);
	float den = (p2.z - p3.z)*(p1.x - p3.x) + (p3.x - p2.x)*(p1.z-p3.z);
	return num / den;
}


void moveSphere() {
	float moveAmount = 0.1;
	// Move diagonally across map
	spherePosition.x += moveAmount / xScale;
	spherePosition.z += moveAmount / zScale;

	// Get the position of the sphere in the terrain texture quad
	float x = (spherePosition.x * xScale);
	float z = (spherePosition.z * zScale);
	
	// Round down to get position of bottom left corner of a quad
	int rx = (int)x;
	int rz = (int)z;

	// Get bounds height values
	float bl = getValue(&ttex, rx, rz, heightScale);
	float br = getValue(&ttex, rx+1, rz, heightScale);
	float tl = getValue(&ttex, rx, rz+1, heightScale);
	float tr = getValue(&ttex, rx+1, rz+1, heightScale);


	// get offset of our point from the lower left corner
	float ox = x - rx;
	float oz = z - rz;

	
	// Use barycentric coordinates + linear interpolation
	// https://stackoverflow.com/questions/36090269/finding-height-of-point-on-height-map-triangles
	// Since the x & z values are always either 0 or 1 (when we have 1:1 xz-scale) we could simplify this, but ehh
	vec3 p1, p2, p3;
	if(ox + oz > 1.0) {
		// in the "right-hand" triangle
		//float l1 = 1 - z;
		//float l2 = 1 - z - x;
		//float l3 = 1 - l1 - l2;

		p1 = (vec3){ 1.f, br, 0.f };
		p2 = (vec3){ 1.f, tr, 1.f };
		p3 = (vec3){ 0.f, tl, 1.f };
	} else {
		// in the "left-hand" triangle
		p1 = (vec3){ 0.f, bl, 0.f };
		p2 = (vec3){ 0.f, tl, 1.f };
		p3 = (vec3){ 1.f, br, 0.f };
	}

	float l1 = lambda1(p1, p2, p3, ox, oz);
	float l2 = lambda2(p1, p2, p3, ox, oz);
	float l3 = 1 - l1 - l2;

	float intz = l1*p1.y + l2*p2.y + l3*p3.y;  
	spherePosition.y = intz;
}

void init(void)
{
	// GL inits
	glClearColor(0.2,0.2,0.5,0);
	glEnable(GL_DEPTH_TEST);
	glDisable(GL_CULL_FACE);
	printError("GL inits");

	projectionMatrix = frustum(-0.1, 0.1, -0.1, 0.1, 0.2, 50.0);

	// Load and compile shader
	program = loadShaders("terrain_snowy.vert", "terrain_snowy.frag");
	glUseProgram(program);
	printError("init shader");
	
	glUniformMatrix4fv(glGetUniformLocation(program, "projMatrix"), 1, GL_TRUE, projectionMatrix.m);
	glUniform1f(glGetUniformLocation(program, "maxheight"), maxheight);

	glUniform1i(glGetUniformLocation(program, "tex"), 0); // Texture unit 0
	LoadTGATextureSimple("maskros512.tga", &tex1);
	glUniform1i(glGetUniformLocation(program, "snowTex"), 1); // Texture unit 0
	LoadTGATextureSimple("snow_r.tga", &snowTex);

	// Load terrain data
	
	LoadTGATextureData("fft-terrain.tga", &ttex);
	tm = GenerateTerrain(&ttex);
	printError("init terrain");

	// Load sphere stuff
	sphereProgram = loadShaders("sphere.vert", "sphere.frag");

	glUseProgram(sphereProgram);
	glUniformMatrix4fv(glGetUniformLocation(sphereProgram, "projMatrix"), 1, GL_TRUE, projectionMatrix.m);
	sphere = LoadModelPlus("groundsphere.obj");
	spherePosition.y = getValue(&ttex, 0, 0, heightScale);
	printError("load sphere");

	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glEnable( GL_BLEND );


	// Load tree model to get vertices + other stuff
	treeProgram = loadShaders("tree.vert", "tree.frag");
	glUseProgram(treeProgram);
	glUniformMatrix4fv(glGetUniformLocation(treeProgram, "projMatrix"), 1, GL_TRUE, projectionMatrix.m);

	tree = LoadModelPlus("models/tree_pineGroundA.obj");

	int i;
	glGenVertexArrays(1, &vao);
	glBindVertexArray(vao);

	glGenBuffers(1, &vbo);
	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glBufferData(GL_ARRAY_BUFFER, tree->numVertices*3*sizeof(GLfloat), tree->vertexArray, GL_STATIC_DRAW);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0); 
	glEnableVertexAttribArray(0);
	glVertexAttribDivisor(0, 0);

	glGenBuffers(1, &nbo);
	glBindBuffer(GL_ARRAY_BUFFER, nbo);
	glBufferData(GL_ARRAY_BUFFER, tree->numVertices*3*sizeof(GLfloat), tree->normalArray, GL_STATIC_DRAW);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, 0); 
	glEnableVertexAttribArray(1);
	glVertexAttribDivisor(1, 0);

	
	glGenBuffers(1, &ivbo);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ivbo);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, tree->numIndices*sizeof(GLuint), tree->indexArray, GL_STATIC_DRAW);


	GLfloat positions[amt*16]; 
	int j;
	SetTransposed(1);

	for(i = 0; i < amt; i++) {
		int x = (rand() / (float)RAND_MAX) * (float)ttex.width;
		int z = (rand() / (float)RAND_MAX) * (float)ttex.height;
		float height = getValue(&ttex, x, z, heightScale);

		float px = (float) x / xScale;
		float py = height;
		float pz = (float) z / zScale;

		float nx = tm->normalArray[(x + z * ttex.width)*3 + 0];
		float ny = tm->normalArray[(x + z * ttex.width)*3 + 1];
		float nz = tm->normalArray[(x + z * ttex.width)*3 + 2];

		vec3 normal = { nx, ny, nz };
		vec3 upVector = {0, -1, 0};
		vec3 axis = CrossProduct(upVector, normal);	
		float lm = sqrt(pow(nx, 2) + pow(ny, 2) + pow(nz, 2));
		float angle = acosf(DotProduct(normal, upVector) / lm);
		mat4 m = Mult(T(px, py, pz), ArbRotate(axis, angle));


		for(j = 0; j < 16; j++) {
			positions[i*16 + j] = m.m[j];
		}
	}

	SetTransposed(0);

	GLuint obo;
	glGenBuffers(1, &obo);
	glBindBuffer(GL_ARRAY_BUFFER, obo);
	glBufferData(GL_ARRAY_BUFFER, amt*16*sizeof(GLfloat), positions, GL_STATIC_DRAW);
	
	// Use multiple attibs, needed to have mat4 in shader
	glEnableVertexAttribArray(2);
	glEnableVertexAttribArray(3);
	glEnableVertexAttribArray(4);
	glEnableVertexAttribArray(5);

	glVertexAttribPointer(2, 4, GL_FLOAT, GL_FALSE, 16*sizeof(GLfloat), (void*) (0)); 
	glVertexAttribPointer(3, 4, GL_FLOAT, GL_FALSE, 16*sizeof(GLfloat), (void*) (sizeof(GLfloat) * 4)); 
	glVertexAttribPointer(4, 4, GL_FLOAT, GL_FALSE, 16*sizeof(GLfloat), (void*) (sizeof(GLfloat) * 8)); 
	glVertexAttribPointer(5, 4, GL_FLOAT, GL_FALSE, 16*sizeof(GLfloat), (void*) (sizeof(GLfloat) * 12)); 

	glVertexAttribDivisor(2, 1);
	glVertexAttribDivisor(3, 1);
	glVertexAttribDivisor(4, 1);
	glVertexAttribDivisor(5, 1);
}

void handleKeyPress() {
		const float mv = 0.1;
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
	}

	if (glutKeyIsDown(GLUT_KEY_SPACEBAR)) {
		pos.y += mv;		
	}

	if (glutKeyIsDown(GLUT_KEY_LEFT_SHIFT)) {
		pos.y -= mv;		
	}


	if (glutKeyIsDown('a')) {
		vec3 left;
		left.x = cosf(toRad(yaw - 90)) * cosf(toRad(pitch));
		left.y = sinf(toRad(pitch));
		left.z = sinf(toRad(yaw - 90)) * cosf(toRad(pitch));
		pos.x += mv * left.x;
		pos.y += mv * left.y;
		pos.z += mv * left.z;
	}

	if (glutKeyIsDown('w')) {
		pos.x += mv * lookAtPoint.x;
		pos.y += mv * lookAtPoint.y;
		pos.z += mv * lookAtPoint.z;
	}
}

void display(void)
{
	// clear the screen
	GLfloat t = (GLfloat)glutGet(GLUT_ELAPSED_TIME);

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	
	mat4 total, modelView, camMatrix;
	vec3 lightPos = {spherePosition.x, spherePosition.y + 3., spherePosition.z};
	printError("pre display");
	handleKeyPress();
	// Draw terrain
	glUseProgram(program);
	camMatrix = lookAt(pos.x, pos.y, pos.z, lookAtPoint.x + pos.x, lookAtPoint.y + pos.y, lookAtPoint.z + pos.z, 0.0, 1.0, 0.0);
	modelView = IdentityMatrix();
	glUniformMatrix4fv(glGetUniformLocation(program, "viewMatrix"), 1, GL_TRUE, camMatrix.m);
	glUniformMatrix4fv(glGetUniformLocation(program, "mdlMatrix"), 1, GL_TRUE, modelView.m);
	glUniform3f(glGetUniformLocation(program, "lightPos"), lightPos.x, lightPos.y, lightPos.z);	
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, tex1);		// Bind Our Texture tex1
	glActiveTexture(GL_TEXTURE1);
	glBindTexture(GL_TEXTURE_2D, snowTex);	
	DrawModel(tm, program, "inPosition", "inNormal", "inTexCoord");

	// draw sphere
	moveSphere();
	glUseProgram(sphereProgram);

	//
	mat4 sphereMdl = Mult(T(spherePosition.x, spherePosition.y, spherePosition.z), Ry(t/1000));
	glUniformMatrix4fv(glGetUniformLocation(sphereProgram, "viewMatrix"), 1, GL_TRUE, camMatrix.m);
	glUniformMatrix4fv(glGetUniformLocation(sphereProgram, "mdlMatrix"), 1, GL_TRUE, sphereMdl.m);
	glUniform1f(glGetUniformLocation(sphereProgram, "iTime"), t/1000);
	glUniform3f(glGetUniformLocation(sphereProgram, "lightPos"), lightPos.x, lightPos.y, lightPos.z);
	DrawModel(sphere, sphereProgram, "inPosition", "inNormal", "inTexCoord");


	// DRAW TREES AND STUFF
	mat4 treeMdl = IdentityMatrix();
	glUniformMatrix4fv(glGetUniformLocation(sphereProgram, "mdlMatrix"), 1, GL_TRUE, treeMdl.m);
	glUseProgram(treeProgram);
	glUniform1f(glGetUniformLocation(sphereProgram, "iTime"), t/1000);
	glUniformMatrix4fv(glGetUniformLocation(treeProgram, "viewMatrix"), 1, GL_TRUE, camMatrix.m);
	glBindVertexArray(vao);
	glDrawElementsInstanced(GL_TRIANGLES, tree->numIndices, GL_UNSIGNED_INT, 0L, amt);

	// Debug normals
	bool debug = true;
	if(debug) {
		glUseProgram(dbProgram);
		glUniformMatrix4fv(glGetUniformLocation(dbProgram, "viewMatrix"), 1, GL_TRUE, camMatrix.m);
		glBindVertexArray(dbVao);
		glDrawArrays(GL_LINES, 0, numNormals);
	}

	printError("display 2");
	glutSwapBuffers();
}

void timer(int i)
{
	glutTimerFunc(20, &timer, i);
	glutPostRedisplay();
}

void mouse(int x, int y) {
	if(leftMouseDown) {
		const float sens = 0.15f;
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
	}
	lastx = x;
	lasty = y;
}

void mousePress(int button, int state, int x , int y) {
	if(button == 0) {
		lastx = x;
		lasty = y;
		leftMouseDown = !state;
	}
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
	glutMouseFunc(mousePress);

	glutMainLoop();
	exit(0);
}
