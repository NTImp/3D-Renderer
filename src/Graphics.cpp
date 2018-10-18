#include "Graphics.hpp"

#include <string.h>
#include <iostream>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>

Window::Window(int w, int h, int pixelSize)
{
	this->w = w;
	this->h = h;

	SDL_Init(SDL_INIT_EVERYTHING);
	IMG_Init(IMG_INIT_PNG | IMG_INIT_JPG);

	win = SDL_CreateWindow("Game", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, w * pixelSize, h * pixelSize, SDL_WINDOW_SHOWN);
	render = SDL_CreateRenderer(win, 0, 0);
	screen = SDL_CreateTexture(render, SDL_PIXELFORMAT_RGBA32, SDL_TEXTUREACCESS_STREAMING, w, h);
	scr = new Pixel[w * h];

}

Window::~Window()
{
	delete scr;
	//delete src.data;
	SDL_DestroyTexture(screen);
	SDL_DestroyWindow(win);
	IMG_Quit();
	SDL_Quit();
}

bool Window::isActive()
{
	static char title[100];
	static Uint32 wticks = SDL_GetTicks();

	delta = (float) (SDL_GetTicks() - wticks) / 1000;

	int FPS = 1.f / delta;

	sprintf(title, "3D ENGINE. %d FPS", FPS);
	SDL_SetWindowTitle(win, title);
	SDL_Event ev;
	while(SDL_PollEvent(&ev)) {
		switch(ev.type) {
			case SDL_QUIT:
				return 0;
			default:
				break;
		}
	}
	wticks = SDL_GetTicks();
	return 1;
}

bool Window::keyPressed(int key)
{
	static const Uint8* keys = SDL_GetKeyboardState(NULL);
	return keys[key];
}

void Window::updateScreen()
{
	SDL_UpdateTexture(screen, NULL, (void*) scr, w * sizeof(Pixel));
	SDL_RenderCopy(render, screen, NULL, NULL);
	SDL_RenderPresent(render);
}

Image::Image(const char* src)
{
	SDL_Surface* raw = IMG_Load(src);
	SDL_Surface* conv = SDL_ConvertSurfaceFormat(raw, SDL_PIXELFORMAT_ABGR8888, 0);
	SDL_FreeSurface(raw);

	w = conv->w;
	h = conv->h;
	data = new Pixel[w * h];

	char* dptr = (char*) conv->pixels;
	for (int i = 0; i < h; i++) {
		char* lptr = dptr + i * conv->pitch;
		for (int e = 0; e < w; e++) {
			Pixel* pptr = ((Pixel*)lptr) + e;
			data[i * w + e] = *pptr;
		}
	}
	SDL_FreeSurface(conv);
}

Image::~Image()
{
	delete data;
}
