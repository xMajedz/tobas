#ifndef CAMERA_H
#define CAMEAR_H
#include <raylib.h>
#include <raymath.h>

struct Gamecam {
	Camera3D camera;
	Vector3 CameraZoom;
	Vector3 CameraOffset;
	
	void CameraRotateZClockwise();
	void CameraRotateZCClockwise();
	void CameraRotateXClockwise();
	void CameraRotateXCClockwise();
	void CameraZoomIn();
	void CameraZoomOut();
	void UpdatePlaycam();
};
#endif
