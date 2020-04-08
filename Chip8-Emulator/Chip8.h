#pragma once
class Chip8
{
public:

	Chip8();

	void initialize();

	void emulateCycle();

private:


	// memory
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

