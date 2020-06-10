#define OLC_PGE_APPLICATION
#include "olcPixelGameEngine.h"
#include "Chip8.h"
#include <map>
#define mp make_pair
using namespace std;
Chip8 chip;
vector<pair<olc::Key, int>> olc_to_chip8key = {
	mp(olc::Key::Q, 0x1),
	mp(olc::Key::W, 0x2),
	mp(olc::Key::E, 0x3),
	mp(olc::Key::R, 0xC),
	mp(olc::Key::A, 0x4),
	mp(olc::Key::S, 0x5),
	mp(olc::Key::D, 0x6),
	mp(olc::Key::F, 0xD),
	mp(olc::Key::Z, 0x7),
	mp(olc::Key::X, 0x8),
	mp(olc::Key::C, 0x9),
	mp(olc::Key::V, 0xE),
	mp(olc::Key::K1, 0xA),
	mp(olc::Key::K2, 0x0),
	mp(olc::Key::K3, 0xB),
	mp(olc::Key::K4, 0xF)};

class MainWindow : public olc::PixelGameEngine
{
public:
	MainWindow()
	{
		sAppName = "Chip-8 emulator";
	}
	bool OnUserCreate() override
	{
		chip.initialize();
		chip.loadGame("invaders.c8");
		Clear(olc::DARK_BLUE);
		return true;
	}

	bool OnUserUpdate(float fElapsedTime) override
	{
		
		std::chrono::milliseconds timespan(100); // or whatever
		std::this_thread::sleep_for(timespan);
		chip.emulateCycle();
		cout << "jes" << endl;
		if (chip.drawFlag)
		{
			//draw graphics
			DrawSprite();
		}
		setKeys(chip);
		return true;
	}
	void DrawSprite()
	{
		for (int y = 0; y < 32; ++y)
		{
			for (int x = 0; x < 64; ++x)
			{
				if (chip.pixels[y][x] == 1)
				{
					Draw(x, y, olc::WHITE);
				}
				else
				{
					Draw(x, y, olc::DARK_BLUE);
				}
			}
		}
		chip.drawFlag = false;
		
	}
	void setKeys(Chip8 &chip)
	{
		for (auto i : olc_to_chip8key)
		{
			if (GetKey(i.first).bPressed)
			{
				chip.keys[i.second] = 1;
			}
			else
			{
				chip.keys[i.second] = 0;
			}
		}
	}
};

int main()
{
	MainWindow demo;
	if (demo.Construct(64, 32, 16, 16))
		demo.Start();
	return 0;
}