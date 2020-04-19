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
	this->pc = 512;
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
	//memcpy(&memory[0], chip8_fontset, 80);

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

	// OPC is 16 bit because we are merging 2 memory positions with each 8 bit
	// 8 bit + 8 bit = 16 bit
	uint16_t _curOPC = ((this->memory[this->pc] << 8) | this->memory[this->pc + 1]);

	uint8_t N = _curOPC & 0x000F; // lowest 4 bits
	uint8_t NN = _curOPC & 0x00FF; // lowest 8 bits
	uint16_t NNN = _curOPC & 0x0FFF; // lowest 12 bits
	uint8_t X = _curOPC >> 8 & 0x000F; // lower 4 bits of the high byte
	uint8_t Y = _curOPC >> 4 & 0x000F; // upper 4 bits of the low byte



	switch (_curOPC & 0xF000) {
		// 0x00E_
		case 0x0000:
			// calc the 2 diffrent sub opcodes
			switch (N) {
				case 0x0000:
				{
					// clear display
					this->display[2047] = { 0 };
					this->draw = true;
					this->pc += 2;
					break;
				}
				case 0x000E:
					// Return from a subroutine
					--this->sp;
					this->pc = this->stack[this->sp];
					this->pc += 2;
					break;
				default:
					cout << "Unknown OPC " << _curOPC << endl;
					break;
			}
			break;
			// 0x1___
		case 0x1000:
			// jump to address NNN without return
			this->pc = NNN;
			break;
			// 0x2___
		case 0x2000:
			// Execute subroutine starting at address NNN
			// save current programmcounter to stack
			this->stack[this->sp] = this->pc;
			// increase stackpointer to new free stack place
			++this->sp;
			// goto adress
			this->pc = NNN;
			break;
			// 0x3___
		case 0x3000:
			// 3XNN
			// Skip the following instruction if the value of register VX equals NN
			// check if Vx is NN
			// we shift the first 8 bits to the right to get the register index
			// then we check if  the address is inside that register
			if (this->V[X] == NN) {
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
			if (this->V[X] != NN) {
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
			if (this->V[X] == this->V[Y]) {
				this->pc += 4;
			}
			else {
				this->pc += 2;
			}
			break;
		case 0x6000:
			// 6XNN
			// Store number NN in register VX
			this->V[X] = NN;
			this->pc += 2;
			break;
		case 0x7000:
			// 7XNN
			// Add the value NN to register VX
			this->V[X] += NN;
			this->pc += 2;
			break;
		case 0x8000:

			switch (N) {
				case 0x0000:
					// 8XY0
					// Store the value of register VY in register VX
					this->V[X] = this->V[Y];
					this->pc += 2;
					break;
				case 0x0001:
					// 8XY1
					// Set VX to VX OR VY
					this->V[X] = this->V[X] |= this->V[Y];
					this->pc += 2;
					break;
				case 0x0002:
					// 8XY2
					// Set VX to VX AND VY
					this->V[X] = this->V[X] &= this->V[Y];
					this->pc += 2;
					break;
				case 0x0003:
					// 8XY3
					// Set VX to VX XOR VY
					this->V[X] = this->V[X] ^= this->V[Y];
					this->pc += 2;
					break;
				case 0x0004:
					// Add the value of register VY to register VX
					// Set VF to 01 if a carry occurs
					// Set VF to 00 if a carry does not occur
					// compute here
					V[X] += V[Y];
					// we need to first determine if a carry will be set before adding the values otherwise false result
	// 0xFF - V[Y] computes the maximum number that can be added to V[X] without overflowing (carry)
					if (V[X] > (0xFF - V[Y])) {
						V[0xF] = 1; // carry
					}
					else {
						V[0xF] = 0; // no carry 
					}

					this->pc += 2;
					break;
				case 0x0005:
					// Subtract the value of register VY from register VX
					// Set VF to 00 if a borrow occurs
					// Set VF to 01 if a borrow does not occur
					if (this->V[X] < this->V[Y]) {
						this->V[0xF] = 0;
					}
					else {
						this->V[0xF] = 1;
					}
					this->V[X] -= this->V[Y];
					this->pc += 2;
					break;
				case 0x0006:
					// Store the value of register VY shifted right one bit in register VX
					// Set register VF to the least significant bit prior to the shift VY is unchanged
					this->V[0xF] = this->V[Y] & 0x1;
					// shift left 
					this->V[X] = this->V[Y] >>= 1;

					this->pc += 2;
					break;
				case 0x0007:
					if (this->V[X] > this->V[Y]) {
						this->V[0xF] = 0;
					}
					else {
						this->V[0xF] = 1;
					}
					this->V[X] = this->V[Y] - this->V[X];
					this->pc += 2;
					break;
				case 0x000E:
					// Store the value of register VY shifted right one bit in register VX
					// Set register VF to the least significant bit prior to the shift VY is unchanged

					this->V[0xF] = this->V[X] >> 7;
					// shift left 
					this->V[X] = this->V[X] <<= 1;

					this->pc += 2;
					break;
				default:
					cout << "Unknown OPCODE 0 " << _curOPC << endl;
			}
			break;
		case 0x9000:
			if (this->V[X] != this->V[Y]) {
				this->pc += 4;
			}
			else {
				this->pc += 2;
			}
			break;
		case 0xA000:
			this->I = NNN;
			this->pc += 2;
			break;
		case 0xB000:
			// No pc += 2 because we want to jump to that exact address
			this->pc = NNN + this->V[0];
			break;
		case 0xC000:
			this->V[X] = (rand() % (0xFF + 1) & NN);
			this->pc += 2;
			break;

			// display drawing
		case 0xD000:


			// Draw a sprite at position VX, VY with N bytes of sprite data starting at the address stored in I
			// Set VF to 01 if any set pixels are changed to unset, and 00 otherwise
		{
			uint8_t xCord = this->V[X];
			uint8_t yCord = this->V[Y];

			V[0xF] = 0; // reset V[F] cause  we maybe set it to 1
			for (int row = 0; row < N; row++) {
				uint8_t pixel = this->memory[this->I + row];
				//cout << pixel << endl;

				for (int column = 0; column < 8; column++) {
					if ((pixel & (0x0080 >> column)) != 0) {
						if (this->display[(xCord + column + ((yCord + row) * 64))] == 1) {
							this->V[0xF] = 1;
						}
						this->display[(xCord + column + ((yCord + row) * 64))] ^= 1;
					}
				}

			}
			//cout << "-----------------" << endl;
			this->pc += 2;
			this->draw = true;

			break;
		}
		case 0xE000:
			switch (NN) {
				case 0x009E:
					if (key[this->V[X]] != 0) {
						this->pc += 4;
					}
					else {
						this->pc += 2;
					}
					break;
				case 0x00A1:
					if (key[this->V[X]] == 0) {
						this->pc += 4;
					}
					else {
						this->pc += 2;
					}
					break;
				default:
					cout << "Unknown opcode 1" << _curOPC << endl;
			}
			break;
		case 0xF000:
			switch (NN) {
				case 0x0007:
					this->V[X] = this->delay;
					this->pc += 2;
					break;
				case 0x000A:
				{
					bool keyPressed = false;
					// 16 because we have 16 keys on the pad
					for (int i = 0; i < 16; i++) {
						if (this->key[i] != 0) {
							this->V[X] = i;
							keyPressed = true;
						}
					}
					// wait for key press if no key pressed we try again
					if (!keyPressed)
						return;
					this->pc += 2;
					break;
				}
				case 0x0015:
					this->delay = this->V[X];
					this->pc += 2;
					break;
				case 0x0018:
					this->sound_timers = this->V[X];
					this->pc += 2;
					break;
				case 0x001E:
					this->I += this->V[X];
					this->pc += 2;
					break;
				case 0x0029:
					this->I = this->V[X] * 0x5;
					this->pc += 2;
					break;
				case 0x0033:
					this->memory[I] = V[X] / 100;
					this->memory[I + 1] = (V[X] / 10) % 10;
					this->memory[I + 2] = V[X] % 10;
					this->pc += 2;
					break;
				case 0x0055:
					for (int i = 0; i <= X; i++) {
						this->memory[this->I + i] = this->V[i];
					}

					this->I += X + 1;
					this->pc += 2;
					break;
				case 0x0065:
					for (int i = 0; i <= X; i++) {
						this->V[i] = this->memory[I + i];
					}

					this->I += X + 1;
					this->pc += 2;
					break;
				default:
					cout << "Unknown opcode2 " << _curOPC << endl;
			}
			break;
		default:
			cout << "Unknown opcode 3 " << _curOPC << endl;
	}

	if (this->delay > 0) {
		--this->delay;
	}

	if (this->sound_timers > 0) {
		if (this->sound_timers == 1) {
			cout << "SOUND" << endl;
		}
		--this->sound_timers;
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
	gWindow = SDL_CreateWindow("Chip8 Emulator", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, SCREEN_WIDTH * 8, SCREEN_HEIGHT * 8, SDL_WINDOW_RESIZABLE);
	if (gWindow == NULL) {
		cout << "Window coult not be created! Error : " << SDL_GetError() << endl;
		return ERROR;
	}

	gRenderer = SDL_CreateRenderer(gWindow, -1, 0);

	SDL_RenderSetLogicalSize(gRenderer, SCREEN_WIDTH, SCREEN_HEIGHT);

	gScreenTexture = SDL_CreateTexture(gRenderer, SDL_PIXELFORMAT_ABGR8888, SDL_TEXTUREACCESS_STREAMING, SCREEN_WIDTH, SCREEN_HEIGHT);

	return SUCCESS;
}