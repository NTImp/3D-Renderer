#include <stdio.h>

#include "Math/Math.hpp"
#include "Renderer/Renderer.hpp"

int main()
{
	Graphics::Screen* screen = Graphics::Init(640, 480, 2);
	Renderer::Init(screen);

	Renderer::Triangle t;
	t.t[0].position = Math::Vector3(0, 2, 0);
	t.t[1].position = Math::Vector3(2, -2, 0);
	t.t[2].position = Math::Vector3(-2, -2, 0);

	std::vector<Renderer::Triangle> model;
	model.push_back(t);

	Renderer::Transform mt;
	mt.position = Math::Vector3(0, 0, 5);
	mt.rotation.x = 0;
	mt.rotation.y = 0;
	mt.rotation.z = 0;

	Renderer::Transform cam;
	cam.position = Math::Vector3(0, 0, 0);
	cam.rotation = Math::Vector3(0, 0, 0);
	
	Renderer::SetProjection(screen->w, screen->h, 90);

	Graphics::Pixel black;
	black.r = black.g = black.b = black.a = 0;

	while (Graphics::isActive()) {
		mt.rotation.y += 0.6;
		mt.rotation.x += 0.6;
		mt.rotation.z += 0.6;

		if (Graphics::KeyPressed(SDL_SCANCODE_W)) {
			cam.position.z -= Math::Cos(cam.rotation.y) * 0.2;
			cam.position.x -= -Math::Sin(cam.rotation.y) * 0.2;
		}
	
		if (Graphics::KeyPressed(SDL_SCANCODE_S)) {
			cam.position.z += Math::Cos(cam.rotation.y) * 0.2;
			cam.position.x += -Math::Sin(cam.rotation.y) * 0.2;
		}

		if (Graphics::KeyPressed(SDL_SCANCODE_A)) {
			cam.rotation.y -= 0.6;
		}
	
		if (Graphics::KeyPressed(SDL_SCANCODE_D)) {
			cam.rotation.y += 0.6;
		}


		Renderer::SetView(cam.position, cam.rotation);
	
		for(int i = 0; i < screen->h; i++)
			for(int e = 0; e < screen->w; e++)
				screen->data[i * screen->w + e] = black;

		Renderer::clearDepthBuffer();
		Renderer::Draw(model, mt);

		Graphics::UpdateScreen();
	};

	Renderer::Finish();
	Graphics::Quit();

	return 0;
}
