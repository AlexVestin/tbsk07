#include "AnimationShader.h"

#define FLAT_SHADING 0
#define LAMBERT_SHADING 1
#define PHONG_SHADING 2

// The default shading mode
GLint	DEFAULT_SHADING = PHONG_SHADING;
// The default values of our light sources.
GLuint  DEFAULT_LIGHT_NO			 = 2;
vec3	DEFAULT_COLORS[]			 = { { 1.0f, 1.0f, 1.0f }, { 1.0f, 0.0f, 0.0f } };
GLint	DEFAULT_DIRECTIONAL[]		 = { 1, 1 };
vec3	DEFAULT_DIRECTION_POSITION[] = { { 1.0f, 1.0f, 0.0f }, { 0.0f,-1.0f, 1.0f } };


// The default constructor.
AnimationShader::AnimationShader() {
	shadingMode						= DEFAULT_SHADING;
	lightNo							= DEFAULT_LIGHT_NO;
	lightSourcesColorsArr			= DEFAULT_COLORS;
	isDirectional					= DEFAULT_DIRECTIONAL;
	lightSourcesDirectionsPositions = DEFAULT_DIRECTION_POSITION;
	load();
}

// The constructor for when you want controll of the light.
AnimationShader::AnimationShader(GLuint lightNo, vec3 lightSourcesColorsArr[], GLint isDirectional[], vec3 lightSourcesDirectionsPositions[]) {
	shadingMode							  = DEFAULT_SHADING;
	this->lightNo						  = lightNo;
	this->lightSourcesColorsArr			  = lightSourcesColorsArr;
	this->isDirectional					  = isDirectional;
	this->lightSourcesDirectionsPositions = lightSourcesDirectionsPositions;
	load();
}

// These constructors are for when you want to customize the shaders.
AnimationShader::AnimationShader(std::string customCodeSnippet) : AnimationShader() {
	AnimationShaderInput asi;
	asi.vertexSnippets = std::vector<std::string>{ customCodeSnippet };
	load(asi);
}

AnimationShader::AnimationShader(AnimationShaderInput asi) : AnimationShader() {
	load(asi);
}

AnimationShader::AnimationShader(GLuint lightNo, vec3 lightSourcesColorsArr[], GLint isDirectional[], vec3 lightSourcesDirectionsPositions[], std::string customCodeSnippet) :
				 AnimationShader(lightNo, lightSourcesColorsArr, isDirectional, lightSourcesColorsArr) {
}

void AnimationShader::setShadingMode(GLint shadingMode) {
	this->shadingMode = shadingMode;
	uploadValues();
}

void AnimationShader::setLightSources(GLuint lightNo, vec3 lightSourcesColorsArr[], GLint isDirectional[], vec3 lightSourcesDirectionsPositions[]) {
	this->lightNo = lightNo;
	this->lightSourcesColorsArr = lightSourcesColorsArr;
	this->isDirectional = isDirectional;
	this->lightSourcesDirectionsPositions = lightSourcesDirectionsPositions;
	uploadValues();
}


void AnimationShader::load() {
	program = loadShaders("main.vert", "main.frag");
	glUseProgram(program);
	glUniformMatrix4fv(glGetUniformLocation(program, "projMatrix"), 1, GL_TRUE, &Camera::projectionMatrix[0]);
	uploadValues();
}

std::string AnimationShader::prepareVertex(AnimationShaderInput asi) {
	std::ifstream vsis("main.vert");
	std::string vs((std::istreambuf_iterator<char>(vsis)), std::istreambuf_iterator<char>());
	std::string newvs;
	std::string breakpointSnippet = "// --- BREAKPOINT-SNIPPETS --- //";
	std::string breakpointDefines = "// --- BREAKPOINT-DEFINES --- //";
	std::string breakpointEnd = "// --- BREAKPOINT-END --- //";

	// split by newline
	auto ss = std::stringstream{ vs };

	for (std::string line; std::getline(ss, line, '\n');) {

		
		if (line.find(breakpointDefines) != std::string::npos) {
			for (auto& sline : asi.vertexDefines) {
				newvs += sline + "\n";
			}
		}

		if (line.find(breakpointSnippet) != std::string::npos) {
			for (auto& sline : asi.vertexSnippets) {
				newvs += sline + "\n";
			}
		}
		
		if (line.find(breakpointEnd) != std::string::npos) {
			for (auto& sline : asi.vertexFinals) {
				newvs += sline + "\n";
			}
		}
	
		newvs += line + "\n";
	}

	return newvs;
};


void AnimationShader::load(AnimationShaderInput asi) {
	std::string vs = prepareVertex(asi);

	std::ifstream fsis("main.frag");
	std::string fs((std::istreambuf_iterator<char>(fsis)), std::istreambuf_iterator<char>());

	GLuint v = glCreateShader(GL_VERTEX_SHADER);
	GLuint f = glCreateShader(GL_FRAGMENT_SHADER);
		
	const char* vsc = vs.c_str();
	glShaderSource(v, 1, &vsc, NULL);

	const char* fsc = fs.c_str();
	glShaderSource(f, 1, &fsc, NULL);
	glCompileShader(v);
	glCompileShader(f);


	GLint infologLength = 0;
	GLint charsWritten = 0;
	char* infoLog;

	glGetShaderiv(v, GL_INFO_LOG_LENGTH, &infologLength);
	if (infologLength > 2)
	{
		infoLog = (char*)malloc(infologLength);
		glGetShaderInfoLog(v, infologLength, &charsWritten, infoLog);
		fprintf(stderr, "%s\n", infoLog);
		free(infoLog);
	}

	glGetShaderiv(f, GL_INFO_LOG_LENGTH, &infologLength);
	if (infologLength > 2)
	{
		infoLog = (char*)malloc(infologLength);
		glGetShaderInfoLog(f, infologLength, &charsWritten, infoLog);
		fprintf(stderr, "%s\n", infoLog);
		free(infoLog);
	}

	program = glCreateProgram();
	glAttachShader(program, v);
	glAttachShader(program, f);

	glLinkProgram(program);
	glUseProgram(program);

	glUniformMatrix4fv(glGetUniformLocation(program, "projMatrix"), 1, GL_TRUE, &Camera::projectionMatrix[0]);
	
	glDeleteShader(v);
	glDeleteShader(f);

	uploadValues();

	std::cout << "Animation shader loaded" << std::endl;
}

void AnimationShader::uploadValues() {

	glUniform1i(glGetUniformLocation(program, "shadingMode"), shadingMode);
	
	GLuint loc = glGetUniformLocation(program, "lightSourcesDirPosArr");
	glUniform3fv(loc, lightNo, &lightSourcesDirectionsPositions[0].x);

	glUniform3fv(glGetUniformLocation(program, "lightSourcesColorArr"), lightNo, &lightSourcesColorsArr[0].x);

	glUniform1iv(glGetUniformLocation(program, "isDirectional"), lightNo, isDirectional);

	glUniform1i(glGetUniformLocation(program, "lightSourcesNo"), lightNo);
}


