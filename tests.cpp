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