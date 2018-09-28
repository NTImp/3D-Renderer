#include "Graphics.hpp"

#include <string.h>
#include <iostream>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>

SDL_Window* window = nullptr;
SDL_Renderer* render = nullptr;
SDL_Texture* screen = nullptr;

float delta = 0;

namespace Graphics {
	Screen scr;

	Screen* Init(int screen_w, int screen_h, int pixelSize)
	{
		scr.w = screen_w;
		scr.h = screen_h;
		
		if (window != nullptr) {
			std::cout << "CANNOT INIT GRAPHICS TWICE, ABORTING" << std::endl;
			throw;
		}

		SDL_Init(SDL_INIT_EVERYTHING);
		IMG_Init(IMG_INIT_PNG | IMG_INIT_JPG);
		window = SDL_CreateWindow("Game", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, screen_w * pixelSize, screen_h * pixelSize, SDL_WINDOW_SHOWN);
		render = SDL_CreateRenderer(window, 0, 0);

		screen = SDL_CreateTexture(render, SDL_PIXELFORMAT_RGBA32, SDL_TEXTUREACCESS_STREAMING, screen_w, screen_h);
		SDL_LockTexture(screen, NULL, (void**) &scr.data, &scr.p);

		//scr.data = new Pixel[scr.w * scr.h];
		return &scr;
	}

	void Quit()
	{
		//delete src.data;
		SDL_DestroyTexture(screen);
		SDL_DestroyWindow(window);
		IMG_Quit();
		SDL_Quit();
	}

	bool isActive()
	{
		static char title[100];
		static Uint32 wticks = SDL_GetTicks();

		delta = (float) (SDL_GetTicks() - wticks) / 1000;

		int FPS = 1.f / delta;

		sprintf(title, "3D ENGINE. %d FPS", FPS);
		SDL_SetWindowTitle(window, title);
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

	bool KeyPressed(int key)
	{
		static const Uint8* keys = SDL_GetKeyboardState(NULL);
		return keys[key];
	}

	float GetDelta()
	{
		return delta;
	}

	void UpdateScreen()
	{
		SDL_UnlockTexture(screen);
		SDL_RenderCopy(render, screen, NULL, NULL);
		SDL_RenderPresent(render);
		SDL_LockTexture(screen, NULL, (void**) &scr.data, &scr.p);

		/*
		for (int y = 0; y < scr.h; y++) {
			for (int x = 0; x < scr.w; x++) {
				Pixel p = scr.data[y * scr.w + x];
				SDL_SetRenderDrawColor(render, p.r, p.g, p.b, 255);
				SDL_RenderDrawPoint(render, x, y);
			}
		}
		SDL_RenderPresent(render);
		*/
	}

	Image* LoadImage(const char* src)
	{
		SDL_Surface* raw = IMG_Load(src);
		SDL_Surface* conv = SDL_ConvertSurfaceFormat(raw, SDL_PIXELFORMAT_ABGR8888, 0);
		SDL_FreeSurface(raw);

		Image* img = new Image;
		img->w = conv->w;
		img->h = conv->h;

		img->data = new Pixel[img->w * img->h];

		printf("%d, %d\n", img->w, img->h);

		char* dptr = (char*) conv->pixels;
		for (int i = 0; i < img->h; i++) {
			char* lptr = dptr + i * conv->pitch;
			for (int e = 0; e < img->w; e++) {
				Pixel* pptr = ((Pixel*)lptr) + e;
				img->data[i * img->w + e] = *pptr;
			}
		}

		SDL_FreeSurface(conv);
		return img;
	}

	void FreeImage(Image* img)
	{
		delete img->data;
		delete img;
	}
}
