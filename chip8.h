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


class chip8 {
public:
    chip8();

    chip8(unsigned short pc, unsigned short opcode);

    // 4096 (0x1000) kilobytes of memory. Each memory region is 8 bit - one byte.
    // the first 512 bytes (0x200) of the memory are occupied by the interpreter. Therefore, programs usually start
    // at 0x200
    // the upper 256 bytes (0xF00 - 0xFFF) are reserved for display refresh. The 96 bytes below that (0xEA0-0xEFF)
    // are reserved for the call stack, internal use and variables.
    unsigned char memory[0x1000] {0};

    // program counter needs to address 4096 locations
    unsigned short PC {0};

    // register to store the current opcode. There are 35 opcodes, each is 2 bytes long and is stored big-endian
    unsigned short opcode {0};

    // 16 8-bit registers named V0 - VF. The VF register is sometimes a flag.
    unsigned char VF[16] {0};

    void decode();

    void cycle();

private:
    // address register is 16 bits wide
    unsigned short I {0};



    // the stack is used to store the return address when subroutines are called. 48 bytes for 24 levels of nesting
    unsigned short stack[24] {0};

    // resolution is 64*32 pixels monochrome
    unsigned char display[64*32] {0};

    // function to fetch the opcode from memory
    void fetch();

};


#endif //CHIP_8_CHIP8_H
