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


#define CATCH_CONFIG_MAIN

#include "catch.h"
#include "chip8.h"

TEST_CASE("opcode 1NNN","[opcodes] [decode]"){
    // Jumps to address NNN.
    unsigned short PC{0};
    unsigned short opcode = 0x1000;
    chip8 ch8(PC,opcode);
    ch8.decode();
    REQUIRE( ch8.PC == 0x0000);
    ch8.opcode = 0x1100;
    ch8.decode();
    REQUIRE( ch8.PC == 0x0100);
    ch8.opcode = 0x1100;
    ch8.decode();
    REQUIRE( ch8.PC == 0x0100);
    ch8.opcode = 0x1110;
    ch8.decode();
    REQUIRE( ch8.PC == 0x0110);
    ch8.opcode = 0x1111;
    ch8.decode();
    REQUIRE( ch8.PC == 0x0111);
    ch8.opcode = 0x1234;
    ch8.decode();
    REQUIRE( ch8.PC == 0x0234);
    ch8.opcode = 0x1F0F;
    ch8.decode();
    REQUIRE( ch8.PC == 0x0F0F);
}

TEST_CASE("opcode 3XNN","[opcodes] [decode]") {
    // Skips the next instruction if VX equals NN
    unsigned short PC{0};
    unsigned short opcode = 0x3000;
    chip8 ch8(PC,opcode);
    ch8.decode();
    REQUIRE( ch8.PC == 4);
    ch8.PC = 0;
    ch8.VF[1] = 1;
    ch8.opcode = 0x3100;
    ch8.decode();
    REQUIRE( ch8.PC == 2);
    ch8.PC = 0;
    ch8.opcode = 0x3101;
    ch8.decode();
    REQUIRE( ch8.PC == 4);
}


TEST_CASE("opcode 4XNN","[opcodes] [decode]") {
    // Skips the next instruction if VX does not NN
    unsigned short PC{0};
    unsigned short opcode = 0x4000;
    chip8 ch8(PC,opcode);
    ch8.decode();
    REQUIRE( ch8.PC == 2);
    ch8.PC = 0;
    ch8.VF[1] = 1;
    ch8.opcode = 0x4100;
    ch8.decode();
    REQUIRE( ch8.PC == 4);
    ch8.PC = 0;
    ch8.opcode = 0x4101;
    ch8.decode();
    REQUIRE( ch8.PC == 2);
}

TEST_CASE("opcode 5XNN","[opcodes] [decode]") {
    // 5XY0 skips the next instruction if VX equals VY
    unsigned short PC{0};
    unsigned short opcode = 0x5000;
    chip8 ch8(PC,opcode);
    ch8.decode();
    REQUIRE( ch8.PC == 4);
    ch8.PC = 0;
    ch8.VF[1] = 1;
    ch8.opcode = 0x5110;
    ch8.decode();
    REQUIRE( ch8.PC == 4);
    ch8.PC = 0;
    ch8.VF[1] = 1;
    ch8.VF[2] = 1;
    ch8.opcode = 0x5120;
    ch8.decode();
    REQUIRE( ch8.PC == 4);
    ch8.PC = 0;
    ch8.VF[1] = 1;
    ch8.VF[3] = 20;
    ch8.opcode = 0x5140;
    ch8.decode();
    REQUIRE( ch8.PC == 2);
}

TEST_CASE("opcode 6XNN","[opcodes] [decode]") {
    // 6XNN set VX to NN
    unsigned short PC{0};
    unsigned short opcode = 0x6000;
    chip8 ch8(PC,opcode);
    ch8.decode();
    REQUIRE( (ch8.PC == 2 && ch8.VF[0] == 0) );
    ch8.PC = 0;
    ch8.opcode = 0x6001;
    ch8.decode();
    REQUIRE( (ch8.PC == 2 && ch8.VF[0] == 1) );
    ch8.PC = 0;
    ch8.opcode = 0x6010;
    ch8.decode();
    REQUIRE( (ch8.PC == 2 && ch8.VF[0] == 0x10) );
    ch8.PC = 0;
    ch8.opcode = 0x641F;
    ch8.decode();
    REQUIRE( (ch8.PC == 2 && ch8.VF[4] == 0x1F) );
}

TEST_CASE("opcode 7XNN", "[opcodes] [decode]"){
    // 7XNN adds NN to VX, carry flag is not changed
    unsigned short PC{0};
    unsigned short opcode = 0x7000;
    chip8 ch8(PC,opcode);
    ch8.decode();
    REQUIRE( (ch8.PC == 2 && ch8.VF[0] == 0x0) );
    ch8.PC = 0;
    ch8.opcode = 0x7005;
    ch8.decode();
    REQUIRE( (ch8.PC == 2 && ch8.VF[0] == 0x5) );
    ch8.PC = 0;
    ch8.opcode = 0x7005;
    ch8.decode();
    REQUIRE( (ch8.PC == 2 && ch8.VF[0] == 0xA) );
    ch8.PC = 0;
    ch8.opcode = 0x7115;
    ch8.decode();
    REQUIRE( (ch8.PC == 2 && ch8.VF[1] == 0x15) );
    ch8.PC = 0;
    ch8.opcode = 0x7101;
    ch8.decode();
    REQUIRE( (ch8.PC == 2 && ch8.VF[1] == 0x16) );
}

TEST_CASE("opcode 8XY0", "[opcodes] [decode]"){
    // 8XY0 sets VX to the value of VY
    unsigned short PC{0};
    unsigned short opcode = 0x8000;
    chip8 ch8(PC,opcode);
    ch8.decode();
    REQUIRE( (ch8.PC == 2 && ch8.VF[0] == 0x0) );
    ch8.PC = 0;
    ch8.opcode = 0x8010;
    ch8.VF[1] = 0x10;
    ch8.decode();
    REQUIRE( (ch8.PC == 2 && ch8.VF[0] == 0x10) );
    ch8.PC = 0;
    ch8.opcode = 0x8120;
    ch8.VF[2] = 0x0F;
    ch8.VF[1] = 0x20;
    ch8.decode();
    REQUIRE( (ch8.PC == 2 && ch8.VF[1] == 0x0F) );
}

TEST_CASE("opcode 8XY1", "[opcodes] [decode]"){
    // 8XY1 sets VX to the value of VX | VY
    unsigned short PC{0};
    unsigned short opcode = 0x8011;
    chip8 ch8(PC,opcode);
    ch8.VF[0] = 0x10;
    ch8.VF[1] = 0x0A;
    ch8.decode();
    REQUIRE( (ch8.PC == 2 && ch8.VF[0] == (0x10 | 0x0A)) );
    ch8.PC = 0;
    ch8.opcode = 0x8001;
    ch8.VF[1] = 0x10;
    ch8.decode();
    REQUIRE( (ch8.PC == 2 && ch8.VF[0] == 0x1A) );
}

TEST_CASE("opcode 8XY2", "[opcodes] [decode]"){
    // 8XY2 sets VX to the value of VX & VY
    unsigned short PC{0};
    unsigned short opcode = 0x8012;
    chip8 ch8(PC,opcode);
    ch8.VF[0] = 0x14;
    ch8.VF[1] = 0x0A;
    ch8.decode();
    REQUIRE( (ch8.PC == 2 && ch8.VF[0] == (0x14 & 0x0A)) );
    ch8.PC = 0;
    ch8.opcode = 0x8002;
    ch8.decode();
    REQUIRE( (ch8.PC == 2 && ch8.VF[0] == 0) );
}

TEST_CASE("opcode 8XY3", "[opcodes] [decode]"){
    // 8XY3 sets VX to the value of VX ^ VY
    unsigned short PC{0};
    unsigned short opcode = 0x8013;
    chip8 ch8(PC,opcode);
    ch8.VF[0] = 0x14;
    ch8.VF[1] = 0x0A;
    ch8.decode();
    REQUIRE( (ch8.PC == 2 && ch8.VF[0] == (0x14 ^ 0x0A)) );
    ch8.PC = 0;
    ch8.opcode = 0x8003;
    ch8.decode();
    REQUIRE( (ch8.PC == 2 && ch8.VF[0] == 0) );
}

TEST_CASE("opcode 8XY4", "[opcodes] [decode]"){
    // 8XY4 adds VY to VX. Set VF to 1 if there's a carry, and to 0 otherwise
    unsigned short PC{0};
    unsigned short opcode = 0x8014;
    chip8 ch8(PC,opcode);
    ch8.VF[0] = 0x0;
    ch8.VF[1] = 0x0A;
    ch8.decode();
    REQUIRE( ch8.PC == 2  );
    REQUIRE(ch8.VF[0] == 0x0A);
    REQUIRE(ch8.VF[0xF] == 0x00);
    ch8.PC = 0;
    ch8.VF[0] = 0xF0;
    ch8.VF[1] = 0x10;
    ch8.decode();
    REQUIRE( ch8.PC == 2  );
    REQUIRE(ch8.VF[0] == 0x0);
    REQUIRE(ch8.VF[0xF] == 0x01);
}

TEST_CASE("opcode 8XY5", "[opcodes] [decode]"){
    // 8XY5 subtracts VY from VX. Set VF to 0 if there's a borrow, and to 1 otherwise
    unsigned short PC{0};
    unsigned short opcode = 0x8015;
    chip8 ch8(PC,opcode);
    ch8.VF[0] = 0x10;
    ch8.VF[1] = 0x0F;
    ch8.decode();
    REQUIRE(ch8.PC == 2);
    REQUIRE(ch8.VF[0] == 0x01);
    REQUIRE(ch8.VF[0xF] == 0x01);
    ch8.VF[0] = 0x00;
    ch8.VF[1] = 0x01;
    ch8.PC = 0;
    ch8.decode();
    REQUIRE(ch8.PC == 2);
    REQUIRE(ch8.VF[0] == (unsigned char)(0x00-0x01));
    REQUIRE(ch8.VF[0xF] == 0x00);
}

TEST_CASE("opcode 8XY6", "[opcodes] [decode]") {
    // 8XY6 Stores the least significant bit of VX in VF, then shifts VX to the right by 1
    unsigned short PC{0};
    unsigned short opcode = 0x8006;
    chip8 ch8(PC, opcode);
    ch8.VF[0] = 0x10;
    ch8.decode();
    REQUIRE(ch8.PC == 2);
    REQUIRE(ch8.VF[0] == 0x08);
    REQUIRE(ch8.VF[0xF] == 0x00);
    ch8.VF[0] = 0x11;
    ch8.PC = 0;
    ch8.decode();
    REQUIRE(ch8.PC == 2);
    REQUIRE(ch8.VF[0] == 0x08);
    REQUIRE(ch8.VF[0xF] == 0x01);
}

TEST_CASE("opcode 8XY7", "[opcodes] [decode]"){
    // 8XY7 sets VX to VY minus VX. VF is set to 0 when there's a borrow, and 1 otherwise
    unsigned short PC{0};
    unsigned short opcode = 0x8017;
    chip8 ch8(PC,opcode);
    ch8.VF[0] = 0x01;
    ch8.VF[1] = 0x03;
    ch8.decode();
    REQUIRE(ch8.PC == 2);
    REQUIRE(ch8.VF[0] == 0x02);
    REQUIRE(ch8.VF[0xF] == 0x01);
    ch8.PC = 0;
    ch8.VF[0] = 0x01;
    ch8.VF[1] = 0x00;
    ch8.decode();
    REQUIRE(ch8.PC == 2);
    REQUIRE(ch8.VF[0] == (unsigned char)(0x00-0x01));
    REQUIRE(ch8.VF[0xF] == 0x00);
}

TEST_CASE("opcode 8XYE", "[opcodes] [decode]"){
    // 8XYE stores the most significant bit of VX in VF, then shifts VX to the left by 1
    unsigned short PC{0};
    unsigned short opcode = 0x800E;
    chip8 ch8(PC,opcode);
    ch8.VF[0] = 0x00;
    ch8.decode();
    REQUIRE(ch8.PC == 2);
    REQUIRE(ch8.VF[0] == 0x00);
    REQUIRE(ch8.VF[0xF] == 0x00);
    ch8.PC = 0;
    ch8.VF[0] = 0x80;
    ch8.decode();
    REQUIRE(ch8.PC == 2);
    REQUIRE(ch8.VF[0] == 0x0);
    REQUIRE(ch8.VF[0xF] == 0x01);
    ch8.PC = 0;
    ch8.VF[0] = 0x81;
    ch8.decode();
    REQUIRE(ch8.PC == 2);
    REQUIRE(ch8.VF[0] == 0x2);
    REQUIRE(ch8.VF[0xF] == 0x01);
}

TEST_CASE("opcode 9XY0", "[opcodes] [decode]"){
    // 9XY0 skips the next instruction if VX != VY
    unsigned short PC{0};
    unsigned short opcode = 0x9010;
    chip8 ch8(PC,opcode);
    ch8.VF[0] = 0x00;
    ch8.VF[1] = 0x00;
    ch8.decode();
    REQUIRE(ch8.PC == 2);
    ch8.PC = 0;
    ch8.VF[0] = 0x01;
    ch8.decode();
    REQUIRE(ch8.PC == 4);
}

TEST_CASE("opcode ANNN", "[opcodes] [decode]"){
    // ANNN sets I to the address NNN
    unsigned short PC{0};
    unsigned short opcode = 0xA123;
    chip8 ch8(PC,opcode);
    ch8.decode();
    REQUIRE(ch8.PC == 2);
    REQUIRE(ch8.I == 0x0123);
}

TEST_CASE("opcode BNNN", "[opcodes] [decode]"){
    // BNNN jumps to the address NNN + V0
    unsigned short PC{0};
    unsigned short opcode = 0xB123;
    chip8 ch8(PC,opcode);
    ch8.VF[0] = 0x00;
    ch8.decode();
    REQUIRE(ch8.PC == 0x123);
    ch8.VF[0] = 0xA0;
    ch8.PC = 0;
    ch8.decode();
    REQUIRE(ch8.PC == 0x1C3);
}

TEST_CASE("opcode EX9E","[opcodes] [decode]"){
    // EX9E skips the next instruction if the key stored in VX is pressed
    unsigned short PC{0};
    unsigned short opcode = 0xE19E;
    chip8 ch8(PC,opcode);
    ch8.VF[1] = 0x0A;
    ch8.decode();
    REQUIRE(ch8.PC == 2);
    ch8.PC = 0;
    ch8.key[0xA] = true;
    ch8.decode();
    REQUIRE(ch8.PC == 4);
}

TEST_CASE("opcode EXA1","[opcodes] [decode]"){
    // EXA1 Skips the next instruction if the key stored in VX isn't pressed
    unsigned short PC{0};
    unsigned short opcode = 0xE1A1;
    chip8 ch8(PC,opcode);
    ch8.VF[1] = 0x0A;
    ch8.decode();
    REQUIRE(ch8.PC == 4);
    ch8.PC = 0;
    ch8.key[0xA] = true;
    ch8.decode();
    REQUIRE(ch8.PC == 2);
}

TEST_CASE("opcode FX07", "[opcodes] [decode]"){
    // FX07 sets VX to the value of the delay timer
    unsigned short PC{0};
    unsigned short opcode = 0xF107;
    chip8 ch8(PC,opcode);
    ch8.delay_timer = 0x23;
    ch8.decode();
    REQUIRE(ch8.PC == 2);
    REQUIRE(ch8.VF[1] == 0x23);
}

TEST_CASE("opcode FX15", "[opcodes] [decode]"){
    // FX15 sets the sound timer to VX
    unsigned short PC{0};
    unsigned short opcode = 0xF115;
    chip8 ch8(PC,opcode);
    ch8.VF[1] = 0x20;
    ch8.decode();
    REQUIRE(ch8.PC == 2);
    REQUIRE(ch8.delay_timer == 0x20);
}

TEST_CASE("opcode FX17", "[opcodes] [decode]"){
    // FX18 sets the sound timer to VX
    unsigned short PC{0};
    unsigned short opcode = 0xF118;
    chip8 ch8(PC,opcode);
    ch8.VF[1] = 0x20;
    ch8.decode();
    REQUIRE(ch8.PC == 2);
    REQUIRE(ch8.sound_timer == 0x20);
}

TEST_CASE("opcode FX1E", "[opcodes] [decode]"){
    // FX1E adds VX to I
    unsigned short PC{0};
    unsigned short opcode = 0xF11E;
    chip8 ch8(PC,opcode);
    ch8.VF[1] = 0x20;
    ch8.decode();
    REQUIRE(ch8.PC == 2);
    REQUIRE(ch8.I == 0x20);
    ch8.PC = 0;
    ch8.decode();
    REQUIRE(ch8.PC == 2);
    REQUIRE(ch8.I == 0x40);
}

TEST_CASE("opcode FX29", "[opcodes] [decode]"){
    // FX29 Sets I to the location of the sprite for the character in VX
    unsigned short PC{0};
    unsigned short opcode = 0xF129;
    chip8 ch8(PC,opcode);
    ch8.VF[1] = 0x0A;
    ch8.decode();
    REQUIRE(ch8.PC == 2);
    REQUIRE(ch8.I == 0x32);
}


TEST_CASE("opcode FX33", "[opcodes] [decode]"){
    // FX33 From the decimal representation of VX, store the hundreds digit in memory location I,
    // the tens digit ad I+1 and the ones digit at I+2
    unsigned short PC{0};
    unsigned short opcode = 0xF133;
    chip8 ch8(PC, opcode);
    ch8.VF[1] = 123;
    ch8.I = 0x200;
    ch8.decode();
    REQUIRE(ch8.PC == 2);
    REQUIRE(ch8.I == 0x200);
    REQUIRE(ch8.memory[0x200] == 1);
    REQUIRE(ch8.memory[0x201] == 2);
    REQUIRE(ch8.memory[0x202] == 3);
    ch8.VF[1] = 23;
    ch8.I = 0x200;
    ch8.PC = 0;
    ch8.decode();
    REQUIRE(ch8.PC == 2);
    REQUIRE(ch8.I == 0x200);
    REQUIRE(ch8.memory[0x200] == 0);
    REQUIRE(ch8.memory[0x201] == 2);
    REQUIRE(ch8.memory[0x202] == 3);
}

TEST_CASE("opcode FX55", "[opcodes] [decode]"){
    // FX55 stores V0 to VX in memory starting at address I
    unsigned short PC{0};
    unsigned short opcode = 0xF455;
    chip8 ch8(PC,opcode);
    ch8.VF[0] = 0x20;
    ch8.VF[1] = 0x21;
    ch8.VF[2] = 0x22;
    ch8.VF[3] = 0x23;
    ch8.I = 0x200;
    ch8.decode();
    REQUIRE(ch8.PC == 2);
    REQUIRE(ch8.I == 0x200);
    REQUIRE(ch8.memory[0x200] == 0x20);
    REQUIRE(ch8.memory[0x201] == 0x21);
    REQUIRE(ch8.memory[0x202] == 0x22);
    REQUIRE(ch8.memory[0x203] == 0x23);
}

TEST_CASE("opcode FX65", "[opcodes] [decode]"){
    // FX65 saves V0 to VX to memory starting at address I
    unsigned short PC{0};
    unsigned short opcode = 0xF465;
    chip8 ch8(PC,opcode);
    ch8.memory[0x200] = 0x20;
    ch8.memory[0x201] = 0x21;
    ch8.memory[0x202] = 0x22;
    ch8.memory[0x203] = 0x23;
    ch8.I = 0x200;
    ch8.decode();
    REQUIRE(ch8.PC == 2);
    REQUIRE(ch8.I == 0x200);
    REQUIRE(ch8.VF[0] == 0x20);
    REQUIRE(ch8.VF[1] == 0x21);
    REQUIRE(ch8.VF[2]  == 0x22);
    REQUIRE(ch8.VF[3]  == 0x23);
}

TEST_CASE("copy sprites"," "){
// Check if the sprites are copied correctly for the different constructors
unsigned short PC{0};
unsigned short opcode = 0x000;
chip8 ch8(PC,opcode);
REQUIRE(ch8.memory[0] == 0xF0);
REQUIRE(ch8.memory[5] == 0x20);
chip8 ch8_2;
REQUIRE(ch8_2.memory[0] == 0xF0);
REQUIRE(ch8_2.memory[5] == 0x20);
}
