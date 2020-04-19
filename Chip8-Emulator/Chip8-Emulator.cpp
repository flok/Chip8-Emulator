//// Chip8 Core logic

#include <Windows.h>

#include "Chip8.h"


#include "SDL/SDL.h"

// Console output
#include <iostream>
#include <thread>
#include <chrono>



using namespace std;


Chip8 chip8;

int main(int argc, char* argv[]) {
	// create cmdline parser


	// initialize the chip 8
	// set memory
	load:
	chip8.loadProgramm("C://Users//adgasdgasdgasd//Documents//Local Git Repos//Chip8-Emulator//Debug//games//PONG");

	chip8.loadFontSet();


	if (!chip8.initializeGraphic()) {
		cout << "Could not initialize graphic" << endl;
	}


	uint32_t screen[2048];

	while (true) {
		// pause emulation
		bool pause = false;

		// quit emulation
		bool quit = false;

		// do the opcode stuff
		chip8.emulateCycle();

		// do the SDL display magic
		if (chip8.draw) {
			chip8.draw = false;
			for (int i = 0; i < 2048; ++i) {
				uint8_t pixel = chip8.display[i];
				screen[i] = (0x00FFFFFF * pixel) | 0xFF000000;
			}


			// update the texture with the new pixel data
			SDL_UpdateTexture(chip8.gScreenTexture, NULL, screen, 64 * sizeof(uint32_t));

			// Clear screen and rerender
			SDL_RenderClear(chip8.gRenderer);
			SDL_RenderCopy(chip8.gRenderer, chip8.gScreenTexture, NULL, NULL);
			SDL_RenderPresent(chip8.gRenderer);

		}

		// Key Press stuff
		SDL_Event e; // Event handler

		while (SDL_PollEvent(&e) != 0) {
			if (e.type == SDL_QUIT) {
				exit(0);
			}
			else if (e.type == SDL_KEYDOWN) {

				if (e.key.keysym.scancode == SDL_SCANCODE_ESCAPE)
					exit(0);
				
				// Reset emulation
				if (e.key.keysym.scancode == SDL_SCANCODE_F1) {
					goto load;
				}

				for (int i = 0; i < 16; i++) {
					if (e.key.keysym.scancode == chip8.keymap[i]) {
						chip8.key[i] = 1;
					}
				}
			}
			else if (e.type == SDL_KEYUP) {
				for (int i = 0; i < 16; i++) {
					if (e.key.keysym.scancode == chip8.keymap[i]) {
						chip8.key[i] = 0;
					}
				}
			}
		}

		// slow the emulation speed to the real speed of chip8
		//this_thread::sleep_for(chrono::microseconds(1200));
		Sleep(1.2f);
	}


	return SUCCESS;
}
