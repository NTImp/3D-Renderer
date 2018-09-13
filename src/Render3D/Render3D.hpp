#pragma once
#include <vector>

#include "Graphics.hpp"
#include "Math/Matrix.hpp"

namespace Render3D {
	struct Vertex {
		Math::Vector3 position;
		Math::Vector3 normal;
	};

	struct Triangle {
		Vertex t[3];
	};

	struct Transform {
		Math::Vector3 position, rotation, scale;
	};

	void Init(Graphics::Screen* scr);
	void Finish();

	void SetProjection(float w, float h, float fov);
	void SetView(Math::Vector3& position, Math::Vector3& rotation);

	void Draw(std::vector<Triangle>& m_model, Transform& tr);

	void clearDepthBuffer();
};
