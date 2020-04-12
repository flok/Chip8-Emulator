//// Chip8 Core logic
#include "Chip8.h"


#include "SDL/SDL.h"

// Console output
#include <iostream>
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



    while (true) {

        chip8.emulateCycle();
    }


    return SUCCESS;
}
