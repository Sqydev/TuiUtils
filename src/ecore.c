#include "../include/esclib.h"
#include "sys/types.h"

#include <signal.h>
#include <stddef.h>
#include <string.h>
#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>



// Setup
#if defined(__APPLE__) || defined(__linux__)

	#include <unistd.h>
	#include <sys/ioctl.h>
	#include <termios.h>
	#include <fcntl.h>
	#include <time.h>

#elif defined(_WIN32) || defined(_WIN64)

	#include <windows.h>
	#include <io.h>

	#define STDOUT_FILENO _fileno(stdout)
	#define STDIN_FILENO _fileno(stdin)
	#define STDERR_FILENO _fileno(stderr)

#endif

typedef struct cell {
	char utf8char[4];
	u_int8_t utfcharlenght;

	char fgSeq[20];
	u_int8_t fgSeqLenght;

	char bgSeq[20];
	u_int8_t bgSeqLenght;
} cell;

typedef struct CoreData {
	cell* backbuffer;
	cell* frontbuffer;

	struct {
		bool shouldClose;

		color bgColor;
		color previousBgColor;
		char bgChar[4];
	} Tui;

	struct {
		// Magical internet var
		volatile sig_atomic_t resized;
		volatile sig_atomic_t exiting;

		#if defined(__APPLE__) || defined(__linux__)

			struct termios defaultSettings;
			struct termios esclibSettings;

		#elif defined(_WIN32) || defined(_WIN64)

			DWORD defaultSettings;
			DWORD esclibSettings;

		#endif

		bool signalsOn;
		bool rawModeOn;
		bool altBuffOn;

		int width;
		int height;

	} Terminal;

	struct {
		vector2 currentPosition;
		intvector2 currentTerminalPosition;
		vector2 lockedPosition;
		bool hidden;
		bool locked;
	} Cursor;

	struct {
		double current;
		double previous;
		double delta;
		double target;
		unsigned int frameCounter;
	} Time;
} CoreData;

CoreData CORE = { 0 };



// TODO: DO ERROR HANDLEING
static void WriteSysCall(int where, const void* what, size_t len) {
	#if defined(__APPLE__) || defined(__linux__)

		write(where, what, len);

	#elif defined(_WIN32) || defined(_WIN64)

		_write(where, what, (unsigned int)len);

	#endif
}

static void EnableRawMode() {
	#if defined(__APPLE__) || defined(__linux__)
	
		struct termios raw;

		tcgetattr(STDIN_FILENO, &CORE.Terminal.defaultSettings);
		raw = CORE.Terminal.defaultSettings;

		raw.c_lflag &= ~(ICANON | ECHO);

		raw.c_iflag &= ~(ICRNL | INLCR);

		raw.c_cc[VMIN] = 0;
		raw.c_cc[VTIME] = 0;

		tcsetattr(STDIN_FILENO, TCSANOW, &raw);

		int flags = fcntl(STDIN_FILENO, F_GETFL, 0);
	
		fcntl(STDIN_FILENO, F_SETFL, flags | O_NONBLOCK);

		CORE.Terminal.esclibSettings = raw;
	
	#elif defined(_WIN32) || defined(_WIN64)
	
		DWORD dwMode = 0;

    	HANDLE hIn = GetStdHandle(STD_INPUT_HANDLE);
    	GetConsoleMode(hIn, &CORE.Terminal.defaultSettings);

		HANDLE hOut = GetStdHandle(STD_OUTPUT_HANDLE);
    	GetConsoleMode(hOut, &dwMode);

    	SetConsoleMode(hOut, dwMode | ENABLE_VIRTUAL_TERMINAL_PROCESSING);

    	DWORD raw = CORE.Terminal.defaultSettings;
		

		raw &= ~(ENABLE_ECHO_INPUT | ENABLE_LINE_INPUT);

		raw |= ENABLE_PROCESSED_INPUT;

    	SetConsoleMode(hIn, raw);

		CORE.Terminal.esclibSettings = raw;
	
	#endif
}

static void DisableRawMode() {
	#if defined(__APPLE__) || defined(__linux__)

		tcsetattr(STDIN_FILENO, TCSANOW, &CORE.Terminal.esclibSettings);

		int flags = fcntl(STDIN_FILENO, F_GETFL, 0);
		fcntl(STDIN_FILENO, F_SETFL, flags & ~O_NONBLOCK);

	#elif defined(_WIN32) || defined(_WIN64)

    	HANDLE hIn = GetStdHandle(STD_INPUT_HANDLE);
		HANDLE hOut = GetStdHandle(STD_OUTPUT_HANDLE);

    	DWORD dwMode = 0;
    	GetConsoleMode(hOut, &dwMode);

    	SetConsoleMode(hIn, CORE.Terminal.settings);
    	SetConsoleMode(hOut, dwMode & ~ENABLE_VIRTUAL_TERMINAL_PROCESSING);

	#endif
}
// TODO: Here also errorhandling(and in height too)
static int GetWidthFr() {
	#if defined(__APPLE__) || defined(__linux__)

		struct winsize sizers;
		ioctl(STDOUT_FILENO, TIOCGWINSZ, &sizers);

		return sizers.ws_col;

	#elif defined(_WIN32) || defined(_WIN64)

		CONSOLE_SCREEN_BUFFER_INFO csbi;
		if (GetConsoleScreenBufferInfo(GetStdHandle(STD_OUTPUT_HANDLE), &csbi)) {
			return csbi.srWindow.Right - csbi.srWindow.Left + 1;
		}

	#endif
}
static int GetHeightFr() {
	#if defined(__APPLE__) || defined(__linux__)

		struct winsize sizers;
		ioctl(STDOUT_FILENO, TIOCGWINSZ, &sizers);

		return sizers.ws_row;

	#elif defined(_WIN32) || defined(_WIN64)

		CONSOLE_SCREEN_BUFFER_INFO csbi;
		GetConsoleScreenBufferInfo(GetStdHandle(STD_OUTPUT_HANDLE), &csbi);
		
		return csbi.srWindow.Bottom - csbi.srWindow.Top + 1;

	#endif
}

static void SIGINTHandler(int signal) {
	// Warning prev
	(void)signal;
	CORE.Terminal.exiting = 1;
}

static void SIGWINCHHandler(int signal) {
	// Warning prev
	(void)signal;
	CORE.Terminal.resized = 1;
}



// TODO: Make signal things for windows and make logic for disableing all sigs besides that one for resizeing by 
void InitTui(int fps, bool ShouldHideCursor, bool DisableStandardSignals) {
	atexit(CloseTui);
	
	// Handlers
	signal(SIGWINCH, SIGWINCHHandler);
	if(!DisableStandardSignals) {
		signal(SIGINT, SIGINTHandler);
		signal(SIGKILL, SIGINTHandler);
		CORE.Terminal.signalsOn = true;
	}
	else {
		// TODO: HERE THAT DISABLEING ALL SIGNALS
		CORE.Terminal.signalsOn = false;
	}

	// Tui
	CORE.Tui.shouldClose = false;

	// Terminal
	CORE.Terminal.exiting = 0;
	CORE.Terminal.resized = 0;
	CORE.Terminal.width = GetWidthFr();
	CORE.Terminal.height = GetHeightFr();

	EnableBufferMode();
	EnableRawMode();

	// Cursor
	SetCursorPosition(0.0f, 0.0f);
	SetLockedCursorPosition(0.0f, 0.0f);
	(ShouldHideCursor) ? HideCursor() : ShowCursor();

	// Time
	CORE.Time.current = GetTime();
	CORE.Time.previous = CORE.Time.current;
	CORE.Time.delta = 0;
	CORE.Time.frameCounter = 0;
	SetTargetFps(fps);

	// INIT BACKBUFFOR
	CORE.backbuffer = calloc(CORE.Terminal.width * CORE.Terminal.height, sizeof(cell));
	CORE.frontbuffer = calloc(CORE.Terminal.width * CORE.Terminal.height, sizeof(cell));
	// TODO: Make alloc error handling here!!!
}

void SetTargetFps(int fps) {
	if(fps < 1) CORE.Time.target = 0.0;
	else CORE.Time.target = 1.0 / (double)fps;
}

void CloseTui(void) {
	DisableRawMode();
	DisableBufferMode();

	CORE.Tui.shouldClose = true;

	// TODO: HERE ADD RECOVERING SIGNAL HANDLER THAT WAS BEFORE INITTUI
}




void BeginDrawing(void) {
	if(CORE.Terminal.exiting == 1) {
		CloseTui();
		exit(0);
	}
	if(CORE.Terminal.resized == 1) {
		CORE.Terminal.width = GetWidthFr();
		CORE.Terminal.height = GetHeightFr();

		// TODO: Do alloc error handling here too(and make it create new one and then make =(to don't lose data when smf is wrg)):
		CORE.backbuffer = realloc(CORE.backbuffer, CORE.Terminal.width * CORE.Terminal.height * sizeof(cell));
		CORE.frontbuffer = realloc(CORE.frontbuffer, CORE.Terminal.width * CORE.Terminal.height * sizeof(cell));

		CORE.Terminal.resized = false;
	}
}

// NOTE: Man, that's a mess, try to didy it up sometime
// TODO:
// NOTE: I've just done chatgbt, make my own version
void EndDrawing(void) {
	int width = CORE.Terminal.width;
	int height = CORE.Terminal.height;
	if(width <= 0 || height <= 0) return;

	// Bufor pomocniczy na sekwencje kursora
	char cursorSeq[32];

	// Przechowuj index ostatnio rysowanego wiersza/kolumny (opcjonalnie)
	// Bierzemy prostą strategię: dla każdej zmiany ustawiamy kursor i wypisujemy bg+fg+char.
	for(int y = 0; y < height; y++) {
		for(int x = 0; x < width; x++) {
			int idx = y * width + x;
			cell *back = &CORE.backbuffer[idx];
			cell *front = &CORE.frontbuffer[idx];

			// Porównanie: jeżeli którekolwiek pole się różni, musimy przepisać
			bool different = false;

			// Porównaj długości i zawartość utf8char
			if(back->utfcharlenght != front->utfcharlenght ||
			   memcmp(back->utf8char, front->utf8char, back->utfcharlenght) != 0) {
				different = true;
			}
			// Porównaj fg seq
			if(!different) {
				if(back->fgSeqLenght != front->fgSeqLenght ||
				   memcmp(back->fgSeq, front->fgSeq, back->fgSeqLenght) != 0) {
					different = true;
				}
			}
			// Porównaj bg seq
			if(!different) {
				if(back->bgSeqLenght != front->bgSeqLenght ||
				   memcmp(back->bgSeq, front->bgSeq, back->bgSeqLenght) != 0) {
					different = true;
				}
			}

			if(different) {
				// Ustaw kursor na (y+1, x+1)
				// sekwencja: \033[<row>;<col>H
				int n = snprintf(cursorSeq, sizeof(cursorSeq), "\033[%d;%dH", y + 1, x + 1);
				if(n > 0) {
					WriteSysCall(STDOUT_FILENO, cursorSeq, (size_t)n);
				}

				// Zapisz kolejno bgSeq, fgSeq, znak
				if(back->bgSeqLenght > 0) {
					WriteSysCall(STDOUT_FILENO, back->bgSeq, back->bgSeqLenght);
				}
				if(back->fgSeqLenght > 0) {
					WriteSysCall(STDOUT_FILENO, back->fgSeq, back->fgSeqLenght);
				}
				if(back->utfcharlenght > 0) {
					WriteSysCall(STDOUT_FILENO, back->utf8char, back->utfcharlenght);
				}

				// Zaktualizuj frontbuffer (kopiujemy cały cell)
				*front = *back;
			}
		}
	}

	// Opcjonalnie: przesunięcie kursora do pozycji kursora biblioteki (CORE.Cursor.currentTerminalPosition)
	// ustawiamy pozycję kursora na bieżącą pozycję (może przydać się dla wejścia)
	{
		int row = CORE.Cursor.currentTerminalPosition.y;
		int col = CORE.Cursor.currentTerminalPosition.x;
		if(row < 1) row = 1;
		if(col < 1) col = 1;
		int n = snprintf(cursorSeq, sizeof(cursorSeq), "\033[%d;%dH", row, col);
		if(n > 0) WriteSysCall(STDOUT_FILENO, cursorSeq, (size_t)n);
	}

	// Aktualizuj czas i delta
	double now = GetTime();
	CORE.Time.previous = CORE.Time.current;
	CORE.Time.current = now;
	CORE.Time.delta = CORE.Time.current - CORE.Time.previous;
	CORE.Time.frameCounter++;

	// Ograniczanie FPS (jeśli target > 0)
	if(CORE.Time.target > 0.0) {
		double frame_time = CORE.Time.delta;
		double need = CORE.Time.target - frame_time;
		if(need > 0.0) {
			// Konwertuj na msec/microsec
		#if defined(__APPLE__) || defined(__linux__)
			struct timespec ts;
			ts.tv_sec = (time_t)need;
			ts.tv_nsec = (long)((need - (time_t)need) * 1e9);
			// nanosleep może być przerwany sygnałem; ignorujemy resztkowy czas
			nanosleep(&ts, NULL);
		#elif defined(_WIN32) || defined(_WIN64)
			// Sleep w ms
			DWORD ms = (DWORD)(need * 1000.0 + 0.5);
			if(ms > 0) Sleep(ms);
		#endif

			// Po spaniu zaktualizuj czasy ponownie
			now = GetTime();
			CORE.Time.previous = CORE.Time.current;
			CORE.Time.current = now;
			CORE.Time.delta = CORE.Time.current - CORE.Time.previous;
		}
	}
}


bool TuiShouldClose(void) {
	return CORE.Tui.shouldClose;
}

bool IsAltBuffOn(void) {
	return CORE.Terminal.altBuffOn;
}

bool IsRawModeOn(void) {
	return CORE.Terminal.rawModeOn;
}




int GetTuiWidth(void) {
	return CORE.Terminal.width;
}

int GetTuiHeight(void) {
	return CORE.Terminal.height;
}

vector2 GetCursorPosition(void);

vector2 GetLockedCursorPosition(void);

int GetKey(void);

double GetTime(void) {
	#if defined(__APPLE__) || defined(__linux__)
	
		struct timespec ts;
    	clock_gettime(CLOCK_MONOTONIC, &ts);

		// Sum 1e9 that I don't know what it does but it's needed(Thanks internet <3)
    	return ts.tv_sec + ts.tv_nsec / 1e9;

	#elif defined(_WIN32) || defined(_WIN64)
	
		static LARGE_INTEGER frequency;
    	static BOOL initialized = FALSE;
    	if (!initialized) {
    	   	QueryPerformanceFrequency(&frequency);
    	    initialized = TRUE;
    	}

    	LARGE_INTEGER counter;
    	QueryPerformanceCounter(&counter);
    	return (double)counter.QuadPart / (double)frequency.QuadPart;

	#endif
}




void SetCursorPosition(float x, float y) {
	if(!CORE.Cursor.locked) {
		CORE.Cursor.currentPosition = (vector2){x, y};
		CORE.Cursor.currentTerminalPosition = (intvector2){x + 1, y + 1};
	}
	else {
		CORE.Cursor.lockedPosition = (vector2){x, y};
	}
}

void SetLockedCursorPosition(float x, float y) {
	CORE.Cursor.lockedPosition = (vector2){x, y};
}

void MoveCursorDirectional(float up, float down, float left, float right);

void MoveCursor(float x, float y);




void HideCursor(void) {
	WriteSysCall(STDOUT_FILENO, "\033[?25l", 6);

	CORE.Cursor.hidden = true;
}

void ShowCursor(void) {
	WriteSysCall(STDOUT_FILENO, "\033[?25h", 6);

	CORE.Cursor.hidden = false;
}

void LockCursor(void) {
	CORE.Cursor.locked = true;
}

void UnlockCursor(void) {
	CORE.Cursor.locked = false;
}




void ClearTuiCharRaw(char character[4], color Color, size_t lenght) {
	CORE.Tui.previousBgColor = CORE.Tui.bgColor;
	CORE.Tui.bgColor = Color;

	char seq[20];
    int p = 0;
	u_int8_t bf;

    seq[p++] = '\033';
    seq[p++] = '[';
    seq[p++] = '4';
    seq[p++] = '8';
    seq[p++] = ';';
    seq[p++] = '2';
    seq[p++] = ';';

    bf = Color.red;
    if(bf >= 100) {
        seq[p++] = '0' + bf / 100;
        seq[p++] = '0' + (bf / 10) % 10;
        seq[p++] = '0' + bf % 10;
    } else if(bf >= 10) {
        seq[p++] = '0' + bf / 10;
        seq[p++] = '0' + bf % 10;
    } else {
        seq[p++] = '0' + bf;
    }

    seq[p++] = ';';

    bf = Color.green;
    if(bf >= 100) {
        seq[p++] = '0' + bf / 100;
        seq[p++] = '0' + (bf / 10) % 10;
        seq[p++] = '0' + bf % 10;
    } else if(bf >= 10) {
        seq[p++] = '0' + bf / 10;
        seq[p++] = '0' + bf % 10;
    } else {
        seq[p++] = '0' + bf;
    }

    seq[p++] = ';';

    bf = Color.blue;
    if(bf >= 100) {
        seq[p++] = '0' + bf / 100;
        seq[p++] = '0' + (bf / 10) % 10;
        seq[p++] = '0' + bf % 10;
    } else if(bf >= 10) {
        seq[p++] = '0' + bf / 10;
        seq[p++] = '0' + bf % 10;
    } else {
        seq[p++] = '0' + bf;
    }

    seq[p++] = 'm';



	for(int i = 0; i < CORE.Terminal.width * CORE.Terminal.height; i++) {
		cell* ptrrer = &CORE.backbuffer[i];

		memcpy(ptrrer->utf8char, character, sizeof(char) * 4);
		ptrrer->utfcharlenght = (u_int8_t)lenght;
 
		memcpy(ptrrer->bgSeq, seq, p);
		ptrrer->bgSeqLenght = p;

		memcpy(ptrrer->fgSeq, "\033[38;2;255;255;255m", 19);
		ptrrer->fgSeqLenght = 19;
	}
}
void ClearTuiChar(char character[4], color Color) {
	size_t len = strnlen(character, 4);
	ClearTuiCharRaw(character, Color, len);
}
void ClearTui(color Color) {
	char character[4] = {' ', '\0', '\0', '\0'};
	ClearTuiCharRaw(character, Color, 1);
}




void WriteToBackBuffor(const char* to_add, size_t lenght);




void EnableBufferMode(void) {
	WriteSysCall(STDOUT_FILENO, "\033[?1049h", 8);
	CORE.Terminal.altBuffOn = true;
}

void DisableBufferMode(void) {
	WriteSysCall(STDOUT_FILENO, "\033[?1049l", 8);
	CORE.Terminal.altBuffOn = false;
}
