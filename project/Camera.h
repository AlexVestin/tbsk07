#pragma once
#include "VectorUtils3.h"
#include "MicroGlut.h"
#include <iostream>

class Camera {
public:

	static void onMouseMove(int x, int y);
	static void onClick(int, int, int x, int y);
	static void handleKeyPress();
	static void setDirection(int x, int y);
	static void Init();

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