#include "camera.h"

void Gamecam::Init()
{
	camera.up = { 0.00, 0.00, 1.00 };
	camera.fovy = 45.00;
	camera.projection = CAMERA_PERSPECTIVE;
	camera_offset = { 0.00, 5.00, 0.00 };
}

Camera Gamecam::Get()
{
	return camera;
}

void CameraRotateZ(Vector3* camera_offset, float degrees)
{
	Quaternion q = QuaternionFromMatrix(MatrixRotateZ(DEG2RAD * degrees));
	*camera_offset = Vector3RotateByQuaternion(*camera_offset, q);
}

void CameraRotateX(Vector3* camera_offset, float degrees)
{
	Quaternion q = QuaternionFromMatrix(MatrixRotateX(DEG2RAD * degrees));
	*camera_offset = Vector3RotateByQuaternion(*camera_offset, q);
}

void CameraRotateZClockwise(Vector3* camera_offset)
{
	CameraRotateZ(camera_offset, 10);
}

void CameraRotateZCClockwise(Vector3* camera_offset)
{
	CameraRotateZ(camera_offset, -10);
}

void CameraRotateXClockwise(Vector3* camera_offset)
{
	CameraRotateX(camera_offset, 10);
}

void CameraRotateXCClockwise(Vector3* camera_offset)
{
	CameraRotateX(camera_offset, -10);
}

void CameraZoomIn(Vector3* camera_offset)
{
	camera_offset->x *= 0.90;
	camera_offset->y *= 0.90;
	camera_offset->z *= 0.90;
}

void CameraZoomOut(Vector3* camera_offset)
{
	camera_offset->x *= 1.10;
	camera_offset->y *= 1.10;
	camera_offset->z *= 1.10;
}

void Gamecam::UpdateDummycam()
{
	camera.position = { 0.00, 5.00, 0.00};
	camera.target = { 0.00, 0.00, 2.00 }; 
}

void UpdatePlaycam(bool freeze, Camera3D* camera, Vector3* camera_offset, Player* selected_player)
{

	/*float x = 0.00;
	float y = 0.00;
	float z = 0.00;
	
	auto b_count = selected_player->b_count; 

	if (b_count > 0) {
		for (auto const [body_name, b] : selected_player->body) {
			if (freeze) {
				x += b.freeze.position.x;
				y += b.freeze.position.y;
				z += b.freeze.position.z;
			} else {
				const dReal *position = dBodyGetPosition(b.dBody);
				x += position[0];
				y += position[1];
				z += position[2];
			}
		}
		x = x/b_count;
		y = y/b_count;
		z = z/b_count;
	}

	camera->position.x = camera_offset->x + x;
	camera->position.y = camera_offset->y + y;
	camera->position.z = camera_offset->z + z;

	camera->target.x = x;
	camera->target.y = y;
	camera->target.z = z;

	if (IsKeyDown(KEY_LEFT_SHIFT)) {
		if (IsKeyDown(KEY_W)) {
			CameraRotateXClockwise(camera_offset);
		}

		if (IsKeyDown(KEY_A)) {
			CameraRotateZClockwise(camera_offset);
		}

		if (IsKeyDown(KEY_S)) {
			CameraRotateXCClockwise(camera_offset);
		}

		if (IsKeyDown(KEY_D)) {
			CameraRotateZCClockwise(camera_offset);
		}
	} else {
		if (IsKeyDown(KEY_W)) {
			CameraZoomIn(camera_offset);
		}

		if (IsKeyDown(KEY_A)) {
			CameraRotateZCClockwise(camera_offset);
		}

		if (IsKeyDown(KEY_S)) {
			CameraZoomOut(camera_offset);
		}

		if (IsKeyDown(KEY_D)) {
			CameraRotateZClockwise(camera_offset);
		}
	}*/
}

/*void Gamecam::CameraRotateZClockwise()
{
	Matrix m = MatrixRotateZ(DEG2RAD*10);
	Quaternion q = QuaternionFromMatrix(m);
	Quaternion iq = QuaternionInvert(q);
	Quaternion p = QuaternionMultiply(
			iq,
			(Quaternion){
				CameraOffset.x,
				CameraOffset.y,
				CameraOffset.z,
				0.0f
	});

	p = QuaternionMultiply(p, q);

	CameraOffset.x = p.x;
	CameraOffset.y = p.y;
	CameraOffset.z = p.z;

	Quaternion p2 = QuaternionMultiply(
			iq,
			(Quaternion){
				CameraZoom.x,
				CameraZoom.y,
				CameraZoom.z,
				0.0f
	});

	p2 = QuaternionMultiply(p2, q);

	CameraZoom.x = p2.x;
	CameraZoom.y = p2.y;
	CameraZoom.y = p2.z;
}

void Gamecam::CameraRotateZCClockwise()
{
	Matrix m = MatrixRotateZ(DEG2RAD*-10);
	Quaternion q = QuaternionFromMatrix(m);
	Quaternion iq = QuaternionInvert(q);
	Quaternion p = QuaternionMultiply(
			iq,
			(Quaternion){
				CameraOffset.x,
				CameraOffset.y,
				CameraOffset.z,
				0.0f
	});

	p = QuaternionMultiply(p, q);

	CameraOffset.x = p.x;
	CameraOffset.y = p.y;
	CameraOffset.z = p.z;

	Quaternion p2 = QuaternionMultiply(
			iq,
			(Quaternion){
				CameraZoom.x,
				CameraZoom.y,
				CameraZoom.z,
				0.0f
	});

	p2 = QuaternionMultiply(p2, q);

	CameraZoom.x = p2.x;
	CameraZoom.y = p2.y;
	CameraZoom.z = p2.z;
}

void Gamecam::CameraRotateXClockwise()
{
	Matrix m = MatrixRotateX(DEG2RAD*10);
	Quaternion q = QuaternionFromMatrix(m);
	Quaternion iq = QuaternionInvert(q);
	Quaternion p = QuaternionMultiply(
			iq,
			(Quaternion){
				CameraOffset.x,
				CameraOffset.y,
				CameraOffset.z,
				0.0f
	});

	p = QuaternionMultiply(p, q);

	CameraOffset.x = p.x;
	CameraOffset.y = p.y;
	CameraOffset.z = p.z;

	Quaternion p2 = QuaternionMultiply(
			iq,
			(Quaternion){
				CameraZoom.x,
				CameraZoom.y,
				CameraZoom.z,
				0.0f
	});

	p2 = QuaternionMultiply(p2, q);

	CameraZoom.x = p2.x;
	CameraZoom.y = p2.y;
	CameraZoom.y = p2.z;
}

void Gamecam::CameraRotateXCClockwise()
{
	Matrix m = MatrixRotateX(DEG2RAD*-10);
	Quaternion q = QuaternionFromMatrix(m);
	Quaternion iq = QuaternionInvert(q);
	Quaternion p = QuaternionMultiply(
			iq,
			(Quaternion){
				CameraOffset.x,
				CameraOffset.y,
				CameraOffset.z,
				0.0f
	});

	p = QuaternionMultiply(p, q);

	CameraOffset.x = p.x;
	CameraOffset.y = p.y;
	CameraOffset.z = p.z;

	Quaternion p2 = QuaternionMultiply(
			iq,
			(Quaternion){
				CameraZoom.x,
				CameraZoom.y,
				CameraZoom.z,
				0.0f
	});

	p2 = QuaternionMultiply(p2, q);

	CameraZoom.x = p2.x;
	CameraZoom.y = p2.y;
	CameraZoom.z = p2.z;
}

void Gamecam::CameraZoomIn()
{
	CameraOffset.x = 0.9f * CameraOffset.x;
	CameraOffset.y = 0.9f * CameraOffset.y;
	CameraOffset.z = 0.9f * CameraOffset.z; 
}

void Gamecam::CameraZoomOut()
{
	CameraOffset.x = 1.1f * CameraOffset.x;
	CameraOffset.y = 1.1f * CameraOffset.y;
	CameraOffset.z = 1.1f * CameraOffset.z;
}*/
