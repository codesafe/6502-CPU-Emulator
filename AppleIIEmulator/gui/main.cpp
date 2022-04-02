
#include "Predef.h"
#include "AppleCPU.h"
#include "AppleMem.h"

#include "Apple2Machine.h"
#include "raylib.h"

#include <string>
#include <format>
#include <list>

Apple2Machine appleplus;

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

int fontsize = 10;
void DrawRegistor()
{
	std::string msga = format_string("REG A : %2X", appleplus.cpu.A);
	DrawText(msga.c_str(), 10, 25, fontsize, WHITE);

	std::string msgx = format_string("REG X : %2X", appleplus.cpu.X);
	DrawText(msgx.c_str(), 10, 45, fontsize, WHITE);

	std::string msgy = format_string("REG Y : %2X", appleplus.cpu.Y);
	DrawText(msgy.c_str(), 10, 65, fontsize, WHITE);

	std::string msgpc = format_string("PC : %4X", appleplus.cpu.PC);
	DrawText(msgpc.c_str(), 10, 85, fontsize, WHITE);

	std::string msgsp = format_string("SP : %2X", appleplus.cpu.SP);
	DrawText(msgsp.c_str(), 10, 105, fontsize, WHITE);
}


void DrawFlags()
{
	// FLAG
	std::string flagc = format_string("0 : FLAG C : %2X", appleplus.cpu.Flag.C);
	DrawText(flagc.c_str(), 10, 130, fontsize, MAGENTA);

	std::string flagz = format_string("1 : FLAG Z : %2X", appleplus.cpu.Flag.Z);
	DrawText(flagz.c_str(), 10, 145, fontsize, MAGENTA);

	std::string flagi = format_string("2 : FLAG I : %2X", appleplus.cpu.Flag.I);
	DrawText(flagi.c_str(), 10, 160, fontsize, MAGENTA);

	std::string flagd = format_string("3 : FLAG D : %2X", appleplus.cpu.Flag.D);
	DrawText(flagd.c_str(), 10, 175, fontsize, MAGENTA);

	std::string flagb = format_string("4 : FLAG B : %2X", appleplus.cpu.Flag.B);
	DrawText(flagb.c_str(), 10, 190, fontsize, MAGENTA);

	std::string flagu = format_string("5 : FLAG U : %2X", appleplus.cpu.Flag.Unused);
	DrawText(flagu.c_str(), 10, 205, fontsize, MAGENTA);

	std::string flagv = format_string("6 : FLAG V : %2X", appleplus.cpu.Flag.V);
	DrawText(flagv.c_str(), 10, 220, fontsize, MAGENTA);

	std::string flagn = format_string("7 : FLAG N : %2X", appleplus.cpu.Flag.N);
	DrawText(flagn.c_str(), 10, 235, fontsize, MAGENTA);
}

void DrawZeroPage()
{
	int xpos = 10;
	int ypos = 300;

	for (int y = 0; y < 16; y++)
		for (int x = 0; x < 16; x++)
		{
			int v = appleplus.mem.ReadByte(y*8 + x);
			std::string msg = format_string("%2X", v);
			DrawText(msg.c_str(), xpos + (x * 15), ypos+(y * 15), fontsize, WHITE);
		}
}

void DrawPadinfo()
{
	if (appleplus.device.gamepad.isavailable)
	{
		DrawText("GamePad Detected", 10, 260, fontsize, YELLOW);
	}
	else
		DrawText("NO GamePad", 10, 260, fontsize, YELLOW);
}

std::list<BYTE> opcodestack;
constexpr int maxopcode = 30;
void DrawInstruction()
{
	if (opcodestack.size() >= maxopcode)
		opcodestack.pop_front();

	opcodestack.push_back(appleplus.cpu.lastInst);
	
	std::list<BYTE>::iterator iter;
	int i = 0;
	for (iter = opcodestack.begin(); iter != opcodestack.end(); iter++, i++) 
	{
		DrawText(appleplus.cpu.GetInstName(*iter).c_str(), 10, 330 + (i * 15), fontsize, GREEN);
	}
}

int main(void)
{
	const int windowWidth = 1280;
	const int windowHeight = 1024;

	SetConfigFlags(FLAG_WINDOW_RESIZABLE | FLAG_VSYNC_HINT);
	InitWindow(windowWidth, windowHeight, "APPLE II Plus Emulator");
	//SetWindowMinSize(320, 240);

	SetTargetFPS(TARGET_FRAME);
	SetTraceLogLevel(LOG_ERROR);

	appleplus.InitMachine();

	int frame = 0;
	int dropfilecount = 0;
	char** droppedFiles = { 0 };

	while (!WindowShouldClose())
	{
		if (IsFileDropped())
		{
			droppedFiles = GetDroppedFiles(&dropfilecount);
			printf("Drop file : %s\n", droppedFiles[0]);
			appleplus.FileDroped(droppedFiles[0]);

			ClearDroppedFiles();
		}

		//int fps = GetFPS();
// 		std::string f = format_string("FPS : %d", fps);
// 		DrawText(f.c_str(), 10,300, 20, MAGENTA);
		//long long p = (long long)(1023000.0 / fps);	// 1.023MHz

		long long p = 17050;
		appleplus.Run((int)p);

		BeginDrawing();
		ClearBackground(BLACK);



		bool diskmotoron = appleplus.device.GetDiskMotorState();
		std::string disk = format_string("DISK MOTOR : %s", diskmotoron ? "ON" : "OFF");
		DrawText(disk.c_str(), 10, 280, 10, GREEN);

		DrawRegistor();
		DrawFlags();
		DrawZeroPage();
		//DrawInstruction();
		DrawPadinfo();

		appleplus.Render(frame);

		EndDrawing();

		if (frame++ > TARGET_FRAME) frame = 0;
	}

	CloseWindow();

	return 0;
}
