#include "console.h"

CONSOLE_FONT_INFOEX defaultFont;
CONSOLE_SCREEN_BUFFER_INFO defaultScreen;
COORD defaultSize;

DLL_EXPORT
void set_title(const char* c)
{
	printf(OSC "2;%s" BEL, c);
}

DLL_EXPORT void put_char_xy(int x, int y, char c) { put_char(x, y, c); }
DLL_EXPORT void put_char_c(char c) { put_char(c); }

DLL_EXPORT
void put_char(int x, int y, char c, int f, int b)
{
	printf(CSI "%d;%dH" CSI "38;2;%d;%d;%dm" CSI "48;2;%d;%d;%dm%c",
		y + 1,
		x + 1,
		(f & 0xff0000) >> 16,
		(f & 0x00ff00) >> 8,
		(f & 0x0000ff),
		(b & 0xff0000) >> 16,
		(b & 0x00ff00) >> 8,
		(b & 0x0000ff),
		c);
}

void put_char(int x, int y, char c)
{
	printf(CSI "%d;%dH%c", y + 1, x + 1, c);
}

void put_char(char c, int f, int b)
{
	printf(CSI "38;2;%d;%d;%dm" CSI "48;2;%d;%d;%dm%c",
		(f & 0xff0000) >> 16,
		(f & 0x00ff00) >> 8,
		(f & 0x0000ff),
		(b & 0xff0000) >> 16,
		(b & 0x00ff00) >> 8,
		(b & 0x0000ff),
		c);
}

void put_char(char c)
{
	printf("%c", c);
}

DLL_EXPORT void print_s_xy(int x, int y, const char* c) { print_s(x, y, c); }
DLL_EXPORT void print_s_c(const char* c) { print_s(c); }

DLL_EXPORT
void print_s(int x, int y, const char* c, int f, int b)
{
	printf(CSI "%d;%dH" CSI "38;2;%d;%d;%dm" CSI "48;2;%d;%d;%dm%s",
		y + 1,
		x + 1,
		(f & 0xff0000) >> 16,
		(f & 0x00ff00) >> 8,
		(f & 0x0000ff),
		(b & 0xff0000) >> 16,
		(b & 0x00ff00) >> 8,
		(b & 0x0000ff),
		c);
}

void print_s(int x, int y, const char* c)
{
	printf(CSI "%d;%dH%s", y + 1, x + 1, c);
}

void print_s(const char* c)
{
	printf(c);
}

DLL_EXPORT
void set_color(int f, int b)
{
	printf(CSI "38;2;%d;%d;%dm" CSI "48;2;%d;%d;%dm",
		(f & 0xff0000) >> 16,
		(f & 0x00ff00) >> 8,
		(f & 0x0000ff),
		(b & 0xff0000) >> 16,
		(b & 0x00ff00) >> 8,
		(b & 0x0000ff));
}

DLL_EXPORT
void init_scr()
{
	DWORD dwMode = 0;
	GetConsoleMode(GetStdHandle(STD_OUTPUT_HANDLE), &dwMode);

	dwMode |= ENABLE_VIRTUAL_TERMINAL_PROCESSING;
	dwMode |= DISABLE_NEWLINE_AUTO_RETURN;
	SetConsoleMode(GetStdHandle(STD_OUTPUT_HANDLE), dwMode);

	defaultFont.cbSize = sizeof(defaultFont);
	GetCurrentConsoleFontEx(GetStdHandle(STD_OUTPUT_HANDLE), false, &defaultFont);

	GetConsoleScreenBufferInfo(GetStdHandle(STD_OUTPUT_HANDLE), &defaultScreen);
}

DLL_EXPORT
void end_scr()
{
	SetCurrentConsoleFontEx(GetStdHandle(STD_OUTPUT_HANDLE), false, &defaultFont);
	SetConsoleScreenBufferSize(GetStdHandle(STD_OUTPUT_HANDLE), defaultScreen.dwSize);
	SMALL_RECT tmpRect = { 0, 0, 1, 1 };
	SetConsoleWindowInfo(GetStdHandle(STD_OUTPUT_HANDLE), true, &tmpRect);
	SetConsoleWindowInfo(GetStdHandle(STD_OUTPUT_HANDLE), true, &defaultScreen.srWindow);
}

DLL_EXPORT
void set_font(int w, int h, const wchar_t* c)
{
	CONSOLE_FONT_INFOEX font = defaultFont;
	font.dwFontSize.X = (short)w;
	font.dwFontSize.Y = (short)h;
	font.FontWeight = 400;
	font.FontFamily = 0;
	wcscpy_s(font.FaceName, c);
	SetCurrentConsoleFontEx(GetStdHandle(STD_OUTPUT_HANDLE), false, &font);
}

DLL_EXPORT
void set_size(int w, int h)
{
	SMALL_RECT rect = { 0, 0, 1, 1 };
	SetConsoleWindowInfo(GetStdHandle(STD_OUTPUT_HANDLE), TRUE, &rect);

	SetConsoleScreenBufferSize(GetStdHandle(STD_OUTPUT_HANDLE), { (SHORT)w, (SHORT)h });

	rect = { 0, 0, (SHORT)w - 1, (SHORT)h - 1 };
	SetConsoleWindowInfo(GetStdHandle(STD_OUTPUT_HANDLE), TRUE, &rect);
}

DLL_EXPORT
long long get_millis()
{
	using namespace std::chrono;
	return duration_cast<milliseconds>(system_clock::now().time_since_epoch()).count();
}