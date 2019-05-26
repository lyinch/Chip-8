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
                case 4:{
                    // 8XY4 adds VY to VX. Set VF to 1 if there's a carry, and to 0 otherwise
                    // registers are 8 bit. We sum them up in a 16bit variable and check if there was an overflow.
                    // 0x100 gives us b1 0000 0000 and only works because the chip-8 is a big-endian machine
                    unsigned short sum = VF[vX] + VF[vY];
                    VF[0xF] = (sum & 0x100)>>8;
                    VF[vX] = sum;
                    PC += 2;
                    break;
                }
                case 5:{
                    // 8XY5 subtracts VY from VX. Set VF to 0 if there's a borrow, and to 1 otherwise
                    unsigned short difference = VF[vX] - VF[vY];
                    if(VF[vY] > VF[vX])
                        VF[0xF] = 0;
                    else
                        VF[0xF] = 1;
                    VF[vX] = difference;
                    PC += 2;
                    break;
                }
                case 6:{
                    // 8XY6 stores the least significant bit of VX in VF, then shifts VX to the right by 1
                    VF[0xF] = VF[vX] & 0x1;
                    VF[vX] >>= 1;
                    PC += 2;
                    break;
                }
                case 7:{
                    // 8XY7 sets VX to VY minus VX. VF is set to 0 when there's a borrow, and 1 otherwise
                    unsigned short difference = VF[vY] - VF[vX];
                    if(VF[vX] > VF[vY])
                        VF[0xF] = 0;
                    else
                        VF[0xF] = 1;
                    VF[vX] = difference;
                    PC += 2;
                    break;
                }
                case 0xE:{
                    // 8XYE stores the most significant bit of VX in VF, then shifts VX to the left by 1
                    VF[0xF] = (VF[vX] &0x80) >> 7; // 0x80 = b 1000 000
                    VF[vX] <<= 1;
                    PC += 2;
                    break;
                }
            }
            break;
        }
    }
}

chip8::chip8( unsigned short pc, unsigned short opcode) : PC(pc), opcode(opcode) {}
