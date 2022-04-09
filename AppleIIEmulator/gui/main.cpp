
#include "Predef.h"
#include "AppleCPU.h"
#include "AppleMem.h"

#include "Apple2Machine.h"
#include "raylib.h"


#define RAYGUI_IMPLEMENTATION
#include "raygui.h"


#undef RAYGUI_IMPLEMENTATION            // Avoid including raygui implementation again
#define GUI_FILE_DIALOG_IMPLEMENTATION
#include "gui_file_dialog.h"



#include <string>
#include <format>
#include <list>

Apple2Machine appleplus;
int frame = 0;

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

void DrawDiskinfo()
{
	std::string dsk = "DISK 0 :" + appleplus.device.GetDiskName(0);
	DrawText(dsk.c_str(), 10, 600, fontsize, YELLOW);

	DrawText("F1 : RESET", 10, 620, fontsize, GREEN);
	DrawText("F2 : MONO <---> COLOR", 10, 635, fontsize, GREEN);
	DrawText("F3 : ZOOM", 10, 650, fontsize, GREEN);
	DrawText("F4 : MUTE", 10, 665, fontsize, GREEN);

	DrawText("F09 : LOAD ROM (NIB, DSK)", 200, 620, fontsize, YELLOW);
	DrawText("F10 : SAVE SNAPSHOT", 200, 635, fontsize, YELLOW);
	DrawText("F11 : LOAD SNAPSHOT", 200, 650, fontsize, YELLOW);
	DrawText("ESC : QUIT", 200, 665, fontsize, ORANGE);
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


//////////////////////////////////////////////////////////////////////////

void RenderGame()
{
	ClearBackground(BLACK);

	bool diskmotoron = appleplus.device.GetDiskMotorState();
	std::string disk = format_string("DISK MOTOR : %s", diskmotoron ? "ON" : "OFF");
	DrawText(disk.c_str(), 10, 280, 10, GREEN);

	DrawRegistor();
	DrawFlags();
	DrawZeroPage();
	//DrawInstruction();
	DrawPadinfo();
	DrawDiskinfo();

	appleplus.Render(frame);
}

GuiFileDialogState loadDumpDialog;
GuiFileDialogState loadRomDialog;
char savefilename[256] = { 0 };

void LoadDumpFile()
{
	if (loadDumpDialog.SelectFilePressed)
	{
		if (IsFileExtension(loadDumpDialog.fileNameText, ".dmp"))
		{
			char fileNameToLoad[512] = { 0 };
			strcpy(fileNameToLoad, TextFormat("%s/%s", loadDumpDialog.dirPathText, loadDumpDialog.fileNameText));

			appleplus.LoadMachine(fileNameToLoad);

		}

		loadDumpDialog.SelectFilePressed = false;
		appleplus.device.loaddumpmachine = false;
	}

	if (appleplus.device.loaddumpmachine)
	{
		//appleplus.device.loaddumpmachine = false;
		loadDumpDialog.fileDialogActive = true;
	}

	GuiFileDialog(&loadDumpDialog);
}

void SaveDumpFile()
{
	// Dump	
	if (appleplus.device.dumpMachine)
	{
		DrawRectangle(0, 0, GetScreenWidth(), GetScreenHeight(), Fade(RAYWHITE, 0.8f));

		Rectangle rec { (float)(GetScreenWidth()/2-120), (float)(GetScreenHeight()/2-60), 240.f, 140.f };
		int result = GuiTextInputBox(rec, "Save", "Enter Dump File name", "Ok;Cancel", savefilename, 256, NULL);
		if (result == 1)
			appleplus.DumpMachine(savefilename);

		if ((result == 0) || (result == 1) || (result == 2))
			appleplus.device.dumpMachine = false;
	}
}

void LoadRomFile()
{
	if (loadRomDialog.SelectFilePressed)
	{
		if (IsFileExtension(loadRomDialog.fileNameText, ".nib;.dsk"))
		{
			char fileNameToLoad[512] = { 0 };
			strcpy(fileNameToLoad, TextFormat("%s/%s", loadRomDialog.dirPathText, loadRomDialog.fileNameText));
			appleplus.FileDroped(fileNameToLoad);
		}

		loadRomDialog.SelectFilePressed = false;
		appleplus.device.loadromfile = false;
	}

	if (appleplus.device.loadromfile)
	{
		//appleplus.device.loadromfile = false;
		loadRomDialog.fileDialogActive = true;
	}

	GuiFileDialog(&loadRomDialog);
}

void RenderGUIMenu()
{
	LoadRomFile();
	SaveDumpFile();
	LoadDumpFile();

}

int main(void)
{
	const int windowWidth = 1280;
	const int windowHeight = 720;

	SetConfigFlags(/*FLAG_WINDOW_RESIZABLE | */FLAG_VSYNC_HINT);
	InitWindow(windowWidth, windowHeight, "APPLE II+ Emulator");
	//SetWindowMinSize(320, 240);

	SetTargetFPS(TARGET_FRAME);
	SetTraceLogLevel(LOG_ERROR);
	SetExitKey(0);

	appleplus.InitMachine();

	int dropfilecount = 0;
	char** droppedFiles = { 0 };

	bool exitWindow = false;
	bool showMessageBox = false;

	loadDumpDialog = InitGuiFileDialog(-1, 600, GetWorkingDirectory(), false);
	loadRomDialog = InitGuiFileDialog(-1, 600, GetWorkingDirectory(), false);
	//strcpy(fileDialogState.filterExt, ".dmp");

	while (!exitWindow)
	{
		exitWindow = WindowShouldClose();
		if (IsKeyPressed(KEY_ESCAPE)) 
			showMessageBox = !showMessageBox;

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

		if (!appleplus.device.dumpMachine &&
			!appleplus.device.loaddumpmachine &&
			!appleplus.device.loadromfile)
		{
			long long p = 17050;// *1.2f;
			appleplus.Run((int)p);
		}

		BeginDrawing();

		RenderGame();
		RenderGUIMenu();

		if (showMessageBox)
		{
			DrawRectangle(0, 0, GetScreenWidth(), GetScreenHeight(), Fade(RAYWHITE, 0.8f));
			Rectangle rec { (float)(GetScreenWidth()/2-125), (float)(GetScreenHeight()/2-50), 250.f, 100.f };
			int result = GuiMessageBox(rec , GuiIconText(RAYGUI_ICON_EXIT, "Close Window"), "Do you really want to exit?", "Yes;No");
			if ((result == 0) || (result == 2)) 
				showMessageBox = false;
			else 
				if (result == 1) 
					exitWindow = true;
		}

		EndDrawing();

		if (frame++ > TARGET_FRAME) 
			frame = 0;
	}

	CloseWindow();
	return 0;
}
