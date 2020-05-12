

#include "Demos.h"
#define _CRT_SECURE_NO_WARNINGS

Geometry* DemoOne() {
	AnimationShader* as = new AnimationShader();
	glUseProgram(as->getProgram());
	Geometry* g = new Geometry{ "teapot.obj", as->getProgram() };

	const int numInstances = 24;
	std::vector<GLfloat> startPositions(numInstances * 3);
	std::vector<GLfloat> endPositions(numInstances * 3);
	std::vector<GLfloat> startTimes(numInstances);
	std::vector<GLfloat> sizes(numInstances);
	std::vector<GLfloat> colors(numInstances * 3);

	float dist = 24.0;
	for (int i = 0; i < numInstances; i++) {
		float rx = ((double)rand() / (RAND_MAX));
		float ry = ((double)rand() / (RAND_MAX));
		float rz = ((double)rand() / (RAND_MAX));

		startPositions[(i * 3)] = (float)rx * dist;
		startPositions[(i * 3) + 1] = (float)ry * dist;
		startPositions[(i * 3) + 2] = (float)rz * dist;

		endPositions[(i * 3)] = (float)0;
		endPositions[(i * 3) + 1] = (float)0;
		endPositions[(i * 3) + 2] = (float)0;

		sizes[i] = (rx + 1.0);

		colors[(i * 3)] = (rx + 1.0) / 2.5;
		colors[(i * 3) + 1] = (ry + 1.0) / 2.5;
		colors[(i * 3) + 2] = (rz + 1.0) / 2.5;
		//colors[(i * 3) + 3] =((double)rand() / (RAND_MAX)) + 1;
		startTimes[i] = 0;
	}
	GeometryAttributeBuffers buffers;
	buffers.startPositions = startPositions;
	buffers.endPositions = endPositions;
	buffers.startTimes = startTimes;
	buffers.sizes = sizes;
	buffers.instanceCount = numInstances;


	g->setUpInstanceBuffers(buffers);
	return g;
}


Geometry* DemoTwo() {
  AnimationShader* as = new AnimationShader();
	Geometry* g = new Geometry{ "bunny.obj", as->getProgram() };

	const int numInstances = 200;
	std::vector<GLfloat> startPositions(numInstances * 3);
	std::vector<GLfloat> endPositions(numInstances * 3);
	std::vector<GLfloat> startTimes(numInstances);
	std::vector<GLfloat> sizes(numInstances);
	std::vector<GLfloat> colors(numInstances * 3);

	float dist = 50.0;
	for (int i = 0; i < numInstances; i++) {
		float rx = ((double)rand() / (RAND_MAX));
		float ry = ((double)rand() / (RAND_MAX));
		float rz = ((double)rand() / (RAND_MAX));

		startPositions[(i * 3)] = (float)rx * dist;
		startPositions[(i * 3) + 1] = (float)ry * dist;
		startPositions[(i * 3) + 2] = (float)rz * dist;

		endPositions[(i * 3)] = (float)0;
		endPositions[(i * 3) + 1] = (float)0;
		endPositions[(i * 3) + 2] = (float)0;

		sizes[i] = (rx + 1.0);

		colors[(i * 3)] = (rx + 1.0) / 2.5;
		colors[(i * 3) + 1] = (ry + 1.0) / 2.5;
		colors[(i * 3) + 2] = (rz + 1.0) / 2.5;
		//colors[(i * 3) + 3] =((double)rand() / (RAND_MAX)) + 1;
		startTimes[i] = 0;
	}
	GeometryAttributeBuffers buffers;
	buffers.startPositions = startPositions;
	buffers.endPositions = endPositions;
	buffers.startTimes = startTimes;
	buffers.sizes = sizes;
	buffers.instanceCount = numInstances;


	g->setUpInstanceBuffers(buffers);
	return g;
}

float getValue(TextureData* tex, int x, int z, float scale) {
	if (x < 0 || x > tex->width - 1 || z < 0 || z > tex->height - 1) return 0;
	return tex->imageData[(x + z * tex->width) * (tex->bpp / 8)] / scale;
}


Geometry* DemoThree() {
	const int width = 100;
	const int height = 100;

	int vertexCount = width*height;
	int triangleCount = (width-1)*(height-1) * 2;
	int x, z;


	float heightScale = 10.0;
	TextureData ttex;
	LoadTGATextureData("fft-terrain.tga", &ttex);

	std::vector<GLfloat> vertexArray(3 * vertexCount);
	std::vector<GLfloat> normalArray(3 * vertexCount);
	std::vector<GLuint> indexArray(3 * triangleCount);


	//GLfloat* vertexArray = (GLfloat*)malloc(sizeof(GLfloat) * 3 * vertexCount);
	//GLuint* indexArray = (GLuint*)malloc(sizeof(GLuint) * triangleCount * 3);

	for (x = 0; x < width; x++)
		for (z = 0; z < height; z++)
		{
			// Vertex array. You need to scale this properly
			float h = getValue(&ttex, x, z, heightScale);
			vertexArray[(x + z * width) * 3 + 0] = x*3.0;
			vertexArray[(x + z * width) * 3 + 1] = h* 3.0;
			vertexArray[(x + z * width) * 3 + 2] = z * 3.0;
			// Normal vectors. You need to calculate these.

				// https://stackoverflow.com/questions/49640250/calculate-normals-from-heightmap
			// vec3 normal = vec3(2*(R-L), 2*(B-T), -4).Normalize();
			float L = getValue(&ttex, x - 1, z, heightScale);
			float R = getValue(&ttex, x + 1, z, heightScale);
			float T = getValue(&ttex, x, z - 1, heightScale);
			float B = getValue(&ttex, x, z + 1, heightScale);
			vec3 comb = { (float)2. * (R - L), -4 ,2. * (B - T) };
			vec3 normal = Normalize(comb);


		}
	for (x = 0; x < width - 1; x++)
		for (z = 0; z < height - 1; z++)
		{
			// Triangle 1
			indexArray[(x + z * (width - 1)) * 6 + 0] = x + z * width;
			indexArray[(x + z * (width - 1)) * 6 + 1] = x + (z + 1) * width;
			indexArray[(x + z * (width - 1)) * 6 + 2] = x + 1 + z * width;
			// Triangle 2
			indexArray[(x + z * (width - 1)) * 6 + 3] = x + 1 + z * width;
			indexArray[(x + z * (width - 1)) * 6 + 4] = x + (z + 1) * width;
			indexArray[(x + z * (width - 1)) * 6 + 5] = x + 1 + (z + 1) * width;
		}


	std::cout << "loading model" << std::endl;


	// Create Model and upload to GPU:
	Model* model = LoadDataToModel(
		&vertexArray[0],
		&normalArray[0],
		NULL,
		NULL,
		&indexArray[0],
		vertexCount,
		triangleCount
    );


	std::cout << "model loaded" << std::endl;
    AnimationShader* as = new AnimationShader();
	Geometry* g = new Geometry{ model, as->getProgram() };

	const int numInstances = vertexCount;
	std::vector<GLfloat> startPositions(numInstances * 3);
	std::vector<GLfloat> endPositions(numInstances * 3);
	std::vector<GLfloat> startTimes(numInstances);
	std::vector<GLfloat> sizes(numInstances);
	std::vector<GLfloat> colors(numInstances * 3);

	float dist = 50.0;
	for (int i = 0; i < numInstances; i++) {
		float rx = ((double)rand() / (RAND_MAX));
		float ry = ((double)rand() / (RAND_MAX));
		float rz = ((double)rand() / (RAND_MAX));

		startPositions[(i * 3)] = (float)rx * dist;
		startPositions[(i * 3) + 1] = (float)ry * dist;
		startPositions[(i * 3) + 2] = (float)rz * dist;

		endPositions[(i * 3)] = (float)0;
		endPositions[(i * 3) + 1] = (float)0;
		endPositions[(i * 3) + 2] = (float)0;

		sizes[i] = (rx + 1.0);

		colors[(i * 3)] = (rx + 1.0) / 2.5;
		colors[(i * 3) + 1] = (ry + 1.0) / 2.5;
		colors[(i * 3) + 2] = (rz + 1.0) / 2.5;
		//colors[(i * 3) + 3] =((double)rand() / (RAND_MAX)) + 1;
		startTimes[i] = 0;
	}
  
	GeometryAttributeBuffers buffers;
	buffers.startPositions = startPositions;
	buffers.endPositions = endPositions;
	buffers.startTimes = startTimes;
	buffers.sizes = sizes;
	buffers.instanceCount = 1;

	g->setUpInstanceBuffers(buffers);

	std::cout << "Buffers set up" << std::endl;
	return g;
}

