#define OLC_PGE_APPLICATION
#include "olcPixelGameEngine.h"
#include "Chip8.h"
#include <map>
using namespace std;
Chip8 chip;
map<string,int> key_def = {
	{"q",olc::Key::Q},
	{"w",olc::Key::Q},
	{"e",olc::Key::Q},
	{"r",olc::Key::Q},
	{"a",olc::Key::Q},
	{"s",olc::Key::Q},
	{"d",olc::Key::Q},
	{"f",olc::Key::Q},
	{"z",olc::Key::Q},
	{"x",olc::Key::Q},
	{"c",olc::Key::Q},
	{"v",olc::Key::Q},
	{"1",olc::Key::Q},
	{"2",olc::Key::Q},
	{"3",olc::Key::Q},
	{"4",olc::Key::Q},
}
void setKey(Chip8 &chip){
	olc::Key::H
}
class MainWindow : public olc::PixelGameEngine
{
public:
	MainWindow()
	{
		sAppName = "Chip-8 emulator";
	}
	bool OnUserCreate() override
	{
		Clear(olc::DARK_BLUE);
		return true;
	}

	bool OnUserUpdate(float fElapsedTime) override
	{
		chip.emulateCycle();
		if (chip.drawFlag) {
			//draw graphics
		}
		setKey();
		return true;
	}
	void DrawSprite() {

	}
};

int main()
{
	MainWindow demo;
	if (demo.Construct(320, 200, 2, 2))
		demo.Start();
	return 0;
}