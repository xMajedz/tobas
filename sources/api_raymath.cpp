#include <api_raymath.h>

int RAYMATH_MatrixIdentity(lua_State* L)
{
	Matrix m = MatrixIdentity();
	lua_newtable(L);
	lua_pushnumber(L, m.m0);
	lua_rawseti(L, -1, 1);
	lua_pushnumber(L, m.m1);
	lua_rawseti(L, -2, 2);
	lua_pushnumber(L, m.m2);
	lua_rawseti(L, -3, 3);
	lua_pushnumber(L, m.m3);
	lua_rawseti(L, -4, 4);

	lua_pushnumber(L, m.m4);
	lua_rawseti(L, -5, 5);
	lua_pushnumber(L, m.m5);
	lua_rawseti(L, -6, 6);
	lua_pushnumber(L, m.m6);
	lua_rawseti(L, -7, 7);
	lua_pushnumber(L, m.m7);
	lua_rawseti(L, -8, 8);

	lua_pushnumber(L, m.m8);
	lua_rawseti(L, -9, 9);
	lua_pushnumber(L, m.m9);
	lua_rawseti(L, -10, 10);
	lua_pushnumber(L, m.m10);
	lua_rawseti(L, -11, 11);
	lua_pushnumber(L, m.m11);
	lua_rawseti(L, -12, 12);

	lua_pushnumber(L, m.m12);
	lua_rawseti(L, -13, 13);
	lua_pushnumber(L, m.m13);
	lua_rawseti(L, -14, 14);
	lua_pushnumber(L, m.m14);
	lua_rawseti(L, -15, 15);
	lua_pushnumber(L, m.m15);
	lua_rawseti(L, -16, 16);

	return 1;
}

int RAYMATH_MatrixMultiply(lua_State* L)
{
	if (lua_istable(L, -1)) {
		Matrix left;
		lua_rawgeti(L, -1, 1);
		left.m0 = lua_tonumber(L, -1);
		lua_rawgeti(L, -2, 2);
		left.m1 = lua_tonumber(L, -1);
		lua_rawgeti(L, -3, 3);
		left.m2 = lua_tonumber(L, -1);
		lua_rawgeti(L, -4, 4);
		left.m3 = lua_tonumber(L, -1);
		lua_rawgeti(L, -5, 5);
		left.m4 = lua_tonumber(L, -1);
		lua_rawgeti(L, -6, 6);
		left.m5 = lua_tonumber(L, -1);
		lua_rawgeti(L, -7, 7);
		left.m6 = lua_tonumber(L, -1);
		lua_rawgeti(L, -8, 8);
		left.m7 = lua_tonumber(L, -1);
		lua_rawgeti(L, -9, 9);
		left.m8 = lua_tonumber(L, -1);
		lua_rawgeti(L, -10, 10);
		left.m9 = lua_tonumber(L, -1);
		lua_rawgeti(L, -11, 11);
		left.m10 = lua_tonumber(L, -1);
		lua_rawgeti(L, -12, 12);
		left.m11 = lua_tonumber(L, -1);
		lua_rawgeti(L, -13, 13);
		left.m12 = lua_tonumber(L, -1);
		lua_rawgeti(L, -14, 14);
		left.m13 = lua_tonumber(L, -1);
		lua_rawgeti(L, -15, 15);
		left.m14 = lua_tonumber(L, -1);
		lua_rawgeti(L, -16, 16);
		left.m15 = lua_tonumber(L, -1);

		Matrix right;
		if (lua_istable(L, -17)) {
			lua_rawgeti(L, -1, 1);
			right.m0 = lua_tonumber(L, -1);
			lua_rawgeti(L, -2, 2);
			right.m1 = lua_tonumber(L, -1);
			lua_rawgeti(L, -3, 3);
			right.m2 = lua_tonumber(L, -1);
			lua_rawgeti(L, -4, 4);
			right.m3 = lua_tonumber(L, -1);
			lua_rawgeti(L, -5, 5);
			right.m4 = lua_tonumber(L, -1);
			lua_rawgeti(L, -6, 6);
			right.m5 = lua_tonumber(L, -1);
			lua_rawgeti(L, -7, 7);
			right.m6 = lua_tonumber(L, -1);
			lua_rawgeti(L, -8, 8);
			right.m7 = lua_tonumber(L, -1);
			lua_rawgeti(L, -9, 9);
			right.m8 = lua_tonumber(L, -1);
			lua_rawgeti(L, -10, 10);
			right.m9 = lua_tonumber(L, -1);
			lua_rawgeti(L, -11, 11);
			right.m10 = lua_tonumber(L, -1);
			lua_rawgeti(L, -12, 12);
			right.m11 = lua_tonumber(L, -1);
			lua_rawgeti(L, -13, 13);
			right.m12 = lua_tonumber(L, -1);
			lua_rawgeti(L, -14, 14);
			right.m13 = lua_tonumber(L, -1);
			lua_rawgeti(L, -15, 15);
			right.m14 = lua_tonumber(L, -1);
			lua_rawgeti(L, -16, 16);
			right.m15 = lua_tonumber(L, -1);
		}
		Matrix result = MatrixMultiply(left, right);
		//return resutl
	}
	return 1;
}

int RAYMATH_MatrixRotate(lua_State* L)
{
	if (lua_istable(L, -1)) {
		Vector3 axis;
		lua_rawgeti(L, -1, 1);
		axis.x = lua_tonumber(L, -1);
		lua_rawgeti(L, -2, 2);
		axis.y = lua_tonumber(L, -1);
		lua_rawgeti(L, -3, 3);
		axis.z = lua_tonumber(L, -1);

		float angel;
		if (lua_isnumber(L, -4)) {
			angel = lua_tonumber(L, -1);
		}
		MatrixRotate(axis, angel);
	}
	return 1;
}

int RAYMATH_MatrixRotateX(lua_State* L)
{
	float angel;
	MatrixRotateX(angel);
	return 1;
}

int RAYMATH_MatrixRotateY(lua_State* L)
{
	float angel;
	MatrixRotateY(angel);
	return 1;
}

int RAYMATH_MatrixRotateZ(lua_State* L)
{
	float angel;
	MatrixRotateZ(angel);
	return 1;
}

int RAYMATH_MatrixRotateXYZ(lua_State* L)
{
	Vector3 angel;
	MatrixRotateXYZ(angel);
	return 1;
}

int RAYMATH_QuaternionFromMatrix(lua_State* L)
{
	Matrix m;
	QuaternionFromMatrix(m);
	return 1;
}

int RAYMATH_QuaternionIdentity(lua_State* L)
{
	Quaternion q = QuaternionIdentity();
	return 1;
}

int RAYMATH_QuaternionMultiply(lua_State* L)
{
	Quaternion q1;
	Quaternion q2;
	QuaternionMultiply(q1, q2);
	return 1;
}

int RAYMATH_Vector3RotateByQuaternion(lua_State* L)
{
	Vector3 v;
	Quaternion q;
	Vector3RotateByQuaternion(v, q);
	return 1;
}

int luaopen_api_raymath(lua_State* L)
{
	luaL_register(L, "RAYMATH", api_raymath);
	return 1;
}
