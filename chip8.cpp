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


#include <iostream>
#include "chip8.h"

chip8::chip8() {}

void chip8::cycle() {
    fetch();
    decode();
}

void chip8::fetch(){
    chip8::opcode = memory[PC] << 8 | memory[PC+1];
}

void chip8::decode() {
    // register index is, if present, always at this position
    unsigned char vX = (opcode & 0x0F00) >> 8;
    unsigned char vY = (opcode & 0x00F0) >> 4;

    // opcodes are in the form of ABCD where each letter corresponds 4 bits. We first do a switch-case on the first 4
    // bits of the opcode
    switch (opcode&0xF000){
        case 0x0000:{
            // opcodes in the form of 0x00CD There are only two, 00EE and 00E0
            switch (opcode&0x00FF){
                case 0x0E00:
                    std::cout << opcode << " not implemented" << std::endl;
                    break;

                case 0x00EE:
                    std::cout << opcode << " not implemented" << std::endl;
                    break;

                default:
                    std::cout << "Unknown opcode: " << opcode << std::endl;
                    break;
            }
            break;
        }

        case 0x1000:{
            // 1NNN  jumps to address NNN
            PC = opcode&0x0FFF;
            break;
        }

        case 0x2000:{
            std::cout << opcode << " not implemented" << std::endl;
            break;
        }

        case 0x3000:{
            // 3XNN skips the next instruction if VX equals NN
            if(VF[vX] == (opcode & 0x00FF))
                PC += 4;
            else
                PC += 2;
            break;
        }

        case 0x4000:{
            // 4XNN skips the next instruction if VX does not equal NN
            if(VF[vX] != (opcode & 0x00FF))
                PC += 4;
            else
                PC += 2;
            break;
        }

        case 0x5000:{
            // 5XY0 skips the next instruction if VX equals VY
            if(VF[vX] == VF[vY])
                PC += 4;
            else
                PC += 2;
            break;
        }

        case 0x6000:{
            // 6XNN set VX to NN
            VF[vX] = (opcode & 0x00FF);
            PC += 2;
            break;
        }

        case 0x7000:{
            // 7XNN adds NN to VX, carry flag is not changed
            VF[vX] += opcode&0x00FF;
            PC += 2;
            break;
        }

        case 0x8000:{
            switch (opcode&0x000F){
                case 0:{
                    // 8XY0 sets VX to the value of VY
                    VF[vX] = VF[vY];
                    PC+= 2;
                    break;
                }

                case 1:{
                    // 8XY1 sets VX to VX | VY (bitwise or)
                    VF[vX] |= VF[vY];
                    PC+= 2;
                    break;
                }

                case 2:{
                    // 8XY2 sets VX to VX & VY (bitwise and)
                    VF[vX] &=VF[vY];
                    PC+= 2;
                    break;
                }

                case 3:{
                    // 8XY3 sets VX to VX ^ VY (bitwise xor)
                    VF[vX] ^=VF[vY];
                    PC+= 2;
                    break;
                }
            }
            break;
        }
    }
}

chip8::chip8( unsigned short pc, unsigned short opcode) : PC(pc), opcode(opcode) {}
