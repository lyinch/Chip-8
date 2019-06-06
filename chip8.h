/*MIT License

Copyright (c) 2019 by lyinch

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
SOFTWARE.
*/


#ifndef CHIP_8_CHIP8_H
#define CHIP_8_CHIP8_H

#include <random>

class chip8 {
public:
    chip8(){
        rng = std::mt19937(dev());
        init();
    };

    chip8(unsigned short pc, unsigned short opcode);

    // 4096 (0x1000) kilobytes of memory. Each memory region is 8 bit - one byte.
    // the first 512 bytes (0x200) of the memory are occupied by the interpreter. Therefore, programs usually start
    // at 0x200
    // the upper 256 bytes (0xF00 - 0xFFF) are reserved for display refresh. The 96 bytes below that (0xEA0-0xEFF)
    // are reserved for the call stack, internal use and variables.
    // Addresses 0x000 - 0x50 are reserved for the hexadecimal sprites. 16 sprites ate 5 bytes each.
    unsigned char memory[0x1000] {0};

    // Each character of the hexadecimal system is represented with 5 bytes. For example the character 2:
    /*

    sprite  binary      hex
    ****    11110000    0xF0
       *    00010000    0x10
    ****    11110000    0xF0
    *       10000000    0x80
    ****    11110000    0xF0

     */
    // This representation needs to be copied into memory
    unsigned char fonts[0x50]{
            0xF0, 0x90, 0x90, 0x90, 0xF0, //0
            0x20, 0x60, 0x20, 0x20, 0x70, //1
            0xF0, 0x10, 0xF0, 0x80, 0xF0, //2
            0xF0, 0x10, 0xF0, 0x10, 0xF0, //3
            0x90, 0x90, 0xF0, 0x10, 0x10, //4
            0xF0, 0x80, 0xF0, 0x10, 0xF0, //5
            0xF0, 0x80, 0xF0, 0x90, 0xF0, //6
            0xF0, 0x10, 0x20, 0x40, 0x40, //7
            0xF0, 0x90, 0xF0, 0x90, 0xF0, //8
            0xF0, 0x90, 0xF0, 0x10, 0xF0, //9
            0xF0, 0x90, 0xF0, 0x90, 0x90, //A
            0xE0, 0x90, 0xE0, 0x90, 0xE0, //B
            0xF0, 0x80, 0x80, 0x80, 0xF0, //C
            0xE0, 0x90, 0x90, 0x90, 0xE0, //D
            0xF0, 0x80, 0xF0, 0x80, 0xF0, //E
            0xF0, 0x80, 0xF0, 0x80, 0x80  //F
    };

    // program counter needs to address 4096 locations
    unsigned short PC {0};

    // register to store the current opcode. There are 35 opcodes, each is 2 bytes long and is stored big-endian
    unsigned short opcode {0};

    // 16 8-bit registers named V0 - VF. The VF register is sometimes a flag.
    unsigned char VF[16] {0};

    // address register is 16 bits wide
    unsigned short I {0};

    // two timers that count down at 60Hz to 0
    unsigned  char delay_timer {0};
    unsigned  char sound_timer {0};

    void decode();

    void cycle();

    void init();

    void load_program(u_int16_t data[]);

    // Chip-8 has a hexadecimal keyboard. key[X] is true, if the key is currently pressed
    bool key[16] {false};

    unsigned char random_256(){
        return rand_256(rng);
    }

private:

    std::random_device dev; // generate a (mostly) true, slow random number
    std::mt19937 rng; // seed the mersenne twister with the random number. Pseudo-random number generator
    std::uniform_int_distribution<std::mt19937::result_type> rand_256; // tweak the random numbers to a range of [0,255] both inclusive. This is guaranteed to be unbiased

    // the stack is used to store the return address when subroutines are called. 48 bytes for 24 levels of nesting
    unsigned short stack[24] {0};

    // resolution is 64*32 pixels monochrome
    unsigned char display[64*32] {0};

    // function to fetch the opcode from memory
    void fetch();

};


#endif //CHIP_8_CHIP8_H
