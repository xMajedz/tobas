#pragma once
#include "raylib.h"
#include "raymath.h"
#include "game.h"

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

void CameraRotateZ(Vector3* camera_offset, float degrees);
void CameraRotateX(Vector3* camera_offset, float degrees);
void CameraRotateZClockwise(Vector3* camera_offset);
void CameraRotateZCClockwise(Vector3* camera_offset);
void CameraRotateXClockwise(Vector3* camera_offset);
void CameraRotateXCClockwise(Vector3* camera_offset);
void CameraZoomIn(Vector3* camera_offset);
void CameraZoomOut(Vector3* camera_offset);
void UpdatePlaycam(bool freeze, Camera3D* camera, Vector3* camera_offset, Player* selected_player);
