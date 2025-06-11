#ifndef API_RAYMATH_H
#define API_RAYMATH_H

int RAYMATH_MatrixIdentity(lua_State* L);

int RAYMATH_MatrixMultiply(lua_State* L);

int RAYMATH_MatrixRotate(lua_State* L);

int RAYMATH_MatrixRotateX(lua_State* L);

int RAYMATH_MatrixRotateY(lua_State* L);

int RAYMATH_MatrixRotateZ(lua_State* L);

int RAYMATH_MatrixRotateXYZ(lua_State* L);

int RAYMATH_QuaternionFromMatrix(lua_State* L);

int RAYMATH_QuaternionIdentity(lua_State* L);

int RAYMATH_QuaternionMultiply(lua_State* L);

int RAYMATH_Vector3RotateByQuaternion(lua_State* L);

int luaopen_api_raymath(lua_State* L);

static const luaL_Reg api_raymath[] {
	{"MatrixIdentity", RAYMATH_MatrixIdentity},
	{"MatrixMultiply", RAYMATH_MatrixMultiply},
	{"MatrixRotate", RAYMATH_MatrixRotate},
	{"MatrixRotateX", RAYMATH_MatrixRotateX},
	{"MatrixRotateY", RAYMATH_MatrixRotateY},
	{"MatrixRotateZ", RAYMATH_MatrixRotateZ},
	{"MatrixRotateXYZ", RAYMATH_MatrixRotateXYZ},
	{"QuaternionFromMatrix", RAYMATH_QuaternionFromMatrix},
	{"QuaternionIdentity", RAYMATH_QuaternionIdentity},
	{"QuaternionMultiply", RAYMATH_QuaternionMultiply},
	{"Vector3RotateByQuaternion", RAYMATH_Vector3RotateByQuaternion},
	{NULL, NULL},
};
#endif
