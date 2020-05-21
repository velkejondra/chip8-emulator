#include "Chip8.h"

#include <stdio.h>

#define X_REG (opcode & 0x0F00) >> 8
#define Y_REG (opcode & 0x00F0) >> 4

void Chip8::disp_clear() {
  for (int i = 0; i < 64 * 32; ++i) {
    pixels[i] = 0;
  }
  drawFlag = true;
  pc += 2;
}
void Chip8::do_return() {
  --sp;
  pc = stack[sp];
  pc += 2;
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
  if (V[X_REG] == opcode & 0x00FF) {
    pc += 2;
  }
  pc += 2;
}
void Chip8::do_negate_if() {
  if (V[X_REG] != opcode & 0x00FF) {
    pc += 2;
  }
  pc += 2;
}
void Chip8::do_equal_if() {
  if (V[X_REG] == V[Y_REG]) {
    pc += 2;
  }
  pc += 2;
}
void Chip8::set_vx() {
  V[X_REG] = 0x00FF & opcode;
  pc += 2;
}
void Chip8::add_vx_to_nn() {
  V[X_REG] += 0x00FF & opcode;
  pc += 2;
}
void Chip8::set_vx_to_vy() {
  V[X_REG] = V[Y_REG];
  pc += 2;
}
void Chip8::vx_or_vy() {
  V[X_REG] |= V[Y_REG];
  pc += 2;
}
void Chip8::vx_and_vy() {
  V[X_REG] &= V[Y_REG];
  pc += 2;
}
void Chip8::vx_xor_vy() {
  V[X_REG] ^= V[Y_REG];
  pc += 2;
}
void Chip8::vx_plus_vy() {
  int result = V[X_REG] + V[Y_REG];
  if (result > 0xFF) {
    V[0xF] = 1;
  }
  V[X_REG] = (unsigned short) result;
  pc += 2;
}
void Chip8::vx_minus_vy() {
  int result = V[X_REG] - V[Y_REG];
  if (V[X_REG] > V[Y_REG]) {
    V[0xF] = 1;
  } else {
    V[0xF] = 0;
  }
  V[X_REG] = (unsigned char) result;
  pc += 2;
}
void Chip8::shift_vx_right() {
  bool bit = V[X_REG] & 1;
  V[X_REG] = V[X_REG]>>1;
  V[0xF] = bit;
  pc += 2;

}
void Chip8::vy_minus_vx(){
  bool bit = V[X_REG] >>7;
  V[X_REG] = V[X_REG]<<1;
  V[0xF] = bit;
  pc += 2;
}
void Chip8::shift_vx_left(){
  bool bit = V[X_REG] >>7;
  V[X_REG] = V[X_REG]<<1;
  V[0xF] = bit;
  pc += 2;
}
void Chip8::skip_if_vx_not_vy(){
  if(V[X_REG] != V[Y_REG]){
    pc += 2;
  }
  pc += 2;
}
void Chip8::set_I(){
  I = opcode & 0x0FFF;
}
void Chip8::jump_to_nnnvo(){
  pc = opcode & 0x0FFF + V[0];
}
void Chip8::random(){
  V[X_REG] = (rand() % 256) & (opcode & 0x00FF);
  pc += 2;
}
void Chip8::drawos(){
  
}
void( * functionpointers) = {};

int Chip8::initialize() {
  pc = 0x200; // Program counter starts at 0x200
  opcode = 0; // Reset current opcode	
  I = 0; // Reset index register
  sp = 0; // Reset stack pointer
  for (int i = 0; i < 80; ++i) // load fontset
    memory[i] = chip8_fontset[i];

  return 0;
}
int Chip8::emulateCycle() {
  opcode = memory[pc] << 8 | memory[pc + 1];
  return 1;
}

int Chip8::setKey() {
  return 0;
}