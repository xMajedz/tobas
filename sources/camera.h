#pragma once
#include "raylib.h"
#include "raymath.h"
#include "game.h"

namespace Gamecam {
	static Camera camera;
	static Vector3 camera_offset;
	
	void Init();
	const Camera& Get();

	void CameraRotateZ(float degrees);
	void CameraRotateX(float degrees);
	void CameraRotateZClockwise();
	void CameraRotateZCClockwise();
	void CameraRotateXClockwise();
	void CameraRotateXCClockwise();
	void CameraZoomIn();
	void CameraZoomOut();

	void UpdateSpectatorcam(bool freeze, std::vector<Player> players);
	void UpdatePlaycam(bool freeze, Player selected_player);
	void UpdateDummycam();
};

