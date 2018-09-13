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
	
	Render3D::Transform cam, mt;
	cam.position = Math::Vector3(0, 0, 0);
	cam.rotation = Math::Vector3(0, 0, 0);

	mt.position = Math::Vector3(0, 0, 10);
	mt.rotation = Math::Vector3(0, 0, 0);
	mt.scale = Math::Vector3(1, -1, 1);
	Render3D::SetProjection(screen->w, screen->h, 90);

	Graphics::Pixel black;
	black.r = black.g = black.b = black.a = 0;

	while (Graphics::isActive()) {

		if (Graphics::KeyPressed(SDL_SCANCODE_W)) {
			cam.position.z += Math::Cos(cam.rotation.y) * 0.2;
			cam.position.x += -Math::Sin(cam.rotation.y) * 0.2;
		}
	
		if (Graphics::KeyPressed(SDL_SCANCODE_S)) {
			cam.position.z -= Math::Cos(cam.rotation.y) * 0.2;
			cam.position.x -= -Math::Sin(cam.rotation.y) * 0.2;
		}

		if (Graphics::KeyPressed(SDL_SCANCODE_A)) {
			cam.rotation.y += 0.6;
		}
	
		if (Graphics::KeyPressed(SDL_SCANCODE_D)) {
			cam.rotation.y -= 0.6;
		}


		Render3D::SetView(cam.position, cam.rotation);
	
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
