#pragma once
#include <SDL2/SDL.h>

namespace Graphics {
#pragma packed(1)
	struct Pixel {
		unsigned char r;
		unsigned char g;
		unsigned char b;
		unsigned char a;
	};

	struct Screen {
		int w, h, p;
		Pixel* data;
	};

	struct Image {
		int w, h;
		Pixel* data;
	};

	Screen* Init(int w, int h, int pixelSize);
	void Quit();

	bool isActive();
	bool KeyPressed(int key);
	float GetDelta();

	void UpdateScreen();

	Image* LoadImage(const char* src);
	void FreeImage(Image* img);
};
