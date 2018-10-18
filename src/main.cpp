#include <stdio.h>

#include "Math/Math.hpp"
#include "Render3D/Renderer.hpp"
#include "ModelLoader.hpp"


int main(int argc, char** argv)
{
	Math::Vector3 cpos, crot, ld;
	Window window(640, 480, 2);
	Pixel* screen = window.getScreen();

	Image txt("test_models/txt.jpg");
	Renderer renderer(640, 480, window.getScreen());

	std::vector<Triangle> model;

	//LoadModel("teapot.obj", model, false, true);
	LoadModel("test_models/spyro.obj", model, false, true);
	
	Transform mt;

	mt.position = Math::Vector3(0, 0, 0);
	mt.rotation = Math::Vector3(0, 0, 0);
	mt.scale = Math::Vector3(1, 1, 1);
	renderer.setProjection(640, 480, 90);

	Pixel black;
	black.r = black.g = black.b = black.a = 0;

	cpos.x = 0;
	cpos.y = 2;
	cpos.z = -7;
	crot.x = 0;
	crot.y = 0;
	crot.z = 0;

	while (window.isActive()) {
		float dt = window.getDelta();

		constexpr float uS = 8;
		constexpr float mS = 8;
		constexpr float aS = 45;
		if (window.keyPressed(SDL_SCANCODE_W))
		{
			cpos.x += -Math::Sin(crot.y) * dt * mS;
			cpos.z += Math::Cos(crot.y) * dt * mS;
		}

		if (window.keyPressed(SDL_SCANCODE_S))
		{
			cpos.x += Math::Sin(crot.y) * dt * mS;
			cpos.z += -Math::Cos(crot.y) * dt * mS;
		}

		if (window.keyPressed(SDL_SCANCODE_UP)) cpos.y += dt * uS;
		if (window.keyPressed(SDL_SCANCODE_DOWN)) cpos.y -= dt * uS;

		if (window.keyPressed(SDL_SCANCODE_A)) crot.y += dt * aS;
		if (window.keyPressed(SDL_SCANCODE_D)) crot.y -= dt * aS;
		if (crot.y < 0) crot.y = 360;
		if (crot.y > 360) crot.y = 0;

		ld.x = -Math::Sin(crot.y);
		ld.z = Math::Cos(crot.y);
		ld.y = -1;

		//mt.rotation.y += delta * 45;
		if (mt.rotation.y > 360) mt.rotation.y = 0;

		renderer.setLightDirection(ld);
		renderer.setView(cpos, crot);
	
		for(int i = 0; i < 480; i++)
			for(int e = 0; e < 640; e++)
				screen[i * 640 + e] = black;
		renderer.clearDepthBuffer();
		renderer.draw(model, mt, &txt, CullType::none);

		window.updateScreen();
	};

	return 0;
}
