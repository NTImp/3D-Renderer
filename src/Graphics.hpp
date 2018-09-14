#pragma once
#include <SDL2/SDL.h>

namespace Graphics {
#pragma packed(1)
	struct Pixel {
		char r, g, b, a;
	};

	struct Screen {
		int w, h, p;
		Pixel* data;
	};

	Screen* Init(int w, int h, int pixelSize);
	void Quit();

	bool isActive();
	bool KeyPressed(int key);
	float GetDelta();

	void UpdateScreen();
};
