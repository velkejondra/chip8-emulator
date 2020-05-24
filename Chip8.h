#pragma once
#include <string>
#include <vector>
using namespace std;

class Chip8
{
public:
	void initialize();
	void loadGame(string Game);
	void emulateCycle();
	bool drawFlag;
	void setKey();
	void decodeOpcode();
public:
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
	unsigned char keys[16]; //keyboard
};
	void call_rca(Chip8 *chipos);
	void disp_clear(Chip8 *chipos);
	void do_return(Chip8 *chipos);
	void go_to(Chip8 *chipos);
	void call_sub(Chip8 *chipos);
	void do_if(Chip8 *chipos);
	void do_negate_if(Chip8 *chipos);
	void do_equal_if(Chip8 *chipos);
	void set_vx(Chip8 *chipos);
	void add_vx_to_nn(Chip8 *chipos);
	void set_vx_to_vy(Chip8 *chipos);
	void vx_or_vy(Chip8 *chipos);
	void vx_and_vy(Chip8 *chipos);
	void vx_xor_vy(Chip8 *chipos);
	void vx_plus_vy(Chip8 *chipos);
	void vx_minus_vy(Chip8 *chipos);
	void shift_vx_right(Chip8 *chipos);
	void vy_minus_vx(Chip8 *chipos);
	void shift_vx_left(Chip8 *chipos);
	void skip_if_vx_not_vy(Chip8 *chipos);
	void set_I(Chip8 *chipos);
	void jump_to_nnnvo(Chip8 *chipos);
	void randomos(Chip8 *chipos);
	void drawos(Chip8 *chipos);
	void skip_keyos(Chip8 *chipos);
	void skip_not_key(Chip8 *chipos);
	void get_delayos(Chip8 *chipos);
	void get_keyos(Chip8 *chipos);
	void delayos_timer(Chip8 *chipos);
	void soundos_timer(Chip8 *chipos);
	void add_vx(Chip8 *chipos);
	void sprite_adress(Chip8 *chipos);
	void set_bcd(Chip8 *chipos);
	void reg_dump(Chip8 *chipos);
	void reg_load(Chip8 *chipos);

