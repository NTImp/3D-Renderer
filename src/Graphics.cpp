#include "Graphics.hpp"

#include <iostream>
#include <SDL2/SDL.h>

SDL_Window* window = nullptr;
SDL_Renderer* render = nullptr;
SDL_Texture* screen = nullptr;


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
		window = SDL_CreateWindow("Game", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, screen_w * pixelSize, screen_h * pixelSize, SDL_WINDOW_SHOWN);
		render = SDL_CreateRenderer(window, 0, 0);

		screen = SDL_CreateTexture(render, SDL_PIXELFORMAT_RGBA32, SDL_TEXTUREACCESS_STREAMING, screen_w, screen_h);
		printf("Locking\n");
		SDL_LockTexture(screen, NULL, (void**) &scr.data, &scr.p);
		printf("Locked\n");

		return &scr;
	}

	void Quit()
	{
		SDL_DestroyTexture(screen);
		SDL_DestroyWindow(window);
		SDL_Quit();
	}

	bool isActive()
	{
		SDL_Event ev;
		while(SDL_PollEvent(&ev)) {
			switch(ev.type) {
				case SDL_QUIT:
					return 0;
				default:
					break;
			}
		}
		return 1;
	}

	bool KeyPressed(int key)
	{
		static const Uint8* keys = SDL_GetKeyboardState(NULL);
		return keys[key];
	}

	void UpdateScreen()
	{
		SDL_UnlockTexture(screen);
		SDL_RenderCopy(render, screen, NULL, NULL);
		SDL_RenderPresent(render);
		SDL_LockTexture(screen, NULL, (void**) &scr.data, &scr.p);
	}
}