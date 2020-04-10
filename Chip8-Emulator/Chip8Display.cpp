#include "Chip8Display.h"
#include <SDL/SDL.h>

#include "global.h"



bool Chip8Display::initializeGraphic() {
	if (SDL_Init(SDL_INIT_VIDEO) < 0) {
		cout << "SDL could not initialze! Error : " << SDL_GetError() << endl;
	}
	else {
		// Create the Window
		gWindow = SDL_CreateWindow("Chip8 Emulator", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_RESIZABLE);
		if (gWindow == NULL) {
			cout << "Window coult not be created! Error : " << SDL_GetError() << endl;
		}
		else {
			gScreenSurface = SDL_GetWindowSurface(gWindow);
		}
	}
	return SUCCESS;
}