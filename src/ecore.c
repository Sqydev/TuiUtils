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

		CORE.Terminal.resized = false;
	}
}

// NOTE: Man, that's a mess, try to didy it up sometime
// TODO: Fix it with mind
void EndDrawing(void) {
	// Optim:)
	if(CORE.Terminal.width <= 0 || CORE.Terminal.height <= 0) return;
	if(CORE.backbuffer == NULL) return;

	size_t max_size = (size_t)CORE.Terminal.width * (size_t)CORE.Terminal.height * 64 + 64 + 4 + 29 + 12;
	// TODO: Do SUPER FAV ERR HANDLING :):):):):):):):)
	char* rawbackbuff = malloc(max_size);
	if(!rawbackbuff) return;
	size_t curr_pos = 0;

	const char* homeclear = "\033[2J\033[H";
	// Could be rawbackbuff but i felt the pasiooooonnnnn TO DO THAT!!!!!!!!!!!!!!!!!!
	memcpy(rawbackbuff + curr_pos, homeclear, 7);
	curr_pos += 7;

	// I fucking cant
	memcpy(rawbackbuff + curr_pos, "\033[?25l", 6);
	curr_pos += 6;

	char* currentFg = NULL;
	char* currentBg = NULL;
	size_t currentFgLen = 0;
	size_t currentBgLen = 0;


	// Normaly I WOULD DO i and j but for the sake of my own sanity i've done THE XY!!!!!!!!!!
	for(int y = 0; y < CORE.Terminal.height; y++) {
		for(int x = 0; x < CORE.Terminal.width; x++) {
			cell* workspace = &CORE.backbuffer[y * CORE.Terminal.width + x];
	
			// Super Fg
			if(workspace->fgSeqLenght != currentFgLen || currentFg == NULL || memcmp(workspace->fgSeq, currentFg, workspace->fgSeqLenght) != 0) {
			    if(curr_pos + workspace->fgSeqLenght < max_size) {
			        memcpy(rawbackbuff + curr_pos, workspace->fgSeq, workspace->fgSeqLenght);
			        curr_pos += workspace->fgSeqLenght;

			        currentFg = workspace->fgSeq;
			        currentFgLen = workspace->fgSeqLenght;
			    }
			}

			// Super BG
			if(workspace->bgSeqLenght != currentBgLen || currentBg == NULL || memcmp(workspace->bgSeq, currentBg, workspace->bgSeqLenght) != 0) {
			    if(curr_pos + workspace->bgSeqLenght < max_size) {
			        memcpy(rawbackbuff + curr_pos, workspace->bgSeq, workspace->bgSeqLenght);
			        curr_pos += workspace->bgSeqLenght;

			        currentBg = workspace->bgSeq;
			        currentBgLen = workspace->bgSeqLenght;
			    }
			}

			size_t workspace_len = workspace->utfcharlenght ? workspace->utfcharlenght : 1;
			if(curr_pos + workspace_len >= max_size) continue;
			memcpy(rawbackbuff + curr_pos, workspace->utf8char, workspace_len);
			curr_pos += workspace_len;
		}

		if(curr_pos + 2 < max_size) {
			rawbackbuff[curr_pos++] = '\r';
			rawbackbuff[curr_pos++] = '\n';
		}
	}

	// WHY: Becouse of leaving the terminal
	const char* reset = "\033[0m";
	if(curr_pos + 4 < max_size) {
		memcpy(rawbackbuff + curr_pos, reset, 4);
		curr_pos += 4;
	}

	// It's chatgbt cuz I could do it but I'm lazy and it sould work
	// TODO: Check if it's slop or not
    u_int32_t cx = CORE.Cursor.currentTerminalPosition.x;
    u_int32_t cy = CORE.Cursor.currentTerminalPosition.y;

	char cursorSeq[32];
	int n = snprintf(cursorSeq, sizeof(cursorSeq), "\033[%u;%uH",
                 cy == 0 ? 1 : cy,
                 cx == 0 ? 1 : cx);

	if(n > 0 && curr_pos + (size_t)n + 6 < max_size) {
	    memcpy(rawbackbuff + curr_pos, cursorSeq, n);
	    curr_pos += n;

	    memcpy(rawbackbuff + curr_pos, "\033[?25h", 6);
	    curr_pos += 6;
	}
	
	// WRITY
	WriteSysCall(STDOUT_FILENO, rawbackbuff, curr_pos);

	free(rawbackbuff);



	// LITRAL CTRL+C + CTRL+V FROM BEFORE REWRITE
	CORE.Time.previous = CORE.Time.current;
    CORE.Time.current = GetTime();

	CORE.Time.delta = CORE.Time.current - CORE.Time.previous;

	if(CORE.Time.delta < CORE.Time.target) {
		double sleepTime = (CORE.Time.target - CORE.Time.delta);

		#if defined(__APPLE__) || defined(__linux__)
		
			// TODO: Check if it's better with nanosleep(Do reasearch)
			usleep((useconds_t)(sleepTime * 1000000.0));

		#elif defined(_WIN32) || defined(_WIN64)
		
			Sleep((DWORD)(sleepTime * 1000.0));

		#endif

		CORE.Time.current = GetTime();
        CORE.Time.delta = CORE.Time.current - CORE.Time.previous;
	}

	CORE.Time.frameCounter++;
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
