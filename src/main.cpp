#include <stdio.h>

#include "Math/Math.hpp"
#include "Render3D/Render3D.hpp"
#include "ModelLoader.hpp"

Math::Vector3 cpos, crot, ld;

void UpdateCamera(float dt)
{
	constexpr float uS = 8;
	constexpr float mS = 8;
	constexpr float aS = 45;
	if (Graphics::KeyPressed(SDL_SCANCODE_W))
	{
		cpos.x += -Math::Sin(crot.y) * dt * mS;
		cpos.z += Math::Cos(crot.y) * dt * mS;
	}

	if (Graphics::KeyPressed(SDL_SCANCODE_S))
	{
		cpos.x += Math::Sin(crot.y) * dt * mS;
		cpos.z += -Math::Cos(crot.y) * dt * mS;
	}

	if (Graphics::KeyPressed(SDL_SCANCODE_UP)) cpos.y += dt * uS;
	if (Graphics::KeyPressed(SDL_SCANCODE_DOWN)) cpos.y -= dt * uS;

	if (Graphics::KeyPressed(SDL_SCANCODE_A)) crot.y += dt * aS;
	if (Graphics::KeyPressed(SDL_SCANCODE_D)) crot.y -= dt * aS;
	if (crot.y < 0) crot.y = 360;
	if (crot.y > 360) crot.y = 0;

	ld.x = -Math::Sin(crot.y);
	ld.z = Math::Cos(crot.y);
	ld.y = -1;
}

int main()
{
	Graphics::Screen* screen = Graphics::Init(640, 480, 1);
	Render3D::Init(screen);

	Graphics::Image* txt = Graphics::LoadImage("test_models/txt.jpg");

	std::vector<Render3D::Triangle> model;

	LoadModel("teapot.obj", model, false, true);
	//LoadModel("test_models/spyro.obj", model);
	
	Render3D::Transform mt;

	mt.position = Math::Vector3(0, 0, 0);
	mt.rotation = Math::Vector3(0, 0, 0);
	mt.scale = Math::Vector3(1, 1, 1);
	Render3D::SetProjection(screen->w, screen->h, 90);

	Graphics::Pixel black;
	black.r = black.g = black.b = black.a = 0;

	cpos.x = 0;
	cpos.y = 2;
	cpos.z = -7;
	crot.x = 0;
	crot.y = 0;
	crot.z = 0;

	while (Graphics::isActive()) {
		float delta = Graphics::GetDelta();

		UpdateCamera(delta);

		//mt.rotation.y += delta * 45;
		if (mt.rotation.y > 360) mt.rotation.y = 0;

		Render3D::SetLightDirection(ld);
		Render3D::SetView(cpos, crot);
	
		for(int i = 0; i < screen->h; i++)
			for(int e = 0; e < screen->w; e++)
				screen->data[i * screen->w + e] = black;
		Render3D::clearDepthBuffer();
		Render3D::Draw(model, mt, txt);

		Graphics::UpdateScreen();
	};

	Graphics::FreeImage(txt);

	Render3D::Finish();
	Graphics::Quit();

	return 0;
}
