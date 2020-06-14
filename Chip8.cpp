#include "Chip8.h"

#include <math.h>

#include <iostream>
#include <list>
#include <string>

#define X_REG (opcode & 0x0F00) >> 8
#define Y_REG (opcode & 0x00F0) >> 4
using namespace std;
typedef void (Chip8::*MethPointer)();
struct OpcodeMethod {
  string MyOpcode;
  MethPointer MyMeth;
};

unsigned char chip8_fontset[80] = {
    0xF0, 0x90, 0x90, 0x90, 0xF0,  // 0
    0x20, 0x60, 0x20, 0x20, 0x70,  // 1
    0xF0, 0x10, 0xF0, 0x80, 0xF0,  // 2
    0xF0, 0x10, 0xF0, 0x10, 0xF0,  // 3
    0x90, 0x90, 0xF0, 0x10, 0x10,  // 4
    0xF0, 0x80, 0xF0, 0x10, 0xF0,  // 5
    0xF0, 0x80, 0xF0, 0x90, 0xF0,  // 6
    0xF0, 0x10, 0x20, 0x40, 0x40,  // 7
    0xF0, 0x90, 0xF0, 0x90, 0xF0,  // 8
    0xF0, 0x90, 0xF0, 0x10, 0xF0,  // 9
    0xF0, 0x90, 0xF0, 0x90, 0x90,  // A
    0xE0, 0x90, 0xE0, 0x90, 0xE0,  // B
    0xF0, 0x80, 0x80, 0x80, 0xF0,  // C
    0xE0, 0x90, 0x90, 0x90, 0xE0,  // D
    0xF0, 0x80, 0xF0, 0x80, 0xF0,  // E
    0xF0, 0x80, 0xF0, 0x80, 0x80   // F
};

void Chip8::disp_clear() {
  for (int y = 0; y < 32; ++y) {
    for (int x = 0; x < 64; ++x) {
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
void Chip8::go_to() { pc = (opcode & 0x0FFF); }
void Chip8::call_sub() {
  stack[sp] = pc;
  ++sp;
  pc = opcode & 0x0FFF;
}
void Chip8::do_if() {
  if (V[X_REG] == (opcode & 0x00FF)) {
    pc += 2;
  }
  pc += 2;
}
void Chip8::do_negate_if() {
  if (V[X_REG] != (opcode & 0x00FF)) {
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
  } else {
    V[0xF] = 0;
  }
  V[X_REG] = (unsigned char)result;
  pc += 2;
}
void Chip8::vx_minus_vy() {
  int result = V[X_REG] - V[Y_REG];
  if (V[X_REG] < V[Y_REG]) {
    V[0xF] = 0;
  } else {
    V[0xF] = 1;
  }
  V[X_REG] = (unsigned char)result;
  pc += 2;
}
void Chip8::shift_vx_right() {
  V[0xF] = V[X_REG] & 1;
  V[X_REG] >>= 1;
  pc += 2;
}
void Chip8::vy_minus_vx() {
  int result = V[Y_REG] - V[X_REG];
  if (V[X_REG] > V[Y_REG]) {
    V[0xF] = 0;
  } else {
    V[0xF] = 1;
  }
  V[X_REG] = (unsigned char)result;
  pc += 2;
}
void Chip8::shift_vx_left() {
  V[0xF] = V[X_REG] >> 7;
  V[X_REG] <<= 1;
  pc += 2;
}
void Chip8::skip_if_vx_not_vy() {
  if (V[X_REG] != V[Y_REG]) {
    pc += 2;
  }
  pc += 2;
}
void Chip8::set_I() {
  I = opcode & 0x0FFF;
  pc += 2;
}
void Chip8::jump_to_nnnvo() { pc = (opcode & 0x0FFF) + V[0]; }
void Chip8::random() {
  V[X_REG] = (rand() % 256) & (opcode & 0x00FF);
  pc += 2;
}
void Chip8::drawos() {
  unsigned short x = V[X_REG];
  unsigned short y = V[Y_REG];
  unsigned short height = opcode & 0x000F;
  unsigned short pixel;

  V[0xF] = 0;
  for (int yline = 0; yline < height; yline++) {
    pixel = memory[I + yline];
    for (int xline = 0; xline < 8; xline++) {
      if ((pixel & (0x80 >> xline)) != 0) {
        if (pixels[y + yline][x + xline] == 1) {
          V[0xF] = 1;
        }
        pixels[y + yline][x + xline] ^= 1;
      }
    }
  }

  drawFlag = true;
  pc += 2;
}
void Chip8::skip_keyos() {
  if (keys[V[X_REG]] != 0) {
    pc += 2;
  }
  pc += 2;
}
void Chip8::skip_not_key() {
  if (keys[V[X_REG]] == 0) {
    pc += 2;
  }
  pc += 2;
}
void Chip8::get_delayos() {
  V[X_REG] = delay_timer;
  pc += 2;
}
void Chip8::get_keyos() {
  bool keypressed = false;
  for (int i = 0; i < 16; ++i) {
    if (keys[i] == 1) {
      V[X_REG] = i;
      keypressed = true;
    }
  }
  if (!keypressed) {
    return;
  }
  pc += 2;
}
void Chip8::delayos_timer() {
  delay_timer = V[X_REG];
  pc += 2;
}
void Chip8::soundos_timer() {
  sound_timer = V[X_REG];
  pc += 2;
}
void Chip8::add_vx() {
  if ((I + V[X_REG]) > 0xFFF) {  // VF is set to 1 when range overflow
                               // (I+VX>0xFFF), and 0 when there isn't.
    V[0xF] = 1;
  } else {
    V[0xF] = 0;
  }
  I += V[X_REG];
  pc += 2;
}
void Chip8::sprite_adress() {
  I = V[X_REG] * 5;
  pc += 2;
}
void Chip8::set_bcd() {
  memory[I] = (V[X_REG]) / 100;
  memory[I + 1] = (V[X_REG] / 10) % 10;
  memory[I + 2] = (V[X_REG] % 100) % 10;
  pc += 2;
}
void Chip8::reg_dump() {
  for (int i = 0; i <= (X_REG); ++i) {
    memory[I + i] = V[i];
  }
  I += (X_REG) + 1;
  pc += 2;
}
void Chip8::reg_load() {
  for (int i = 0; i <= X_REG; ++i) {
    V[i] = memory[I + i];
  }
  I += (X_REG) + 1;
  pc += 2;
}
int hex2int(char ch) {
  if (ch >= '0' && ch <= '9') return ch - '0';
  if (ch >= 'A' && ch <= 'F') return ch - 'A' + 10;
  if (ch >= 'a' && ch <= 'f') return ch - 'a' + 10;
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
    {"F3", &Chip8::set_bcd},
    {"F51", &Chip8::delayos_timer},
    {"F55", &Chip8::reg_dump},
    {"F56", &Chip8::reg_load},
    {"F7", &Chip8::get_delayos},
    {"F8", &Chip8::soundos_timer},
    {"F9", &Chip8::sprite_adress},
    {"FA", &Chip8::get_keyos},
    {"FE", &Chip8::add_vx},
};
int poradi[] = {3, 0, 1, 2};
bool operator>(const OpcodeMethod left, unsigned short right) {
  int *myptr = poradi;
  for (int i = 0; i < left.MyOpcode.size(); ++i) {
    int rightbyte = (right >> (0x4 * *myptr)) & 0xF;
    int leftbyte = hex2int(left.MyOpcode[i]);
    myptr++;
    if (leftbyte == rightbyte) continue;
    if (leftbyte > rightbyte) return true;
    if (leftbyte < rightbyte) return false;
  }
  return false;
}
bool operator==(const OpcodeMethod left, unsigned short right) {
  int *myptr = poradi;
  for (int i = 0; i < left.MyOpcode.size(); ++i) {
    int rightbyte = (right >> (0x4 * *myptr)) & 0xF;
    int leftbyte = hex2int(left.MyOpcode[i]);
    myptr++;
    if (leftbyte == rightbyte) continue;
    if (leftbyte > rightbyte) return false;
    if (leftbyte < rightbyte) return false;
  }
  return true;
}
int DecodeOpcode(unsigned short myopcode, int l, int r) {
  if (r >= l) {
    int mid = l + (r - l) / 2;

    // If the element is present at the middle
    // itself

    if (AllOpcodes[mid] == myopcode) return mid;

    // If element is smaller than mid, then
    // it can only be present in left subarray
    if (AllOpcodes[mid] > myopcode) return DecodeOpcode(myopcode, l, mid - 1);

    // Else the element can only be present
    // in right subarray
    return DecodeOpcode(myopcode, mid + 1, r);
  }

  // We reach here when element is not
  // present in array
  return -1;
}

void Chip8::initialize() {
  pc = 0x200;  // Program counter starts at 0x200
  opcode = 0;  // Reset current opcode
  I = 0;       // Reset index register
  sp = 0;      // Reset stack pointer
  // clear stack and registers
  for (int i = 0; i < 16; ++i) {
    stack[i] = 0;
    V[i] = 0;
    keys.push_back(0);
  }
  for (int i = 0; i < 4096; ++i) {
    memory[i] = 0;
  }
  // clear pixels
  for (int y = 0; y < 32; ++y) {
    for (int x = 0; x < 64; ++x) {
      pixels[y][x] = 0;
    }
  }
  drawFlag = true;
  delay_timer = 0;
  sound_timer = 0;
  for (int i = 0; i < 80; ++i) {  // load fontset
    memory[i] = chip8_fontset[i];
  }
  srand(time(NULL));
}
void Chip8::emulateCycle() {
  opcode = (memory[pc] << 8) | (memory[pc + 1]);
  int index = DecodeOpcode(opcode, 0, AllOpcodes.size());
  OpcodeMethod myMethod = AllOpcodes[index];
  if (index == -1) {
    cout << "prosim" << endl;
    pc += 2;
    return;
  }
  (this->*myMethod.MyMeth)();
  if (delay_timer > 0) {
    --delay_timer;
  }
  if (sound_timer > 0) {
    --sound_timer;
  }
}
bool Chip8::loadGame(const char *filename) {
  cout << "Loading: " << filename << endl;

  // Open file
  FILE *pFile = fopen(filename, "rb");
  if (pFile == NULL) {
    fputs("File error", stderr);
  }

  // Check file size
  fseek(pFile, 0, SEEK_END);
  long lSize = ftell(pFile);
  rewind(pFile);
  printf("Filesize: %d\n", (int)lSize);

  // Allocate memory to contain the whole file
  char *buffer = (char *)malloc(sizeof(char) * lSize);
  if (buffer == NULL) {
    fputs("Memory error", stderr);
    return false;
  }

  // Copy the file into the buffer
  size_t result = fread(buffer, 1, lSize, pFile);
  if (result != lSize) {
    fputs("Reading error", stderr);
    return false;
  }

  // Copy buffer to Chip8 memory
  if ((4096 - 512) > lSize) {
    for (int i = 0; i < lSize; ++i) memory[i + 512] = buffer[i];
  } else
    printf("Error: ROM too big for memory");

  // Close file, free buffer
  fclose(pFile);
  free(buffer);

  return true;
}