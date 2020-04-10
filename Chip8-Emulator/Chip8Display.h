#pragma once

#include <SDL/SDL.h>
class Chip8Display {

public:
	bool initializeGraphic();


	SDL_Window* gWindow = NULL;

	SDL_Surface* gScreenSurface = NULL;

	SDL_Surface* gDisplay = NULL;

};

