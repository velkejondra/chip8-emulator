#include "Chip8.h"

#include <stdio.h>

#define X_REG (opcode & 0x0F00) >> 8
#define Y_REG (opcode & 0x00F0) >> 4

unsigned char chip8_fontset[80] =
{ 
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

void Chip8::disp_clear() {
  for (int y = 0; y < 32; ++y) {
    for(int x = 0; x < 64; ++x){
    pixels[y][x] = 0;
    }
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
  pc += 2;
}
void Chip8::jump_to_nnnvo(){
  pc = opcode & 0x0FFF + V[0];
}
void Chip8::random(){
  V[X_REG] = (rand() % 256) & (opcode & 0x00FF);
  pc += 2;
}
void Chip8::drawos(){
  int n = opcode & 0xF;
  int y = Y_REG;
  int x = X_REG;
  unsigned char pixelos;
  drawFlag = true;
  for(int line = 0; line < n; ++y){
    pixelos = memory[I+line];
    for(int curpix = 0; curpix < 8; ++curpix){
      if((pixelos >> curpix) & 1 == 1){
        if(pixels[y+line][curpix] == 1){
          V[0xF] = 1;
        }
        pixels[y+line][curpix] ^=(pixelos >> curpix) & 1;
      }
    }
  }
  pc += 2;
}
void Chip8::skip_keyos(){
  if(keys[V[X_REG]] != 0){
		pc += 2;
  }
  pc += 2;
}
void Chip8::skip_not_key(){
  if(keys[V[X_REG]] == 0){
		pc += 2;
  }
  pc += 2;
}
void Chip8::get_delayos(){
  V[X_REG] = delay_timer;
  pc +=2;
}
void Chip8::get_keyos(){
  bool keypressed = false;
  for(int i = 0; i < 16; ++i){
    if(keys[i] == 1){
      V[X_REG] = i;
      keypressed = true;
    }
  }
  if(!keypressed){
    return;
  }
  pc +=2;
}
void Chip8::delayos_timer(){
  delay_timer = V[X_REG];
  pc +=2;
}
void Chip8::soundos_timer(){
  sound_timer = V[X_REG];
  pc +=2;
}
void Chip8::add_vx(){
  I += V[X_REG];
  pc += 2;
}
void Chip8::sprite_adress(){
  I = V[X_REG] * 5;
  pc += 2;
}
void Chip8::set_bcd(){
  memory[I] = (V[X_REG] & 0xF00)/100;
  memory[I+1] = (V[X_REG] & 0xF0 / 10) % 10;
  memory[I+2] = (V[X_REG] & 0xF0 % 100) % 10;
}
void Chip8::reg_dump(){
  for(int i = 0; i < X_REG; ++i){
    memory[I+i] = V[i];
  }
  pc += 2;
}
void Chip8::reg_load(){
  for(int i = 0; i < X_REG; ++i){
    V[i] = memory[I+i];
  }
  pc += 2;
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