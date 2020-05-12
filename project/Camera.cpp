#include "Camera.h"

vec3 Camera::upVector = SetVector(0, 1, 0);
vec3 Camera::pos = SetVector(-4, 15, 20);
vec3 Camera::lookAtPoint = SetVector(0, 0, 0);

bool Camera::leftMouseIsDown = false;
int Camera::lastX = 0;
int Camera::lastY = 0;

float Camera::pitch = -30;
float Camera::yaw = -90.0;






mat4 Camera::getMatrix() {
	return lookAt(
		pos.x, pos.y, pos.z, 
		pos.x + lookAtPoint.x, pos.y + lookAtPoint.y, pos.z + lookAtPoint.z, 
		upVector.x, upVector.y, upVector.z
	);
}

void Camera::setDirection(int x, int y) {
	const float sens = 0.15f;
	yaw += (x - lastX) * sens;
	yaw = fmod(yaw, 360.0f);
	pitch += (lastY - y) * sens;

	if (pitch > 89.0f)
		pitch = 89.0f;
	if (pitch < -89.0f)
		pitch = -89.0f;

	vec3 cameraFront;
	cameraFront.x = cosf(toRad(yaw)) * cosf(toRad(pitch));
	cameraFront.y = sinf(toRad(pitch));
	cameraFront.z = sinf(toRad(yaw)) * cosf(toRad(pitch));
	lookAtPoint = Normalize(cameraFront);
}

void Camera::onMouseMove(int x, int y) {
	if (leftMouseIsDown) {
		setDirection(x, y);
	}

	lastX = x;
	lastY = y;
};


void Camera::onClick(int button, int state, int x, int y) {
	if (button == 0) {
		lastX = x;
		lastY = y;
		leftMouseIsDown = !state;
	}
}
void Camera::handleKeyPress() {
	const float mv = 0.25;

	if (glutKeyIsDown('s')) {
		pos.x -= mv * lookAtPoint.x;
		pos.y -= mv * lookAtPoint.y;
		pos.z -= mv * lookAtPoint.z;
	}

	if (glutKeyIsDown(GLUT_KEY_SPACE) || glutKeyIsDown('i')) {
		pos.y += mv;
	}

	if (glutKeyIsDown(GLUT_KEY_LEFT_SHIFT) || glutKeyIsDown('o')) {
		pos.y -= mv;
	}

	if (glutKeyIsDown('a')) {
		vec3 leftVec;
		leftVec.x = cosf(toRad(yaw - 90)) * cosf(toRad(pitch));
		leftVec.y = sinf(toRad(pitch));
		leftVec.z = sinf(toRad(yaw - 90)) * cosf(toRad(pitch));
		pos.x += mv * leftVec.x;
		//pos.y += mv * left.y;
		pos.z += mv * leftVec.z;
	}

	if (glutKeyIsDown('d')) {
		vec3 rightVec;
		rightVec.x = cosf(toRad(yaw + 90)) * cosf(toRad(pitch));
		//rightVec.y = sinf(toRad(pitch));
		rightVec.z = sinf(toRad(yaw + 90)) * cosf(toRad(pitch));
		pos.x += mv * rightVec.x;
		//pos.y += mv * right.y;
		pos.z += mv * rightVec.z;
	}

	if (glutKeyIsDown('w')) {
		pos.x += mv * lookAtPoint.x;
		pos.y += mv * lookAtPoint.y;
		pos.z += mv * lookAtPoint.z;
	}	

	// If either q or e is pressed, we need to change our lookAtPoint.
	if (glutKeyIsDown('q') || glutKeyIsDown('e')) {
		if (glutKeyIsDown('q'))
			yaw -= mv / 2;
		
		if (glutKeyIsDown('e'))
			yaw += mv / 2;
	
		yaw = fmod(yaw, 360.0f);
		
		vec3 cameraFront;
		cameraFront.x = cosf(toRad(yaw)) * cosf(toRad(pitch));
		cameraFront.y = sinf(toRad(pitch));
		cameraFront.z = sinf(toRad(yaw)) * cosf(toRad(pitch));
		lookAtPoint   = Normalize(cameraFront);
	}

}