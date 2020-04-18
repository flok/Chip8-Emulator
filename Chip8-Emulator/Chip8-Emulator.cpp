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

int main(int argc, char* argv[])
{
    // create cmdline parser


    // initialize the chip 8
    // set memory
    chip8.loadProgramm("C://Users//adgasdgasdgasd//Documents//Local Git Repos//Chip8-Emulator//Debug//games//TETRIS");

    chip8.loadFontSet();


    if (!chip8.initializeGraphic()) {
        cout << "Could not initialize graphic" << endl;
    }

    uint8_t t = 0xfe;


    while (true) {
        // do the opcode stuff
        chip8.emulateCycle();

        // do the SDL display magic
        chip8.UpdateGraphic();


        // slow the emulation speed to the real speed of chip8
        //this_thread::sleep_for(chrono::microseconds(1200));
        Sleep(1.2f);
    }


    return SUCCESS;
}
