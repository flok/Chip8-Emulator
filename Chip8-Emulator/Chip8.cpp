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
	// Fetch Opcode
	// Decode Opcode
	// Execute Opcode

	// Update timers
}
