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
	unsigned char memory[4096];

	// general purpose 8 bit registers
	// V15 is not used by any programm, used as flag for some instructions.
	unsigned char V[16];


	// Address register
	// can only be loaded bwith 12bit address, due to 
	// range of memory to CHIP-8 instruction
	unsigned short I;

	unsigned char delay;
	unsigned char sound_timers;

	// programm counter (pc)
	// current executing address
	unsigned short pc;

	// stack pointer (sp)
	// topmost level of the stack
	unsigned char sp;

	// stack
	unsigned short stack[16];

	// keyboard
	//	***********
	//	* 1	2 3	C *
	//	* 4 5 6	D *
	//	* 7	8 9	E *
	//	* A	0 B	F *
	//	***********
	unsigned char key[16];

	// display 
	// 64 * 32 pixel big
	unsigned char display[64 * 32];


	unsigned short opcode;




};

