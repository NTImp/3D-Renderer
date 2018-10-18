#pragma once
#include <SDL2/SDL.h>

#pragma packed(1)
struct Pixel {
	unsigned char r;
	unsigned char g;
	unsigned char b;
	unsigned char a;
};

class Window {
public:
	Window(int w, int h, int pixelSize);
	~Window();

	bool isActive();
	bool keyPressed(int key);

	inline float getDelta()
	{
		return delta;
	}
	inline int getW()
	{
		return w;
	}
	inline int getH()
	{
		return h;
	}

	inline Pixel* getScreen()
	{
		return scr;
	}

	void updateScreen();
private:
	SDL_Window* win;
	SDL_Renderer* render;
	SDL_Texture* screen;
	int w, h;
	float delta;
	Pixel* scr;
};

class Image {
public:
	Image(const char* src);
	~Image();

	inline int getW() {
		return w;
	};

	int getH() {
		return h;
	};

	inline const Pixel* getData() {
		return data;
	};
private:
	int w, h;
	Pixel* data;
};
