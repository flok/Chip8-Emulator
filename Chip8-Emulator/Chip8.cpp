#include "Chip8.h"

#include <iostream>
#include <filesystem>
#include <fstream>

using namespace std;

void Chip8::initialize() {
	// Initialize memory and other stuff for first cycle
	this->memory[4095] = { 0 };

	// clear all registers
	this->V[15] = { 0 };

	// reset address register
	this->I = 0;

	// reset delay timer and sound
	this->delay = 0;
	this->sound_timers = 0;

	// reset programm counter and stackpointer
	this->pc = 0;
	this->sp = 0;

	// reset stack
	this->stack[15] = { 0 };

	// reset keypad
	this->key[15] = { 0 };

	// clear display
	this->display[2047] = { 0 };

	// clear opcode
	this->opcode = 0;
}


bool Chip8::loadFontSet() {
	memcpy(&memory[0], chip8_fontset, 80);

	return SUCCESS;
}

// Loading Programm into memory region starting at 0x200 (512)
bool Chip8::loadProgramm(string path) {

	ifstream _gameFile(path, ios::in | ios::binary);

	if (!_gameFile) {
		cout << "Could not open file : " << path << endl;
	}

	while (!_gameFile.eof()) {
		_gameFile.read((char*)(&this->memory[512]), 3584);
	}

	_gameFile.close();

	return SUCCESS;
}



void Chip8::emulateCycle() {

	// increase programm counter by 2 bytes
	this->pc += 2;

	// OPC is 16 bit because we are merging 2 memory positions with each 8 bit
	// 8 bit + 8 bit = 16 bit
	uint16_t _curOPC = ((this->memory[this->pc] << 8) | this->memory[this->pc + 1]);
	cout << _curOPC << endl;
	uint16_t opcode = 0;

	switch (_curOPC & 0xF000) {
		// 0x00E_
		case 0x0000:
			// calc the 2 diffrent sub opcodes
			switch (_curOPC & 0x000F) {
				case 0x0000:
					// clear display
					this->display[2047] = { 0 };
					break;
				case 0x000E:
					// Return from a subroutine
					--this->sp;
					this->pc = this->stack[this->sp];
					this->pc += 2;
					break;
				default:
					cout << "Unknown OPC " << _curOPC << endl;
			}
			// 0x1___
		case 0x1000:
			// jump to address NNN without return
			this->pc = _curOPC & 0x0FFF;
			break;
			// 0x2___
		case 0x2000:
			// Execute subroutine starting at address NNN
			// save current programmcounter to stack
			this->stack[sp] = this->pc;
			// increase stackpointer to new free stack place
			++this->sp;
			// goto adress
			this->pc = _curOPC & 0x0FFF;
			break;
			// 0x3___
		case 0x3000:
			// 3XNN
			// Skip the following instruction if the value of register VX equals NN
			// check if Vx is NN
			// we shift the first 8 bits to the right to get the register index
			// then we check if  the address is inside that register
			if (this->V[(_curOPC & 0x0F00)] >> 8 == (_curOPC & 0x00FF)) {
				// increase programmcounter by 4 cause 1 instruction is 8 bytes
				// and we need to skip the current and next following instruction
				// current instruction + following instruction => 2 + 2 = 4
				this->pc += 4;
			}
			else {
				// increase programmcounter by 2 cause next instruction
				this->pc += 2;
			}
			break;
		case 0x4000:
			// 4XNN
			// Skip the following instruction if the value of register VX is not equal to NN
			// check if Vx is not NN
			// we shift the first 8 bits to the right to get the register index
			// then we check if  the address is inside that register
			if (this->V[_curOPC & 0x0F00] >> 8 != (_curOPC & 0x00FF)) {
				// increase programmcounter by 4 cause 1 instruction is 8 bytes
				// and we need to skip the current and next following instruction
				// current instruction + following instruction => 2 + 2 = 4
				this->pc += 4;
			}
			else {
				// increase programmcounter by 2 cause next instruction
				this->pc += 2;
			}
			break;
		case 0x5000:
			// 5XY0
			// Skip the following instruction if the value of register VX is equal to the value of register VY
			if (this->V[_curOPC & 0x0F00] >> 8 == this->V[_curOPC & 0x00F0] >> 4) {
				this->pc += 4;
			}
			else {
				this->pc += 2;
			}
			break;
		case 0x6000:
			// 6XNN
			// Store number NN in register VX
			this->V[_curOPC & 0x0F00 >> 8] = opcode & 0x00FF;
			this->pc += 2;
			break;
		case 0x7000:
			// 7XNN
			// Add the value NN to register VX
			this->V[_curOPC & 0x0F00 >> 8] += opcode & 0x00FF;
			this->pc += 2;
			break;
		case 0x8000:

			switch (_curOPC & 0x000F) {
				case 0x0000:
					// 8XY0
					// Store the value of register VY in register VX
					this->V[_curOPC & 0x0F00 >> 8] = this->V[_curOPC & 0x00F0 >> 4];
					this->pc += 2;
					break;
				case 0x0001:
					// 8XY1
					// Set VX to VX OR VY
					this->V[_curOPC & 0x0F00 >> 8] = this->V[_curOPC & 0x0F00 >> 8] |= this->V[_curOPC & 0x00F0 >> 4];
					this->pc += 2;
					break;
				case 0x0002:
					// 8XY2
					// Set VX to VX AND VY
					this->V[_curOPC & 0x0F00 >> 8] = this->V[_curOPC & 0x0F00 >> 8] &= this->V[_curOPC & 0x00F0 >> 4];
					this->pc += 2;
					break;
				case 0x0003:
					// 8XY3
					// Set VX to VX OR VY
					this->V[_curOPC & 0x0F00 >> 8] = this->V[_curOPC & 0x0F00 >> 8] ^= this->V[_curOPC & 0x00F0 >> 4];
					this->pc += 2;
					break;
				case 0x0004:
					this->V[_curOPC & 0x0F00 >> 8] = this->V[_curOPC & 0x0F00 >> 8] + this->V[_curOPC & 0x00F0 >> 4];

					// check if carry occured
					if (this->V[_curOPC & 0x0F00 >> 8] + this->V[_curOPC & 0x00F0 >> 4] > (0xFF - )
					break;
				case 0x0005:
					break;
				case 0x0006:
					break;
				case 0x0007:
					break;
				case 0x000E:
					break;
				default:
					cout << "Unknown OPCODE " << _curOPC << endl;
			}
			break;
		case 0x9000:
			break;
		case 0xA000:
			break;
		case 0xB000:
			break;
		case 0xC000:
			break;
		case 0xD000:
			break;
		case 0xE000:
			break;
		case 0xF000:
			break;
	}


	// Fetch Opcode
	// Decode Opcode
	// Execute Opcode

	// Update timers
}



/// Graphic Stuff

bool Chip8::initializeGraphic() {
	if (SDL_Init(SDL_INIT_VIDEO) < 0) {
		cout << "SDL could not initialze! Error : " << SDL_GetError() << endl;
		return ERROR;
	}

	// Create the Window
	gWindow = SDL_CreateWindow("Chip8 Emulator", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_RESIZABLE);
	if (gWindow == NULL) {
		cout << "Window coult not be created! Error : " << SDL_GetError() << endl;
		return ERROR;
	}

	gRenderer = SDL_CreateRenderer(gWindow, -1, SDL_RENDERER_ACCELERATED);

	gScreenSurface = SDL_CreateRGBSurfaceFrom(this->display, 64, 32, 8, 0, 0, 0, 0, 0);

	return SUCCESS;
}