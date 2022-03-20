
#include "../../Predef.h"
#include "../../6502CPU.h"
#include "../../6502Memory.h"
#include "raylib.h"

#include <string>
#include <format>

// 6502 CPU
CPU cpu;
// Memory
Memory mem;

std::string format_string(const std::string fmt, ...) 
{
	int size = ((int)fmt.size()) * 2;
	std::string buffer;
	va_list ap;
	while (1) {
		buffer.resize(size);
		va_start(ap, fmt);
		int n = vsnprintf((char*)buffer.data(), size, fmt.c_str(), ap);
		va_end(ap);
		if (n > -1 && n < size) {
			buffer.resize(n);
			return buffer;
		}
		if (n > -1)
			size = n + 1;
		else
			size *= 2;
	}
	return buffer;
}


void UpLoadProgram()
{
	constexpr int codesize = 65526;
	BYTE* buffer = new BYTE[codesize];

	FILE* fp;
	fopen_s(&fp, "6502_functional_test.bin", "rb");
	fread(buffer, 1, codesize, fp);
	fclose(fp);

	mem.UpLoadProgram(0x000A, buffer, codesize);
	delete[] buffer;
	cpu.PC = 0x400;
}

void DrawRegistor()
{
	std::string msga = format_string("REG A : %2X", cpu.A);
	DrawText(msga.c_str(), 10, 25, 20, WHITE);

	std::string msgx = format_string("REG X : %2X", cpu.X);
	DrawText(msgx.c_str(), 10, 45, 20, WHITE);

	std::string msgy = format_string("REG Y : %2X", cpu.Y);
	DrawText(msgy.c_str(), 10, 65, 20, WHITE);

	std::string msgpc = format_string("PC : %4X", cpu.PC);
	DrawText(msgpc.c_str(), 10, 85, 20, WHITE);

	std::string msgsp = format_string("SP : %2X", cpu.SP);
	DrawText(msgsp.c_str(), 10, 105, 20, WHITE);
}


void DrawFlags()
{
	// FLAG
	std::string flagc = format_string("0 : FLAG C : %2X", cpu.Flag.C);
	DrawText(flagc.c_str(), 10, 130, 20, MAGENTA);

	std::string flagz = format_string("1 : FLAG Z : %2X", cpu.Flag.Z);
	DrawText(flagz.c_str(), 10, 150, 20, MAGENTA);

	std::string flagi = format_string("2 : FLAG I : %2X", cpu.Flag.I);
	DrawText(flagi.c_str(), 10, 170, 20, MAGENTA);

	std::string flagd = format_string("3 : FLAG D : %2X", cpu.Flag.D);
	DrawText(flagd.c_str(), 10, 190, 20, MAGENTA);

	std::string flagb = format_string("4 : FLAG B : %2X", cpu.Flag.B);
	DrawText(flagb.c_str(), 10, 210, 20, MAGENTA);

	std::string flagu = format_string("5 : FLAG U : %2X", cpu.Flag.Unused);
	DrawText(flagu.c_str(), 10, 230, 20, MAGENTA);

	std::string flagv = format_string("6 : FLAG V : %2X", cpu.Flag.V);
	DrawText(flagv.c_str(), 10, 250, 20, MAGENTA);

	std::string flagn = format_string("7 : FLAG N : %2X", cpu.Flag.N);
	DrawText(flagn.c_str(), 10, 270, 20, MAGENTA);
}

void DrawZeroPage()
{
	int xpos = 300;
	int ypos = 10;

	for (int y = 0; y < 16; y++)
		for (int x = 0; x < 16; x++)
		{
			int v = mem[y*8 + x];
			std::string msg = format_string("%2X", v);
			DrawText(msg.c_str(), xpos + (x * 35), ypos+(y*35), 20, WHITE);
		}
}


int main(void)
{
	cpu.Reset();
	mem.Create();
	UpLoadProgram();


	const int windowWidth = 1280;
	const int windowHeight = 1024;

	SetConfigFlags(FLAG_WINDOW_RESIZABLE /*| FLAG_VSYNC_HINT*/);
	InitWindow(windowWidth, windowHeight, "6502 CPU Emulator");
	SetWindowMinSize(320, 240);

	int gameScreenWidth = 1280;
	int gameScreenHeight = 1024;

	//SetTargetFPS(160);

	while (!WindowShouldClose())
	{
		BeginDrawing();
		ClearBackground(BLACK);

		cpu.Run(mem, 1);

		DrawRegistor();
		DrawFlags();
		DrawZeroPage();

		std::string inst = format_string("INSTRUCTION : 0x%2X", cpu.lastInst);
		DrawText(inst.c_str(), 10, 330, 20, GREEN);

		EndDrawing();
	}

	CloseWindow();

	return 0;
}
