#pragma once

#include <Windows.h>
#include <cstdio>
#include <string>
#include <chrono>

#define DLL_EXPORT extern "C" __declspec(dllexport)

#define ESC "\x1b"
#define CSI "\x1b["
#define OSC "\x1b]"
#define BEL "\x07"

#define USE_ALTERNATE_BUFFER		CSI "?1049h"
#define USE_MAIN_BUFFER				CSI "?1049l"
#define SOFT_RESET					CSI "!p"

#define ENABLE_CURSOR_BLINK			CSI "?12h"
#define DISABLE_CURSOR_BLINK		CSI "?12l"
#define SHOW_CURSOR					CSI "?25h"
#define HIDE_CURSOR					CSI "?25l"
#define SAVE_CURSOR					CSI "s"
#define RESTORE_CURSOR				CSI "u"
#define CURSOR_UP					ESC "A"
#define CURSOR_DOWN					ESC "B"
#define CURSOR_RIGHT				ESC "C"
#define CURSOR_LEFT					ESC "D"
#define CURSOR_NEXT_LINE			CSI "E"
#define CURSOR_PREVIOUS_LINE		CSI "F"
#define CURSOR_UP_N(n)				CSI #n"A"
#define CURSOR_DOWN_N(n)			CSI #n"B"
#define CURSOR_RIGHT_N(n)			CSI #n"C"
#define CURSOR_LEFT_N(n)			CSI #n"D"
#define CURSOR_NEXT_LINE_N(n)		CSI #n"E"
#define CURSOR_PREVIOUS_LINE_N(n)	CSI #n"F"
#define CURSOR_SET_XY(x,y)			CSI + std::to_string(y) + ";" + std::to_string(x) + "H"

#define FORMAT_NONE					CSI "0m"
#define FORMAT_BRIGHT				CSI "1m"
#define FORMAT_UNDERLINE			CSI "4m"
#define FORMAT_NO_UNDERLINE			CSI "24m"
#define FORMAT_NEGATIVE				CSI "7m"
#define FORMAT_NO_NEGATIVE			CSI "27m"

#define RESET_FOREGROUND			CSI "39m"
#define RESET_BACKGROUND			CSI "49m"

#define ENABLE_LINE_DRAWING_MODE	ESC "(0"
#define DISABLE_LINE_DRAWING_MODE	ESC "(B"
#define LINE_BOTTOM_RIGHT				"j"
#define LINE_TOP_RIGHT					"k"
#define LINE_TOP_LEFT					"l"
#define LINE_BOTTOM_LEFT				"m"
#define LINE_MIDDLE						"n"
#define LINE_HORIZONTAL					"q"
#define LINE_MID_LEFT					"t"
#define LINE_MID_RIGHT					"u"
#define LINE_MID_BOTTOM					"v"
#define LINE_MID_TOP					"w"
#define LINE_VERTICAL					"x"

#define CLEAR_SCREEN				CSI "2J"
#define CLEAR_LINE					CSI "K"
#define NEXT_LINE					CSI "E"
#define INSERT_LINES(n)				CSI #n"L"
#define DELETE_LINES(n)				CSI #n"M"
#define SCROLL_UP(n)				CSI #n"S"
#define SCROLL_DOWN(n)				CSI #n"T"


DLL_EXPORT void set_title(const char* c);

DLL_EXPORT void put_char(int x, int y, char c, int f, int b);
void put_char(int x, int y, char c);
void put_char(char c);

DLL_EXPORT void print_s(int x, int y, const char* c, int f, int b);
void print_s(int x, int y, const char* c);
void print_s(const char* c);

DLL_EXPORT void set_color(int f, int b);

#ifdef WIN32
DLL_EXPORT void init_scr();
DLL_EXPORT void end_scr();
DLL_EXPORT void set_font(int w, int h, const wchar_t* c);
DLL_EXPORT void set_size(int w, int h);
#endif

DLL_EXPORT long long get_millis();
