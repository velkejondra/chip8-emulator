#include "Chip8.h"

#include <stdio.h>

#define X_REG (chipos->opcode & 0x0F00) >> 8
#define Y_REG (chipos->opcode & 0x00F0) >> 4
typedef void (Chip8::*method_function)();

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

void disp_clear(Chip8 *chipos) {
  for (int y = 0; y < 32; ++y) {
    for(int x = 0; x < 64; ++x){
    chipos->pixels[y][x] = 0;
    }
  }
  chipos->drawFlag = true;
  chipos->pc += 2;
}
void do_return(Chip8 *chipos) {
  --chipos->sp;
  chipos->pc = chipos->stack[chipos->sp];
  chipos->pc += 2;
}
void go_to(Chip8 *chipos) {
  chipos->pc = chipos->opcode & 0x0FFF;
}
void call_sub(Chip8 *chipos) {
  chipos->stack[chipos->sp] = chipos->pc;
  ++chipos->sp;
  chipos->pc = chipos->opcode & 0x0FFF;
}
void do_if(Chip8 *chipos) {
  if (chipos->V[X_REG] == chipos->opcode & 0x00FF) {
    chipos->pc += 2;
  }
  chipos->pc += 2;
}
void do_negate_if(Chip8 *chipos) {
  if (chipos->V[X_REG] != chipos->opcode & 0x00FF) {
    chipos->pc += 2;
  }
  chipos->pc += 2;
}
void do_equal_if(Chip8 *chipos) {
  if (chipos->V[X_REG] == chipos->V[Y_REG]) {
    chipos->pc += 2;
  }
  chipos->pc += 2;
}
void set_vx(Chip8 *chipos) {
  chipos->V[X_REG] = 0x00FF & chipos->opcode;
  chipos->pc += 2;
}
void add_vx_to_nn(Chip8 *chipos) {
  chipos->V[X_REG] += 0x00FF & chipos->opcode;
  chipos->pc += 2;
}
void set_vx_to_vy(Chip8 *chipos) {
  chipos->V[X_REG] = chipos->V[Y_REG];
  chipos->pc += 2;
}
void vx_or_vy(Chip8 *chipos) {
  chipos->V[X_REG] |= chipos->V[Y_REG];
  chipos->pc += 2;
}
void vx_and_vy(Chip8 *chipos) {
  chipos->V[X_REG] &= chipos->V[Y_REG];
  chipos->pc += 2;
}
void vx_xor_vy(Chip8 *chipos) {
  chipos->V[X_REG] ^= chipos->V[Y_REG];
  chipos->pc += 2;
}
void vx_plus_vy(Chip8 *chipos) {
  int result = chipos->V[X_REG] + chipos->V[Y_REG];
  if (result > 0xFF) {
    chipos->V[0xF] = 1;
  }
  chipos->V[X_REG] = (unsigned short) result;
  chipos->pc += 2;
}
void vx_minus_vy(Chip8 *chipos) {
  int result = chipos->V[X_REG] - chipos->V[Y_REG];
  if (chipos->V[X_REG] > chipos->V[Y_REG]) {
    chipos->V[0xF] = 1;
  } else {
    chipos->V[0xF] = 0;
  }
  chipos->V[X_REG] = (unsigned char) result;
  chipos->pc += 2;
}
void shift_vx_right(Chip8 *chipos) {
  bool bit = chipos->V[X_REG] & 1;
  chipos->V[X_REG] = chipos->V[X_REG]>>1;
  chipos->V[0xF] = bit;
  chipos->pc += 2;

}
void vy_minus_vx(Chip8 *chipos){
  bool bit = chipos->V[X_REG] >>7;
  chipos->V[X_REG] = chipos->V[X_REG]<<1;
  chipos->V[0xF] = bit;
  chipos->pc += 2;
}
void shift_vx_left(Chip8 *chipos){
  bool bit = chipos->V[X_REG] >>7;
  chipos->V[X_REG] = chipos->V[X_REG]<<1;
  chipos->V[0xF] = bit;
  chipos->pc += 2;
}
void skip_if_vx_not_vy(Chip8 *chipos){
  if(chipos->V[X_REG] != chipos->V[Y_REG]){
    chipos->pc += 2;
  }
  chipos->pc += 2;
}
void set_I(Chip8 *chipos){
  chipos->I = chipos->opcode & 0x0FFF;
  chipos->pc += 2;
}
void jump_to_nnnvo(Chip8 *chipos){
  chipos->pc = chipos->opcode & 0x0FFF + chipos->V[0];
}
void randomos(Chip8 *chipos){
  chipos->V[X_REG] = (rand() % 256) & (chipos->opcode & 0x00FF);
  chipos->pc += 2;
}
void drawos(Chip8 *chipos){
  int n = chipos->opcode & 0xF;
  int y = Y_REG;
  int x = X_REG;
  unsigned char pixelos;
  chipos->drawFlag = true;
  for(int line = 0; line < n; ++y){
    pixelos = chipos->memory[chipos->I+line];
    for(int curpix = 0; curpix < 8; ++curpix){
      if((pixelos >> curpix) & 1 == 1){
        if(chipos->pixels[y+line][curpix] == 1){
          chipos->V[0xF] = 1;
        }
        chipos->pixels[y+line][curpix] ^=(pixelos >> curpix) & 1;
      }
    }
  }
  chipos->pc += 2;
}
void skip_keyos(Chip8 *chipos){
  if(chipos->keys[chipos->V[X_REG]] != 0){
		chipos->pc += 2;
  }
  chipos->pc += 2;
}
void skip_not_key(Chip8 *chipos){
  if(chipos->keys[chipos->V[X_REG]] == 0){
		chipos->pc += 2;
  }
  chipos->pc += 2;
}
void get_delayos(Chip8 *chipos){
  chipos->V[X_REG] = chipos->delay_timer;
  chipos->pc +=2;
}
void get_keyos(Chip8 *chipos){
  bool keypressed = false;
  for(int i = 0; i < 16; ++i){
    if(chipos->keys[i] == 1){
      chipos->V[X_REG] = i;
      keypressed = true;
    }
  }
  if(!keypressed){
    return;
  }
  chipos->pc +=2;
}
void delayos_timer(Chip8 *chipos){
  chipos->delay_timer = chipos->V[X_REG];
  chipos->pc +=2;
}
void soundos_timer(Chip8 *chipos){
  chipos->sound_timer = chipos->V[X_REG];
  chipos->pc +=2;
}
void add_vx(Chip8 *chipos){
  chipos->I += chipos->V[X_REG];
  chipos->pc += 2;
}
void sprite_adress(Chip8 *chipos){
  chipos->I = chipos->V[X_REG] * 5;
  chipos->pc += 2;
}
void set_bcd(Chip8 *chipos){
  chipos->memory[chipos->I] = (chipos->V[X_REG] & 0xF00)/100;
  chipos->memory[chipos->I+1] = (chipos->V[X_REG] & 0xF0 / 10) % 10;
  chipos->memory[chipos->I+2] = (chipos->V[X_REG] & 0xF0 % 100) % 10;
}
void reg_dump(Chip8 *chipos){
  for(int i = 0; i < X_REG; ++i){
    chipos->memory[chipos->I+i] = chipos->V[i];
  }
  chipos->pc += 2;
}
void reg_load(Chip8 *chipos){
  for(int i = 0; i < X_REG; ++i){
    chipos->V[i] = chipos->memory[chipos->I+i];
  }
  chipos->pc += 2;
}



method_function myptr= {
  
};


void Chip8::initialize() {
  pc = 0x200; // Program counter starts at 0x200
  opcode = 0; // Reset current opcode	
  I = 0; // Reset index register
  sp = 0; // Reset stack pointer
  //clear stack and registers
  for(int i = 0; i < 16; ++i){
    stack[i] = 0;
    V[i] = 0;
    keys[i] = 0;
  }
  for(int i = 0; i < 4096; ++i){
    memory[i] = 0;
  }
  //clear pixels
  for(int y = 0; y < 32; ++y){
    for(int x = 0; x < 64; ++x){
      pixels[y][x] = 0;
    }
  }
  drawFlag = true;
  delay_timer = 0;
  sound_timer = 0;
  for (int i = 0; i < 80; ++i){// load fontset
    memory[i] = chip8_fontset[i];
  }
  
}
void Chip8::emulateCycle() {
  opcode = memory[pc] << 8 | memory[pc + 1];
}
void Chip8::setKey() {
} 