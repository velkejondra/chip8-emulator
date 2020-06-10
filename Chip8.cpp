#include "Chip8.h"
#include <list>

<<<<<<< HEAD
#include <stdio.h>
using namespace std;
#define X_REG (chipos->opcode & 0x0F00) >> 8
#define Y_REG (chipos->opcode & 0x00F0) >> 4
#define MP make_pair
typedef void (Chip8::*method_function)();
=======
#include <iostream>
#include <math.h>

#define X_REG (opcode & 0x0F00) >> 8
#define Y_REG (opcode & 0x00F0) >> 4
>>>>>>> pokus-function-method

using namespace std;
typedef void (Chip8::*MethPointer)();
struct OpcodeMethod
{
  const char* MyOpcode;
  MethPointer MyMeth;
};

<<<<<<< HEAD
void disp_clear(Chip8 *chipos) {
  for (int y = 0; y < 32; ++y) {
    for(int x = 0; x < 64; ++x){
    chipos->pixels[y][x] = 0;
=======
unsigned char chip8_fontset[80] = {
    0xF0, 0x90, 0x90, 0x90, 0xF0, // 0
    0x20, 0x60, 0x20, 0x20, 0x70, // 1
    0xF0, 0x10, 0xF0, 0x80, 0xF0, // 2
    0xF0, 0x10, 0xF0, 0x10, 0xF0, // 3
    0x90, 0x90, 0xF0, 0x10, 0x10, // 4
    0xF0, 0x80, 0xF0, 0x10, 0xF0, // 5
    0xF0, 0x80, 0xF0, 0x90, 0xF0, // 6
    0xF0, 0x10, 0x20, 0x40, 0x40, // 7
    0xF0, 0x90, 0xF0, 0x90, 0xF0, // 8
    0xF0, 0x90, 0xF0, 0x10, 0xF0, // 9
    0xF0, 0x90, 0xF0, 0x90, 0x90, // A
    0xE0, 0x90, 0xE0, 0x90, 0xE0, // B
    0xF0, 0x80, 0x80, 0x80, 0xF0, // C
    0xE0, 0x90, 0x90, 0x90, 0xE0, // D
    0xF0, 0x80, 0xF0, 0x80, 0xF0, // E
    0xF0, 0x80, 0xF0, 0x80, 0x80  // F
};

void Chip8::disp_clear()
{
  for (int y = 0; y < 32; ++y)
  {
    for (int x = 0; x < 64; ++x)
    {
      pixels[y][x] = 0;
>>>>>>> pokus-function-method
    }
  }
  chipos->drawFlag = true;
  chipos->pc += 2;
}
<<<<<<< HEAD
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
=======
void Chip8::do_return()
{
  --sp;
  pc = stack[sp];
  pc += 2;
}
void Chip8::go_to() { pc = opcode & 0x0FFF; }
void Chip8::call_sub()
{
  stack[sp] = pc;
  ++sp;
  pc = opcode & 0x0FFF;
}
void Chip8::do_if()
{
  if (V[X_REG] == opcode & 0x00FF)
  {
    pc += 2;
>>>>>>> pokus-function-method
  }
  chipos->pc += 2;
}
<<<<<<< HEAD
void do_negate_if(Chip8 *chipos) {
  if (chipos->V[X_REG] != chipos->opcode & 0x00FF) {
    chipos->pc += 2;
=======
void Chip8::do_negate_if()
{
  if (V[X_REG] != opcode & 0x00FF)
  {
    pc += 2;
>>>>>>> pokus-function-method
  }
  chipos->pc += 2;
}
<<<<<<< HEAD
void do_equal_if(Chip8 *chipos) {
  if (chipos->V[X_REG] == chipos->V[Y_REG]) {
    chipos->pc += 2;
=======
void Chip8::do_equal_if()
{
  if (V[X_REG] == V[Y_REG])
  {
    pc += 2;
>>>>>>> pokus-function-method
  }
  chipos->pc += 2;
}
<<<<<<< HEAD
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
=======
void Chip8::set_vx()
{
  V[X_REG] = 0x00FF & opcode;
  pc += 2;
}
void Chip8::add_vx_to_nn()
{
  V[X_REG] += 0x00FF & opcode;
  pc += 2;
}
void Chip8::set_vx_to_vy()
{
  V[X_REG] = V[Y_REG];
  pc += 2;
}
void Chip8::vx_or_vy()
{
  V[X_REG] |= V[Y_REG];
  pc += 2;
}
void Chip8::vx_and_vy()
{
  V[X_REG] &= V[Y_REG];
  pc += 2;
}
void Chip8::vx_xor_vy()
{
  V[X_REG] ^= V[Y_REG];
  pc += 2;
}
void Chip8::vx_plus_vy()
{
  int result = V[X_REG] + V[Y_REG];
  if (result > 0xFF)
  {
    V[0xF] = 1;
  }
  V[X_REG] = (unsigned short)result;
  pc += 2;
}
void Chip8::vx_minus_vy()
{
  int result = V[X_REG] - V[Y_REG];
  if (V[X_REG] > V[Y_REG])
  {
    V[0xF] = 1;
  }
  else
  {
    V[0xF] = 0;
  }
  V[X_REG] = (unsigned char)result;
  pc += 2;
}
void Chip8::shift_vx_right()
{
  bool bit = V[X_REG] & 1;
  V[X_REG] = V[X_REG] >> 1;
  V[0xF] = bit;
  pc += 2;
}
void Chip8::vy_minus_vx()
{
  bool bit = V[X_REG] >> 7;
  V[X_REG] = V[X_REG] << 1;
  V[0xF] = bit;
  pc += 2;
}
void Chip8::shift_vx_left()
{
  bool bit = V[X_REG] >> 7;
  V[X_REG] = V[X_REG] << 1;
  V[0xF] = bit;
  pc += 2;
}
void Chip8::skip_if_vx_not_vy()
{
  if (V[X_REG] != V[Y_REG])
  {
    pc += 2;
>>>>>>> pokus-function-method
  }
  chipos->pc += 2;
}
<<<<<<< HEAD
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
=======
void Chip8::set_I()
{
  I = opcode & 0x0FFF;
  pc += 2;
}
void Chip8::jump_to_nnnvo() { pc = opcode & 0x0FFF + V[0]; }
void Chip8::random()
{
  V[X_REG] = (rand() % 256) & (opcode & 0x00FF);
  pc += 2;
}
void Chip8::drawos()
{
  int n = opcode & 0xF;
  int y = Y_REG;
  int x = X_REG;
  unsigned char pixelos;
  drawFlag = true;
  for (int line = 0; line < n; ++line)
  {
    pixelos = memory[I + line];
    for (int curpix = 0; curpix < 8; ++curpix)
    {
      if ((pixelos >> curpix) & 1 == 1)
      {
        if (pixels[y + line][curpix] == 1)
        {
          V[0xF] = 1;
        }
        pixels[y + line][curpix] ^= (pixelos >> curpix) & 1;
>>>>>>> pokus-function-method
      }
    }
  }
  chipos->pc += 2;
}
<<<<<<< HEAD
void skip_keyos(Chip8 *chipos){
  if(chipos->keys[chipos->V[X_REG]] != 0){
		chipos->pc += 2;
=======
void Chip8::skip_keyos()
{
  if (keys[V[X_REG]] != 0)
  {
    pc += 2;
>>>>>>> pokus-function-method
  }
  chipos->pc += 2;
}
<<<<<<< HEAD
void skip_not_key(Chip8 *chipos){
  if(chipos->keys[chipos->V[X_REG]] == 0){
		chipos->pc += 2;
=======
void Chip8::skip_not_key()
{
  if (keys[V[X_REG]] == 0)
  {
    pc += 2;
>>>>>>> pokus-function-method
  }
  chipos->pc += 2;
}
<<<<<<< HEAD
void get_delayos(Chip8 *chipos){
  chipos->V[X_REG] = chipos->delay_timer;
  chipos->pc +=2;
}
void get_keyos(Chip8 *chipos){
  bool keypressed = false;
  for(int i = 0; i < 16; ++i){
    if(chipos->keys[i] == 1){
      chipos->V[X_REG] = i;
=======
void Chip8::get_delayos()
{
  V[X_REG] = delay_timer;
  pc += 2;
}
void Chip8::get_keyos()
{
  bool keypressed = false;
  for (int i = 0; i < 16; ++i)
  {
    if (keys[i] == 1)
    {
      V[X_REG] = i;
>>>>>>> pokus-function-method
      keypressed = true;
    }
  }
  if (!keypressed)
  {
    return;
  }
<<<<<<< HEAD
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
=======
  pc += 2;
}
void Chip8::delayos_timer()
{
  delay_timer = V[X_REG];
  pc += 2;
}
void Chip8::soundos_timer()
{
  sound_timer = V[X_REG];
  pc += 2;
}
void Chip8::add_vx()
{
  I += V[X_REG];
  pc += 2;
}
void Chip8::sprite_adress()
{
  I = V[X_REG] * 5;
  pc += 2;
}
void Chip8::set_bcd()
{
  memory[I] = (V[X_REG] & 0xF00) / 100;
  memory[I + 1] = (V[X_REG] & 0xF0 / 10) % 10;
  memory[I + 2] = (V[X_REG] & 0xF0 % 100) % 10;
}
void Chip8::reg_dump()
{
  for (int i = 0; i < X_REG; ++i)
  {
    memory[I + i] = V[i];
>>>>>>> pokus-function-method
  }
  chipos->pc += 2;
}
<<<<<<< HEAD
void reg_load(Chip8 *chipos){
  for(int i = 0; i < X_REG; ++i){
    chipos->V[i] = chipos->memory[chipos->I+i];
=======
void Chip8::reg_load()
{
  for (int i = 0; i < X_REG; ++i)
  {
    V[i] = memory[I + i];
>>>>>>> pokus-function-method
  }
  chipos->pc += 2;
}
<<<<<<< HEAD
void Chip8Arithmetic(){

}
void (*ArithmeticTable[16]) = {

};
void (*Chip8Table[17]) ={
  Chip8Arithmetic,Chip8Arithmetic,go_to,call_sub,do_if,do_negate_if,do_equal_if,
  set_vx,add_vx_to_nn,Chip8Arithmetic,Chip8Arithmetic,Chip8Arithmetic,Chip8Arithmetic,
  Chip8Arithmetic,Chip8Arithmetic,Chip8Arithmetic,Chip8Arithmetic,skip_if_vx_not_vy,
  set_I,jump_to_nnnvo,randomos,drawos,skip_keyos
}; 
vector<pair<string,void (*)()> func_arr ={
  MP("00",),
  MP("00",),
  MP("00",),
  MP("00",),
  MP("00",),
  MP("00",),
  MP("00",),
  MP("00",),
  MP("00",),
  MP("00",),
  MP("00",),
  MP("00",),
  MP("00",),
  MP("00",),
  MP("00",),
  MP("00",),
  MP("00",),
  MP("00",),
  MP("00",),
  MP("00",),
  MP("00",),
  MP("00",),
  MP("00",),
  MP("00",),
  MP("00",),
  MP("00",),
  MP("00",),
  MP("00",),
  MP("00",),
  MP("00",),
  MP("00",),
  MP("00",),
  MP("00",),
=======
int hex2int(char ch)
{
    if (ch >= '0' && ch <= '9')
        return ch - '0';
    if (ch >= 'A' && ch <= 'F')
        return ch - 'A' + 10;
    if (ch >= 'a' && ch <= 'f')
        return ch - 'a' + 10;
    return -1;
}
vector<OpcodeMethod> AllOpcodes = {
    {"00", &Chip8::disp_clear},
    {"0E", &Chip8::do_return},
    {"1", &Chip8::go_to},
    {"2", &Chip8::call_sub},
    {"3", &Chip8::do_if},
    {"4", &Chip8::do_negate_if},
    {"5", &Chip8::do_equal_if},
    {"6", &Chip8::set_vx},
    {"7", &Chip8::add_vx_to_nn},
    {"80", &Chip8::set_vx_to_vy},
    {"81", &Chip8::vx_or_vy},
    {"82", &Chip8::vx_and_vy},
    {"83", &Chip8::vx_xor_vy},
    {"84", &Chip8::vx_plus_vy},
    {"85", &Chip8::vx_minus_vy},
    {"86", &Chip8::shift_vx_right},
    {"87", &Chip8::vy_minus_vx},
    {"8E", &Chip8::shift_vx_left},
    {"9", &Chip8::skip_if_vx_not_vy},
    {"A", &Chip8::set_I},
    {"B", &Chip8::jump_to_nnnvo},
    {"C", &Chip8::random},
    {"D", &Chip8::drawos},
    {"E1", &Chip8::skip_not_key},
    {"EE", &Chip8::skip_keyos},
    {"F15", &Chip8::delayos_timer},
    {"F3", &Chip8::set_bcd},
    {"F55", &Chip8::reg_dump},
    {"F56", &Chip8::reg_load},
    {"F7", &Chip8::get_delayos},
    {"F8", &Chip8::soundos_timer},
    {"F9", &Chip8::sprite_adress},
    {"FA", &Chip8::get_keyos},
    {"FE", &Chip8::add_vx},
};
int poradi[] = {3, 0, 1, 2};
bool operator>(const OpcodeMethod left, unsigned short right)
{
  int *myptr = poradi;
  for (int i = 0; i > strlen(left.MyOpcode); ++i)
  {
    int rightbyte = (right >> (0x4 * *myptr)) & 0xF;
    int leftbyte = hex2int(left.MyOpcode[*myptr]); 
    myptr++;
    if (leftbyte == rightbyte)
      continue;
    if (leftbyte > rightbyte)
      return true;
    if (leftbyte < rightbyte)
      return false;
  }
  return false;
}
bool operator==(const OpcodeMethod left, unsigned short right)
{
  int *myptr = poradi;
  for (int i = 0; i > strlen(left.MyOpcode); ++i)
  {
    int rightbyte = (right >> (0x4 * *myptr)) & 0xF;
    int leftbyte = hex2int(left.MyOpcode[*myptr]); 
    myptr++;
    if (leftbyte == rightbyte)
      continue;
    if (leftbyte > rightbyte)
      return false;
    if (leftbyte < rightbyte)
      return false;
  }
  return true;
}
int DecodeOpcode(unsigned short myopcode, int l, int r)
{

  if (r >= l)
  {
    int mid = l + (r - l) / 2;

    // If the element is present at the middle
    // itself
    if (myopcode == 61447)
    {
      int test = 0;
    }
    if (AllOpcodes[mid] == myopcode)
      return mid;

    // If element is smaller than mid, then
    // it can only be present in left subarray
    if (AllOpcodes[mid] > myopcode)
      return DecodeOpcode(myopcode, l, mid - 1);

    // Else the element can only be present
    // in right subarray
    return DecodeOpcode(myopcode, mid + 1, r);
  }

  // We reach here when element is not
  // present in array
  return -1;
>>>>>>> pokus-function-method
}

void Chip8::initialize()
{
  pc = 0x200; // Program counter starts at 0x200
  opcode = 0; // Reset current opcode
  I = 0;      // Reset index register
  sp = 0;     // Reset stack pointer
  // clear stack and registers
  for (int i = 0; i < 16; ++i)
  {
    stack[i] = 0;
    V[i] = 0;
    keys[i] = 0;
  }
  for (int i = 0; i < 4096; ++i)
  {
    memory[i] = 0;
  }
  // clear pixels
  for (int y = 0; y < 32; ++y)
  {
    for (int x = 0; x < 64; ++x)
    {
      pixels[y][x] = 0;
    }
  }
  drawFlag = true;
  delay_timer = 0;
  sound_timer = 0;
  for (int i = 0; i < 80; ++i)
  { // load fontset
    memory[i] = chip8_fontset[i];
  }
<<<<<<< HEAD
  
=======
  srand(time(NULL));
>>>>>>> pokus-function-method
}
void Chip8::emulateCycle()
{
  opcode = memory[pc] << 8 | memory[pc + 1];
<<<<<<< HEAD
}
void Chip8::setKey() {
} 
=======
  int index = DecodeOpcode(opcode, 0, AllOpcodes.size());
  OpcodeMethod myMethod = AllOpcodes[index];
  if (index == -1)
  {
    pc += 2;
    return;
  }
  (this->*myMethod.MyMeth)();
}
bool Chip8::loadGame(const char *filename)
{
  cout << "Loading: " << filename << endl;

  // Open file
  FILE *pFile = fopen(filename, "rb");
  if (pFile == NULL)
  {
    fputs("File error", stderr);
  }

  // Check file size
  fseek(pFile, 0, SEEK_END);
  long lSize = ftell(pFile);
  rewind(pFile);
  printf("Filesize: %d\n", (int)lSize);

  // Allocate memory to contain the whole file
  char *buffer = (char *)malloc(sizeof(char) * lSize);
  if (buffer == NULL)
  {
    fputs("Memory error", stderr);
    return false;
  }

  // Copy the file into the buffer
  size_t result = fread(buffer, 1, lSize, pFile);
  if (result != lSize)
  {
    fputs("Reading error", stderr);
    return false;
  }

  // Copy buffer to Chip8 memory
  if ((4096 - 512) > lSize)
  {
    for (int i = 0; i < lSize; ++i)
      memory[i + 512] = buffer[i];
  }
  else
    printf("Error: ROM too big for memory");

  // Close file, free buffer
  fclose(pFile);
  free(buffer);

  return true;
}
>>>>>>> pokus-function-method
