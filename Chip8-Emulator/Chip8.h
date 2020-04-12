#pragma once


#include <SDL/SDL.h>

#include "global.h"


class Chip8
{
public:
	// Initialize the memory and fontset
	void initialize();

	bool loadFontSet();

	bool loadProgramm(string path);


	// create SDL window and key callbacks etc..
	bool initializeGraphic();

	void emulateCycle();

private:



	unsigned char chip8_fontset[80] =
	{
	  0xF0, 0x90, 0x90, 0x90, 0xF0, // 0
	  0x20, 0x60, 0x20, 0x20, 0x70, // 1
	  0xF0, 0x10, 0xF0, 0x80, 0xF0, // 2
	  0xF0, 0x10, 0xF0, 0x10, 0xF0, // 3
	  0x90, 0x90, 0xF0, 0x10, 0x10, // 4
	  0xF0, 0x80, 0xF0, 0x10, 0xF0, // 5
	  0xF0, 0x80, 0xF0, 0x90, 0xF0, // 6
	  0xF0, 0x10, 0x20, 0x40, 0x40, // 7
	  0xF0, 0x90, 0xF0, 0x90, 0xF0, // 8
	  0xF0, 0x90, 0xF0, 0x10, 0xF0, // 9
	  0xF0, 0x90, 0xF0, 0x90, 0x90, // A
	  0xE0, 0x90, 0xE0, 0x90, 0xE0, // B
	  0xF0, 0x80, 0x80, 0x80, 0xF0, // C
	  0xE0, 0x90, 0x90, 0x90, 0xE0, // D
	  0xF0, 0x80, 0xF0, 0x80, 0xF0, // E
	  0xF0, 0x80, 0xF0, 0x80, 0x80  // F
	};


	// memory pointer
	// 8 bit
	uint8_t memory[4096];

	// general purpose 8 bit registers
	// V15 is not used by any programm, used as flag for some instructions.
	uint8_t V[16];


	// Address register
	// can only be loaded bwith 12bit address, due to 
	// range of memory to CHIP-8 instruction
	// 16 bit
	uint16_t I;

	// 8 bit
	uint8_t delay;
	uint8_t sound_timers;

	// programm counter (pc)
	// current executing address
	// 16bit
	uint16_t pc;

	// stack pointer (sp)
	// topmost level of the stack
	// 16bit
	uint16_t sp;

	// stack
	uint16_t stack[16];

	// keyboard
	//	***********
	//	* 1	2 3	C *
	//	* 4 5 6	D *
	//	* 7	8 9	E *
	//	* A	0 B	F *
	//	***********
	uint8_t key[16];

	// display 
	// 64 * 32 pixel big
	uint8_t display[64 * 32];


	uint16_t opcode;



	SDL_Window* gWindow = NULL;

	SDL_Renderer* gRenderer = NULL;

	SDL_Surface* gScreenSurface = NULL;


};

