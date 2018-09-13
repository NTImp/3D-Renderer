#pragma once

#include "Render3D.hpp"

namespace Render3D {
	struct ProjOutVertex {
		Math::Vector4 position;
		float light;
	};

	struct ProjOutTriangle {
		ProjOutTriangle() = default;
		ProjOutTriangle(Triangle& t);
		ProjOutVertex t[3];
	};
};
