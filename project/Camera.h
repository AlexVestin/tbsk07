#pragma once
#include "VectorUtils3.h"
#include "MicroGlut.h"
#include <iostream>

#define near 1.0
#define far 300.0
#define right 0.5
#define left -0.5
#define top 0.5
#define bottom -0.5

class Camera {
public:

	static void onMouseMove(int x, int y);
	static void onClick(int, int, int x, int y);
	static void handleKeyPress();
	static void setDirection(int x, int y);
	static constexpr GLfloat projectionMatrix[16] = {
		2.0f * near / (right - left), 0.0f, (right + left) / (right - left), 0.0f,
		0.0f, 2.0f * near / (top - bottom), (top + bottom) / (top - bottom), 0.0f,
		0.0f, 0.0f, -(far + near) / (far - near), -2 * far * near / (far - near),
		0.0f, 0.0f, -1.0f, 0.0f 
	};
	

	static mat4 getMatrix();

	static float toRad(float deg) {
		return deg * (M_PI / 180.0);
	}

	static bool leftMouseIsDown;
	static float pitch;
	static float yaw;

	static int lastX;
	static int lastY;

	static vec3 pos;
	static vec3 lookAtPoint;
	static vec3 upVector;

private:
	// Disallow creating an instance of this object
	Camera() {}
};