#include <stdio.h>

#include "Math/Math.hpp"
#include "Render3D/Render3D.hpp"
#include "ModelLoader.hpp"

int main()
{
	Graphics::Screen* screen = Graphics::Init(640, 480, 2);
	Render3D::Init(screen);

	std::vector<Render3D::Triangle> model;

	LoadModel("teapot.obj", model);
	
	Render3D::Transform mt;

	mt.position = Math::Vector3(0, 0, 0);
	mt.rotation = Math::Vector3(0, 0, 0);
	mt.scale = Math::Vector3(1, -1, 1);
	Render3D::SetProjection(screen->w, screen->h, 90);

	Graphics::Pixel black;
	black.r = black.g = black.b = black.a = 0;

	Math::Vector3 cpos, crot;
	cpos.x = 0;
	cpos.y = -2;
	cpos.z = -7;
	crot.x = 0;
	crot.y = 0;
	crot.z = 0;

	Math::Vector3 ld;
	ld.x = 1;
	ld.y = 1;
	ld.z = 1;

	Render3D::SetLightDirection(ld);

	while (Graphics::isActive()) {
		float delta = Graphics::GetDelta();
		mt.rotation.y += delta * 180;
		if (mt.rotation.y > 360) mt.rotation.y = 0;

		Render3D::SetView(cpos, crot);
	
		for(int i = 0; i < screen->h; i++)
			for(int e = 0; e < screen->w; e++)
				screen->data[i * screen->w + e] = black;
		Render3D::clearDepthBuffer();
		Render3D::Draw(model, mt);

		Graphics::UpdateScreen();
	};

	Render3D::Finish();
	Graphics::Quit();

	return 0;
}
