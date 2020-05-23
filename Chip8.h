#pragma once
#include <string>
#include <vector>
using namespace std;
struct ChooseOpcode
{
	int opcode;
	void(*handdle_fun);
	ChooseOpcode() : opcode(0), handdle_fun(NULL) { }
	ChooseOpcode(int code, void(*func)) : opcode(0), handdle_fun(func) { }
};
class Chip8
{
public:
	void initialize();
	void loadGame(string Game);
	void emulateCycle();
	bool drawFlag;
	void setKey();
private:
	unsigned short opcode; // current opcode
	unsigned char memory[4096];
	unsigned char V[16];//cpu registers
	unsigned short I;//index register
	unsigned short pc;//program counter
	unsigned char pixels[32][64];//pixel states
	unsigned char delay_timer;
	unsigned char sound_timer;
	unsigned short stack[16];
	unsigned short sp; //stack pointer
	void (Chip8::* FunctionPointers[17])();
	void (Chip8::* FunctionArith[16])();
	unsigned char keys[16]; //keyboard
	void (*foo)();
	void call_rca();
	void disp_clear();
	void do_return();
	void go_to();
	void call_sub();
	void do_if();
	void do_negate_if();
	void do_equal_if();
	void set_vx();
	void add_vx_to_nn();
	void set_vx_to_vy();
	void vx_or_vy();
	void vx_and_vy();
	void vx_xor_vy();
	void vx_plus_vy();
	void vx_minus_vy();
	void shift_vx_right();
	void vy_minus_vx();
	void shift_vx_left();
	void skip_if_vx_not_vy();
	void set_I();
	void jump_to_nnnvo();
	void random();
	void drawos();
	void skip_keyos();
	void skip_not_key();
	void get_delayos();
	void get_keyos();
	void delayos_timer();
	void soundos_timer();
	void add_vx();
	void sprite_adress();
	void set_bcd();
	void reg_dump();
	void reg_load();
};

