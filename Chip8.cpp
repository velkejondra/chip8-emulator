#include "Chip8.h"
#include <stdio.h>
void Chip8::disp_clear() {
    for (int i = 0; i < 64 * 32; ++i) {
        pixels[i] = 0;
    }
    drawFlag = true;
    pc+=2;
}
void Chip8::do_return() {
    --sp;
    pc = stack[sp];
    pc+=2;
}
void Chip8::go_to() {
    pc = opcode & 0x0FFF;
}
void Chip8::call_sub() {
    stack[sp] = pc;
    ++sp;
    pc = opcode & 0x0FFF;
}
void Chip8::do_if() {
    if (V[(opcode & 0x0F00)<<12]== opcode & 0x00FF) {
        pc+=2;
    }
    pc+=2;
}
void Chip8::do_negate_if() {
    if (V[(opcode & 0x0F00)<<12]!= opcode & 0x00FF) {
        pc+=2;
    }
    pc+=2;
}
void Chip8::do_equal_if() {
    if (V[(opcode & 0x0F00)<< 12] == V[(opcode & 0x00F0)<<12]) {
        pc += 2;
    }
    pc += 2;
}
void Chip8::set_vx() {
    V[(opcode & 0x0F00) << 12] = 0x00FF & opcode;
    pc += 2;
}
void Chip8::add_vx_to_nn() {
    V[(opcode & 0x0F00) << 12] += 0x00FF & opcode;
    pc += 2;
}
void Chip8::set_vx_to_vy() {
    V[(opcode & 0x0F00) << 12] = V[(opcode & 0x00F0) << 12];
    pc += 2;
}
void Chip8::vx_or_vy() {
    V[(opcode & 0x0F00) << 12] |= V[(opcode & 0x00F0) << 12];
    pc += 2;
}
void Chip8::
void(*functionpointers) = {  };

int Chip8::initialize() {
    pc = 0x200;  // Program counter starts at 0x200
    opcode = 0;      // Reset current opcode	
    I = 0;      // Reset index register
    sp = 0;      // Reset stack pointer
    for (int i = 0; i < 80; ++i) // load fontset
        memory[i] = chip8_fontset[i];

    return 0;
}
int Chip8::emulateCycle() {
    opcode = memory[pc] << 8 | memory[pc + 1];
        

    return 1;
}