#pragma once
#include <vector>

#include "Structs.hpp"
#include "Graphics.hpp"
#include "Math/Matrix.hpp"


class Renderer {
public:
	Renderer(int w, int h, Pixel* screen);
	~Renderer();

	void setLightDirection(Math::Vector3& light);
	void setProjection(float w, float h, float fov);
	void setModelMatrix(Transform& tr);
	void setView(Math::Vector3& position, Math::Vector3& rotation);
	void setView(Math::Vector3& position, Math::Vector3& forward, Math::Vector3& up);

	void draw(std::vector<Triangle>& m_model, Transform& tr, Image* img, CullType c);

	void clearDepthBuffer();

private:
	void renderTriangles(std::vector<Triangle>& model);

	static constexpr int maxTriangles = 50 * 1024 * 1024 / sizeof(ProjOutTriangle);
	static constexpr float cz = 0.1f;

	Math::Matrix4 modelMatrix, viewMatrix, projMatrix, pvMatrix, vmMatrix, pvmMatrix;
	Math::Vector4 lightDir;

	float* depthBuffer;
	Pixel* screen;
	int w, h;
	CullType fcull;
	Image* texture;

	ProjOutTriangle *projOut;
	ProjOutTriangle *clipOut;
};
