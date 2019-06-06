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
#include <ncurses.h>

void print_registers(WINDOW *win,chip8 *ch8);
void print_memory(WINDOW *win, int current_instruction,chip8 *ch8);


std::vector<u_int16_t> chip8_logo = {
0x00e0, 0x6000, 0x6100, 0x6208, 0xa220, 0x4040, 0x221a, 0x4120,
0x1210, 0xd018, 0xf21e, 0x7008, 0x120a, 0x6000, 0x7108, 0x00ee,
0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000,
0x007f, 0x405f, 0x5057, 0x5454, 0x00fc, 0x04f4, 0x14d4, 0x5454,
0x003f, 0x202f, 0x282b, 0x2a2a, 0x00fe, 0x02fa, 0x0aea, 0x2a2a,
0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000,
0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000,
0x5454, 0x5454, 0x5454, 0x7400, 0x5454, 0x5454, 0x7400, 0x0000,
0x2a2a, 0x2a2a, 0x2a2a, 0x3b00, 0x2a2a, 0x2a2a, 0x2a2a, 0xee00,
0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000,
0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000,
0x7454, 0x5454, 0x5454, 0x5454, 0x0000, 0x7454, 0x5454, 0x5454,
0x3b2a, 0x2a2a, 0x2a2a, 0x2a2a, 0xee2a, 0x2a2a, 0x2a2a, 0x2a2a,
0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000,
0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000,
0x5454, 0x5750, 0x5f40, 0x7f00, 0x5454, 0xd414, 0xf404, 0xfc00,
0x2a2a, 0x2b28, 0x2f20, 0x3f00, 0x2a2a, 0xea0a, 0xfa02, 0xfe00,
0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000};

int main() {
    chip8 ch8;
    ch8.load_program(chip8_logo);

    WINDOW *registers_window;
    WINDOW *memory_window;
    WINDOW *game_window;
    WINDOW *info_window;

    initscr();
    noecho(); // don't output inpit
    cbreak(); // disable terminal line buffering
    curs_set(0); // hide cursor
    start_color();
    init_pair(1,COLOR_WHITE,COLOR_BLACK); // set color pair 1 to black background, white foreground
    init_pair(2,COLOR_RED,COLOR_BLACK);
    refresh();

    game_window = newwin(32,64,0,0);
    wattron(game_window,COLOR_PAIR(2));
    box(game_window,0,0);
    wrefresh(game_window);
    wattroff(game_window,COLOR_PAIR(2));

    memory_window = newwin(32,33,0,65);
    wrefresh(memory_window);
    print_memory(memory_window,ch8.PC,&ch8);

    registers_window = newwin(16,33,33,65);
    wrefresh(registers_window);
    print_registers(registers_window,&ch8);

    info_window = newwin(16,64,33,0);
    wrefresh(info_window);


    int character = 0;
    bool debugging = true;
    while (true){
        ch8.cycle();
        print_registers(registers_window,&ch8);
        print_memory(memory_window,ch8.PC,&ch8);
        mvwprintw(info_window,0,0,"%s",ch8.info_string);
        wrefresh(info_window);
        sprintf(ch8.info_string," "); //clear the infostring

        // step trough program
        if(debugging) {
            character = getch();
            if(character == 27) //quit when pressing ESCAPE
                break;
        }
    }

    endwin();
    return 0;
}

void print_registers(WINDOW *win,chip8 *ch8){
    // print normal registers
    for(int i = 0; i < 16; ++i){
        mvwprintw(win,i,0,"V%d = 0x%X",i,ch8->VF[i]);
    }

    mvwprintw(win,0,15,"DT = 0x%X",ch8->delay_timer);
    mvwprintw(win,1,15,"ST = 0x%X",ch8->sound_timer);
    mvwprintw(win,3,15,"OP = 0x%04X",ch8->opcode);

    /*
    // print second column
    for(int i = 0; i < 12; ++i) {
        mvwprintw(win,i,15,"%s = %d",register_names[i+16],i);
    }
     */
    wrefresh(win);
}

void print_memory(WINDOW *win,int current_instruction,chip8 *ch8){
    int i = std::max(current_instruction-2,0);
    int to = std::min(0x1000,i+64);
    //std::cout << "DATA: " << i << "   " << to << "----" << current_instruction << std::endl;
    int y = 0;
    for(; i < to; i+=2){
        if(i == current_instruction)
            wattron(win,A_REVERSE);
        if(i%2 == 0)
            mvwprintw(win,y,0,"0x%X - 0x%04X      ????",i,(ch8->memory[i]<<8)|ch8->memory[i+1]);
        else
            mvwprintw(win,y,0,"0x%X - 0x%04X       ????",i,(ch8->memory[i]<<8)|ch8->memory[i+1]);
        y++;
        if(i == current_instruction)
            wattroff(win,A_REVERSE);
    }
    wrefresh(win);
}
