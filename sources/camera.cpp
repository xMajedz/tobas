#include <camera.h>

void Gamecam::CameraRotateZClockwise()
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
}
