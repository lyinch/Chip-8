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
#include <iterator>

void chip8::cycle() {
    fetch();
    decode();
}

void chip8::fetch(){
    chip8::opcode = (memory[PC] << 8) | memory[PC+1];
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
                case 0x00E0:
                    sprintf(info_string,"opcode 0x%04x not implemented",opcode);
                    PC+=2;
                    //std::cout << opcode << " not implemented" << std::endl;
                    break;

                case 0x00EE:
                    sprintf(info_string,"opcode 0x%04x not implemented",opcode);
                    //std::cout << opcode << " not implemented" << std::endl;
                    break;

                default:
                    sprintf(info_string,"Unknown opcode 0x%04x",opcode);
                    //std::cout << "Unknown opcode: " << opcode << std::endl;
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
            sprintf(info_string,"opcode 0x%04x not implemented",opcode);
            //std::cout << opcode << " not implemented" << std::endl;
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
        case 0x9000: {
            // 9XY0 skips the next instruction if VX != VY
            if (VF[vX] == VF[vY])
                PC += 2;
            else
                PC += 4;
            break;
        }

        case 0xA000:{
            // ANNN sets I to the address NNN
            I = opcode&0x0FFF;
            PC += 2;
            break;
        }

        case 0xB000:{
            // BNNN jumps to the address NNN + V0
            PC = VF[0] + opcode&0x0FFF;
            break;
        }
        case 0xC000:{
            // CXNN Sets VX to the result of NN&rand()
            VF[vX] = (opcode & 0x00FF) & random_256();
            PC += 2;
            break;
        }

        case 0xD000:{
            //std::cout << opcode << " not implemented" << std::endl;
            sprintf(info_string,"opcode 0x%04x not implemented",opcode);
            break;
        }

        case 0xE000:{
            switch (opcode&0x000F){
                case 0xE:{
                    // EX9E skips the next instruction if the key stored in VX is pressed
                    if(key[VF[vX]])
                        PC += 4;
                    else
                        PC += 2;
                    break;
                }
                case 0x1:{
                    // EXA1 Skips the next instruction if the key stored in VX isn't pressed
                    if(key[VF[vX]])
                        PC += 2;
                    else
                        PC += 4;
                    break;
                }
            }
            break;
        }

        case 0xF000:{
            switch (opcode&0x00FF){
                case 0x07:{
                    // FX07 sets VX to the value of the delay timer
                    VF[vX] = delay_timer;
                    PC += 2;
                    break;
                }
                case 0x0A:{
                    //std::cout << opcode << " not implemented" << std::endl;
                    sprintf(info_string,"opcode 0x%04x not implemented",opcode);
                    break;
                }
                case 0x15:{
                    // FX15 sets the delay timer to VX
                    delay_timer = VF[vX];
                    PC += 2;
                    break;
                }
                case 0x18:{
                    // FX15 sets the sound timer to VX
                    sound_timer = VF[vX];
                    PC += 2;
                    break;
                }
                case 0x1E:{
                    // FX1E adds VX to I
                    I += VF[vX];
                    PC += 2;
                    break;
                }
                case 0x29:{
                    // Sets I to the location of the sprite for the character in VX
                    // each sprite is represented as 5 bytes in memory. Sprite for the char 0 starts at memory 0x0
                    // sprite for char 1 starts at memory location 0x05 ,...
                    I = VF[vX]*5;
                    PC += 2;
                    break;
                }
                case 0x33:{
                    // FX33 From the decimal representation of VX, store the hundreds digit in memory location I,
                    // the tens digit ad I+1 and the ones digit at I+2
                    memory[I] =   (unsigned  char)  VF[vX]/100;
                    memory[I+1] = (unsigned  char) (VF[vX] % 100)/10;
                    memory[I+2] = (unsigned  char)  VF[vX] % 10;
                    PC += 2;
                    break;
                }
                case 0x55:{
                    // FX55 stores V0 to VX in memory starting at address I
                    for(int offset = 0; offset < vX; ++offset){
                        memory[I+offset] = VF[offset];
                    }
                    PC += 2;
                    break;
                }
                case 0x65:{
                    // FX65 saves V0 to VX to memory starting at address I
                    for(int offset = 0; offset < vX; ++offset){
                        VF[offset] = memory[I+offset];
                    }
                    PC += 2;
                    break;
                }
            }
            break;
        }

    }
}

chip8::chip8( unsigned short pc, unsigned short opcode) : PC(pc), opcode(opcode) {
    rng = std::mt19937(dev());
    init();
}

void chip8::init() {
    // copy all the fonts into the beginning of the memory
    std::copy(std::begin(fonts),std::end(fonts),std::begin(memory));
}

void chip8::load_program(std::vector<uint16_t> data) {
    PC = 0x200;
    opcode = 0;
    I = 0;
    int k = 0;
    for(int i = 0x200; i < 0x200+data.size()*2; ++i){
        if(i%2 == 0)
            memory[i] = (data[k]&0xFF00)>>8;
        else {
            memory[i] = (data[k]&0x00FF);
            ++k;
        }

    }
}

