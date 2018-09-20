#pragma once

#include "Render3D.hpp"

namespace Render3D {
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

	constexpr int maxTriangles = 50 * 1024 * 1024 / sizeof(ProjOutTriangle);
};
