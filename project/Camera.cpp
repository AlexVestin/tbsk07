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

	if (glutKeyIsDown(GLUT_KEY_SPACE)) {
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
		//pos.y += mv * left.y;
		pos.z += mv * left.z;
	}

	if (glutKeyIsDown('d')) {
		vec3 right;
		right.x = cosf(toRad(yaw + 90)) * cosf(toRad(pitch));
		right.y = sinf(toRad(pitch));
		right.z = sinf(toRad(yaw + 90)) * cosf(toRad(pitch));
		pos.x += mv * right.x;

		//pos.y += mv * right.y;
		pos.z += mv * right.z;
	}

	if (glutKeyIsDown('w')) {
		pos.x += mv * lookAtPoint.x;
		pos.y += mv * lookAtPoint.y;
		pos.z += mv * lookAtPoint.z;
	}	
}