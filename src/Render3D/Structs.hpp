#pragma once

#include "Math/Matrix.hpp"

struct Vertex {
	Math::Vector3 position;
	Math::Vector3 normal;
	Math::Vector2 texture;
};

struct Triangle {
	Vertex t[3];
};

struct Transform {
	Math::Vector3 position, rotation, scale;
};

enum class CullType {
	none,
	back,
	front
};

struct ProjOutVertex {
	Math::Vector4 position;
	float light;
	Math::Vector2 texture;
};

struct ProjOutTriangle {
	ProjOutTriangle() = default;
	ProjOutTriangle(Triangle& t);
	ProjOutVertex t[3];
};
