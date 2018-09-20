#pragma once
#include <vector>

#include "Graphics.hpp"
#include "Math/Matrix.hpp"

namespace Render3D {
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

	void Init(Graphics::Screen* scr);
	void Finish();

	void SetLightDirection(Math::Vector3& light);
	void SetProjection(float w, float h, float fov);
	void SetView(Math::Vector3& position, Math::Vector3& rotation);
	void SetView(Math::Vector3& position, Math::Vector3& forward, Math::Vector3& up);

	void Draw(std::vector<Triangle>& m_model, Transform& tr);

	void clearDepthBuffer();
};
