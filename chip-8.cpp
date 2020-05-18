#define OLC_PGE_APPLICATION
#include "olcPixelGameEngine.h"
#include "Chip8.h"
using namespace std;
Chip8 chip;
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
		chip.setKey();
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