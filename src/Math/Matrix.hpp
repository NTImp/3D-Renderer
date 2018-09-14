#pragma once
#include "Math.hpp"

namespace Math {
	struct Vector2 {
		Vector2() = default;
		Vector2(float _x, float _y);
		float x, y;
	};

	struct Vector3 {
		Vector3() = default;
		Vector3(float _x, float _y, float _z);
		float x, y, z;
	};
	struct Vector4 {
		Vector4() = default;
		Vector4(float _x, float _y, float _z, float _w);
		Vector4(Vector3& v, float _w);
		float x, y, z, w;
	};

	struct Matrix4 {
		Matrix4() = default;
		Matrix4(float d);
		float m[4][4];
	};

	inline float mod(Vector2 a) {
		return Sqrt(a.x*a.x + a.y * a.y);
	}

	inline float mod(Vector3 a) {
		return Sqrt(a.x*a.x + a.y * a.y + a.z * a.z);
	}

	inline float mod(Vector4 a) {
		return Sqrt(a.x*a.x + a.y * a.y + a.z * a.z + a.w * a.w);
	}

	inline float dot(Vector2 a, Vector2 b) {
		return a.x * b.x + a.y * b.y;
	};

	inline float cross(Vector2 a, Vector2 b) {
		return a.x * b.y - a.y * b.x;
	};

	inline float dot(Vector3 a, Vector3 b) {
		return a.x * b.x + a.y * b.y + a.z* b.z;
	};

	inline Vector3 cross(Vector3 a, Vector3 b) {
		Vector3 ret;
		ret.x = a.y * b.z - a.z * b.y;
		ret.y = a.z * b.x - a.x * b.z;
		ret.z = a.x * b.y - a.y * b.x;

		return ret;
	};

	inline float dot(Vector4 a, Vector4 b){
		return a.x * b.x + a.y * b.y + a.z* b.z + a.w * b.w;
	};
	
	Matrix4 mul(Matrix4 a, Matrix4 b);
	Vector4 mul(Matrix4 a, Vector4 b);


	Matrix4 RotateX(float degrees);
	Matrix4 RotateY(float degrees);
	Matrix4 RotateZ(float degrees);
	Matrix4 Translate(Vector3 translation, float scale);
	Matrix4 Scale(Vector3 vscale, float scale);
	Matrix4 LookAt(Vector3 f, Vector3 up);
};
