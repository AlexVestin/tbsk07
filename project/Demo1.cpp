

#include "Demos.h"
#define _CRT_SECURE_NO_WARNINGS



Geometry* Demo1() {
	AnimationShader* as = new AnimationShader();

	const int numInstances = 20000;
	std::vector<GLfloat> startPositions(numInstances * 3);
	std::vector<GLfloat> endPositions(numInstances * 3);
	std::vector<GLfloat> startTimes(numInstances);
	std::vector<GLfloat> sizes(numInstances);
	std::vector<GLfloat> colors(numInstances * 3);

	float dist = 1900.0;
	for (int i = 0; i < numInstances; i++) {
		float rx = ((double)rand() / (RAND_MAX)) - 0.5;
		float ry = ((double)rand() / (RAND_MAX)) - 0.5;
		float rz = ((double)rand() / (RAND_MAX)) - 0.5;

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
	buffers.duration = 10.;


	Geometry* g = new Geometry{buffers, "teapot.obj", as->getProgram(), GL_TRIANGLES };
	return g;
}


Geometry* Demo2() {

  AnimationShader* as = new AnimationShader(AnimationShader::NoiseMovement());

	const int numInstances = 2000;
	std::vector<GLfloat> startPositions(numInstances * 3);
	std::vector<GLfloat> endPositions(numInstances * 3);
	std::vector<GLfloat> startTimes(numInstances);
	std::vector<GLfloat> sizes(numInstances);
	std::vector<GLfloat> colors(numInstances * 3);

	float dist = 500.0;
	for (int i = 0; i < numInstances; i++) {
		float rx = ((double)rand() / (RAND_MAX)) - 0.5;
		float ry = ((double)rand() / (RAND_MAX)) - 0.5;
		float rz = ((double)rand() / (RAND_MAX)) - 0.5;

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

	Geometry* g = new Geometry{buffers, "bunny.obj", as->getProgram() };
	return g;
}

float getValue(TextureData* tex, int x, int z, float scale) {
	if (x < 0 || x > tex->width - 1 || z < 0 || z > tex->height - 1) return 0;
	return tex->imageData[(x + z * tex->width) * (tex->bpp / 8)] / scale;
}


Geometry* Demo3() {
	TextureData ttex;

	std::string path = __FILE__; //gets source code path, include file name
	path = path.substr(0, 1 + path.find_last_of('\\')) + "\\" + "fft-terrain.tga";

	// LoadTGASimple only takes char* and not const char*, so we need to make it writeable
	char* writable = new char[path.size() + 1];
	std::copy(path.begin(), path.end(), writable);
	writable[path.size()] = '\0'; // don't forget the terminat


	LoadTGATextureData(writable, &ttex);
	const int width = ttex.width;
	const int height = ttex.height;

	int vertexCount = width*height;
	int triangleCount = (width-1)*(height-1) * 2;
	int x, z;


	float heightScale = 10.0;

	std::vector<GLfloat> vertexArray(3 * vertexCount);
	std::vector<GLfloat> normalArray(3 * vertexCount);
	std::vector<GLuint> indexArray(3 * triangleCount);

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
			vec3 comb = { 2.f * (R - L), -4 ,2.f * (B - T) };
			vec3 normal = Normalize(comb);


			normalArray[(x + z * width) * 3 + 0] = normal.x;
			normalArray[(x + z * width) * 3 + 1] = normal.y;
			normalArray[(x + z * width) * 3 + 2] = normal.z;

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
		endPositions[(i * 3) + 1] = (float)20;
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

	AnimationShaderInput asi;
	asi.vertexSnippets = std::vector<std::string>{
		
		AnimationShader::NoiseMovement(10, 4),
	};
	AnimationShader* as = new AnimationShader(asi);

	Geometry* g = new Geometry{buffers, model, as->getProgram(), GL_TRIANGLES };

	std::cout << "Buffers set up" << std::endl;
	return g;
}


GeometryAttributeBuffers getRandomParticlePositions(int numVertices, float duration, float spread) {

	std::vector<GLfloat> startPositions(numVertices * 3);
	std::vector<GLfloat> endPositions(numVertices * 3);
	std::vector<GLfloat> startTimes(numVertices);
	std::vector<GLfloat> sizes(numVertices);
	std::vector<GLfloat> colors(numVertices * 3);

	for (int i = 0; i < numVertices; i++) {
		float rx = ((double)rand() / (RAND_MAX));
		float ry = ((double)rand() / (RAND_MAX));
		float rz = ((double)rand() / (RAND_MAX));

		startPositions[(i * 3)] = (float)rx * spread;
		startPositions[(i * 3) + 1] = (float)ry * spread;
		startPositions[(i * 3) + 2] = (float)rz * spread;

		endPositions[(i * 3)] = (float)0;
		endPositions[(i * 3) + 1] = (float)0;
		endPositions[(i * 3) + 2] = (float)0;

		sizes[i] = (rx * 3.0 + 1.0);
		startTimes[i] = ((float)i / numVertices) * duration;
	}

	GeometryAttributeBuffers buffers;
	buffers.startPositions = startPositions;
	buffers.endPositions = endPositions;
	buffers.startTimes = startTimes;
	buffers.sizes = sizes;
	buffers.instanceCount = numVertices;
	return buffers;
}


Geometry* Demo4() {
	const int numVertices = 100000;

	AnimationShaderInput asi;
	asi.vertexSnippets = std::vector<std::string>{
		AnimationShader::Tube(2.0, 0.12, 10),
		AnimationShader::NoiseMovement(14, 34)
	};
	AnimationShader* as = new AnimationShader(asi);
	float duration = 60.0;
	GeometryAttributeBuffers buffers = getRandomParticlePositions(numVertices, duration, 400.);
	buffers.duration = duration;

	return new Geometry{ buffers, as->getProgram(), GL_POINTS, DrawFunc::ARRAY };
}


Geometry* Demo5() {
	const int numVertices = 100000;

	AnimationShaderInput asi;
	asi.vertexSnippets = std::vector<std::string>{
		AnimationShader::Tube(2.0, 0.12, 20),
		AnimationShader::NoiseMovement(14, 26)
	};
	AnimationShader* as = new AnimationShader(asi);
	float duration = 60.0;
	GeometryAttributeBuffers buffers = getRandomParticlePositions(numVertices, duration, 400.);
	buffers.duration = duration;
	buffers.repeat = true;

	return new Geometry{ buffers, as->getProgram(), GL_POINTS, DrawFunc::ARRAY };
}


Geometry* Demo6() {
	const int numVertices = 3000;

	AnimationShaderInput asi;
	asi.vertexSnippets = std::vector<std::string>{
		AnimationShader::Tube(2.0, 0.12, 10),
		AnimationShader::Merge(1.0)
	};
	AnimationShader* as = new AnimationShader(asi);
	float duration = 5.0;
	GeometryAttributeBuffers buffers = getRandomParticlePositions(numVertices, duration, 400.);
	buffers.duration = duration;
	buffers.repeat = true;

	return new Geometry{ buffers, as->getProgram(), GL_POINTS, DrawFunc::ARRAY };
}

Geometry* Demo7() {
	const int numVertices = 30000;

	AnimationShaderInput asi;
	asi.vertexSnippets = std::vector<std::string>{
		AnimationShader::Gravity(),
		AnimationShader::NoiseMovement(25, 10)
	};
	AnimationShader* as = new AnimationShader(asi);
	float duration = 20.0;

	std::vector<GLfloat> startPositions(numVertices * 3);
	std::vector<GLfloat> endPositions(numVertices * 3);
	std::vector<GLfloat> startTimes(numVertices);
	std::vector<GLfloat> sizes(numVertices);
	std::vector<GLfloat> colors(numVertices * 3);
	
	float spread = 25.;

	for (int i = 0; i < numVertices; i++) {
		float rx = ((double)rand() / (RAND_MAX)) -0.5;
		float ry = ((double)rand() / (RAND_MAX)) -0.5;
		float rz = ((double)rand() / (RAND_MAX)) -0.5;

		endPositions[(i * 3)] = (float)rx * spread;
		endPositions[(i * 3) + 1] = (float)70;
		endPositions[(i * 3) + 2] = (float)rz * spread;

		startPositions[(i * 3)] = (float)0;
		startPositions[(i * 3) + 1] = (float)0;
		startPositions[(i * 3) + 2] = (float)0;

		colors[(i * 3)] = ((i + 500) % 255) / 255.;
		colors[(i * 3) + 1] = (float) (i % 255) / 255.;
		colors[(i * 3) + 2] = (float) ((i + 88) % 255) / 255.;

		sizes[i] = (rx * 5.0 + 1.0);
		startTimes[i] =(i / (float)numVertices) * duration;
	}

	GeometryAttributeBuffers buffers;
	buffers.startPositions = startPositions;
	buffers.endPositions = endPositions;
	buffers.startTimes = startTimes;
	buffers.sizes = sizes;
	buffers.instanceCount = numVertices;
	buffers.duration = duration;
	buffers.colors = colors;


	return new Geometry{ buffers, as->getProgram(), GL_POINTS, DrawFunc::ARRAY };
}


Geometry* Demo8() {
	const int numVertices = 30000;

	AnimationShaderInput asi;
	asi.vertexSnippets = std::vector<std::string>{
		AnimationShader::Gravity(),
	};
	AnimationShader* as = new AnimationShader(asi);
	float duration = 20.0;

	std::vector<GLfloat> startPositions(numVertices * 3);
	std::vector<GLfloat> endPositions(numVertices * 3);
	std::vector<GLfloat> startTimes(numVertices);
	std::vector<GLfloat> sizes(numVertices);
	std::vector<GLfloat> colors(numVertices * 3);

	float spread = 25.;

	for (int i = 0; i < numVertices; i++) {
		float rx = ((double)rand() / (RAND_MAX)) - 0.5;
		float ry = ((double)rand() / (RAND_MAX)) - 0.5;
		float rz = ((double)rand() / (RAND_MAX)) - 0.5;

		endPositions[(i * 3)] = (float)rx * spread;
		endPositions[(i * 3) + 1] = (float)70;
		endPositions[(i * 3) + 2] = (float)rz * spread;

		startPositions[(i * 3)] = (float)0;
		startPositions[(i * 3) + 1] = (float)0;
		startPositions[(i * 3) + 2] = (float)0;

		colors[(i * 3)] = ((i + 500) % 255) / 255.;
		colors[(i * 3) + 1] = (float)(i % 255) / 255.;
		colors[(i * 3) + 2] = (float)((i + 88) % 255) / 255.;

		sizes[i] = (rx * 5.0 + 1.0);
		startTimes[i] = (i / (float)numVertices) * duration;
	}

	GeometryAttributeBuffers buffers;
	buffers.startPositions = startPositions;
	buffers.endPositions = endPositions;
	buffers.startTimes = startTimes;
	buffers.sizes = sizes;
	buffers.instanceCount = numVertices;
	buffers.duration = duration;
	buffers.colors = colors;

	return new Geometry{ buffers, as->getProgram(), GL_POINTS, DrawFunc::ARRAY };
}

Geometry* Demo9() {
	const int numVertices = 30000;

	AnimationShaderInput asi;
	asi.vertexSnippets = std::vector<std::string>{
		AnimationShader::Gravity(50),
		AnimationShader::NoiseMovement()
	};
	AnimationShader* as = new AnimationShader(asi);
	float duration = 20.0;

	std::vector<GLfloat> startPositions(numVertices * 3);
	std::vector<GLfloat> endPositions(numVertices * 3);
	std::vector<GLfloat> startTimes(numVertices);
	std::vector<GLfloat> sizes(numVertices);
	std::vector<GLfloat> colors(numVertices * 3);

	float spread = 25.;

	for (int i = 0; i < numVertices; i++) {
		float rx = ((double)rand() / (RAND_MAX)) - 0.5;
		float ry = ((double)rand() / (RAND_MAX)) - 0.5;
		float rz = ((double)rand() / (RAND_MAX)) - 0.5;

		endPositions[(i * 3)] = (float)rx * spread;
		endPositions[(i * 3) + 1] = (float)70;
		endPositions[(i * 3) + 2] = (float)rz * spread;

		colors[(i * 3)] = 200 + (i % 55) / 55.;
		colors[(i * 3) + 1] = (float)(i % 40) / 255.;
		colors[(i * 3) + 2] = (float)((i + 16) % 40) / 255.;

		sizes[i] = (rx * 5.0 + 1.0);
		startTimes[i] = (i / (float)numVertices) * duration;
	}

	GeometryAttributeBuffers buffers;
	buffers.startPositions = startPositions;//Geometry::RandomPositions(numVertices, spread);

	buffers.endPositions = endPositions;
	buffers.startTimes = startTimes;
	buffers.sizes = sizes;
	buffers.instanceCount = numVertices;
	buffers.duration = duration;
	buffers.colors = colors;

	return new Geometry{ buffers, as->getProgram(), GL_POINTS, DrawFunc::ARRAY };
}