#include <iostream>
#include <sstream>
#include <fstream>
#include <string>
#include <vector>
#include <stdlib.h>
#include <stdio.h>
#include <time.h>

#define GLEW_STATIC
#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include "vector2.h"

#define ERR_GLFW	1
#define ERR_WINDOW	2
#define ERR_GLEW	3
#define ERR_CONTEXT 4
#define ERR_SHADER  5

#define POINT_COUNT 9000

#define ERRLOG(errstr) std::cerr << "ERR [" << __FILE__ << ":" << __LINE__ << "] " << errstr << std::endl

using namespace std;
using namespace BLAH;

static char* ReadPlaintext(const char* filename);
GLuint CreateProgram(const char* vShaderFilename, const char* fShaderFilename);

typedef float REAL_TYPE;

int main(int argc, char* argv[])
{
	GLFWwindow* hWindow;

	// INITIALIZE
	cout << "INITIALIZING SYSTEMS" << endl
	     << "--------------------" << endl;

	// -- Seed random generator
	cout << "\tSRAND ... \t";

	srand(time(NULL));

	cout << "OK" << endl;

	// -- GLFW
	cout << "\tGLFW ... \t";

	if(!glfwInit()) {
		ERRLOG("Could not initialize GLFW!");

		return ERR_GLFW;
	} else {
		cout << "OK" << endl;
	}

	// Create window and context for OpenGL.
	cout << "\tWindow ... \t";

	hWindow = glfwCreateWindow(640, 480, "GFX Playground Framework v1.4", NULL, NULL);

	if(!hWindow) {
		ERRLOG("Could not create window!");

		glfwTerminate();

		return ERR_WINDOW;
	} else {
		cout << "OK" << endl;
	}

	// Focus this window's context.
	cout << "\tGL Context ... \t";

	glfwMakeContextCurrent(hWindow);

	if(!glfwGetCurrentContext()) {
		ERRLOG("Could not get context!");

		glfwTerminate();

		return ERR_CONTEXT;
	} else {
		cout << "OK [v " << (glGetString(GL_VERSION) != NULL ? (const char*)glGetString(GL_VERSION) : "NULL") << "]" << endl;
	}

	// -- GLEW
	cout << "\tGLEW ... \t";

	GLenum* glewErr = (GLenum*)malloc(sizeof(GLenum)); // Heap alloc for error, because I am a stinge.

	*glewErr = glewInit();

	if(*glewErr != GLEW_OK) {
		ERRLOG(glewGetErrorString(*glewErr));

		free(glewErr); // Free error memory.
		glfwTerminate();

		return ERR_GLEW;
	} else {
		cout << "OK [v " << glewGetString(GLEW_VERSION) << "]" << endl;
	}

	free(glewErr); // Free error memory.

	// FINISH INITIALIZATION
	cout << "DONE" << endl
	     << "RUNNING ... " << endl;

	// !!! APPLICATION !!!
	vector2<REAL_TYPE> points[POINT_COUNT];

	// Triangle verts
	vector2<REAL_TYPE> vertices[3] = {
		vector2<REAL_TYPE>(-1.0, -1.0),
		vector2<REAL_TYPE>( 0.0,  1.0),
		vector2<REAL_TYPE>( 1.0, -1.0)
	};

	// Select an arbitrary initial point inside of triangle.
	points[0] = vector2<REAL_TYPE>( 0.25, 0.5);

	// Computer and store COUNT-1 new points
	for(int point = 1; point < POINT_COUNT; ++point) {
		int vertex = rand() % 3;

		// Compute new point, halfway between selected vert and the last point.
		points[point] = (points[point - 1] + vertices[vertex])/2.0f;
	}

	// Create a vertex array object.
	GLuint vao;
	glGenVertexArrays(1, &vao);
	glBindVertexArray(vao);

	// Create and initialize a buffer object.
	GLuint buffer;
	glGenBuffers(1, &buffer);
	glBindBuffer(GL_ARRAY_BUFFER, buffer);
	glBufferData(GL_ARRAY_BUFFER, sizeof(points), points, GL_STATIC_DRAW);

	// Load shaders and program.
	GLuint program = CreateProgram("default.v.glsl", "default.f.glsl");
	glUseProgram(program);

	// Init vertex position.
	GLuint location = glGetAttribLocation(program, "vPosition");
	glEnableVertexAttribArray(location);
	glVertexAttribPointer(location, 2, GL_FLOAT, GL_FALSE, 0, 0);

	// Set clear color
	glClearColor(1.0, 1.0, 1.0, 1.0);

	// Enter main program loop!
	while(!glfwWindowShouldClose(hWindow)) {
		// Clear context
		glClear(GL_COLOR_BUFFER_BIT);

		// Application GL Commands
		glDrawArrays(GL_POINTS, 0, POINT_COUNT);
		glFlush();

		// Swap buffers
		glfwSwapBuffers(hWindow);

		// Poll events
		glfwPollEvents();
	}

	// DEINITIALIZE
	cout << "SHUTTING DOWN ... ";

	// -- GLFW
	glfwTerminate();

	cout << "DONE. Goodbye." << endl;
	return 0;
}

static char* ReadPlaintext(const char* filename)
{
	ifstream file;

	file.open(filename);

	string buffer((istreambuf_iterator<char>(file)), (istreambuf_iterator<char>()));

	file.close();

	return const_cast<char*>(buffer.c_str());
}

GLuint CreateProgram(const char* vShaderFilename, const char* fShaderFilename)
{
	struct shader {
		const char* filename;
		GLenum      type;
		GLchar*     source;
	} shaders[2] = {
		{vShaderFilename, GL_VERTEX_SHADER, NULL},
		{fShaderFilename, GL_FRAGMENT_SHADER, NULL}
	};

	GLuint program = glCreateProgram();

	for(int i = 0; i < 2; ++i) {
		shader& currShader = shaders[i];
		currShader.source = ReadPlaintext(currShader.filename);

		if(currShader.source == NULL) {
			stringstream buffer;

			buffer << (currShader.type == GL_VERTEX_SHADER ? "VERT" : "FRAG")
			       << " shader could not be read! ["
			       << currShader.filename << "]";

			ERRLOG(buffer.str());

			glfwTerminate();
			exit(ERR_SHADER);
		}

		GLuint tmpShader = glCreateShader(currShader.type);
		glShaderSource(tmpShader, 1, (const GLchar**)&currShader.source, NULL);
		glCompileShader(tmpShader);

		GLint compiled;
		glGetShaderiv(tmpShader, GL_COMPILE_STATUS, &compiled);

		if(!compiled) {
			stringstream buffer;

			buffer << (currShader.type == GL_VERTEX_SHADER ? "VERT" : "FRAG")
			       << " shader failed to compile! ["
			       << currShader.filename << "]" << endl;

			GLint logSize;
			glGetShaderiv(tmpShader, GL_INFO_LOG_LENGTH, &logSize);

			char* logMessage = new char[logSize];
			glGetShaderInfoLog(tmpShader, logSize, NULL, logMessage);

			buffer << logMessage;

			delete[] logMessage;

			ERRLOG(buffer.str());

			glfwTerminate();
			exit(ERR_SHADER);
		}

		delete[] currShader.source;

		glAttachShader(program, tmpShader);
	}

	// Link the program and check for errors.
	glLinkProgram(program);

	GLint linked;
	glGetProgramiv(program, GL_LINK_STATUS, &linked);

	if(!linked) {
		stringstream buffer;

		buffer << "Program failed to link!" << endl;

		GLint logSize;
		glGetProgramiv(program, GL_INFO_LOG_LENGTH, &logSize);

		char* logMessage = new char[logSize];
		glGetProgramInfoLog(program, logSize, NULL, logMessage);

		buffer << logMessage;

		ERRLOG(buffer.str());

		delete[] logMessage;

		glfwTerminate();
		exit(ERR_SHADER);
	}

	return program;
}