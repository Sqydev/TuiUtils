#include "../include/esclib.h"
#include "PrivateErrorProtocols.h"

#include <signal.h>
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
	u_int8_t charlenght;

	char fgSeq[20];
	u_int8_t fgSeqLenght;

	char bgSeq[20];
	u_int8_t bgSeqLenght;
} cell;

typedef struct CoreData {
	cell* backbuffer;

	struct {
		bool shouldClose;
	} Tui;

	struct {

		#if defined(__APPLE__) || defined(__linux__)

			struct termios defaultSettings;
			struct termios esclibSettings;

		#elif defined(_WIN32) || defined(_WIN64)

			DWORD defaultSettings;
			DWORD esclibSettings;

		#endif

		bool signalsOn;
		bool altbuffOn;

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
	
}

static void DisableRawMode() {

}

static void SignalThingies(int signal) {
	if(signal == SIGINT) {
		CloseTui();
		exit(0);
	}
}




// TODO: Check if it works on windows
void InitTui(int fps, bool DisableSignals) {
	atexit(CloseTui);
	
	if(!DisableSignals) {
		signal(SIGINT, SignalThingies);
		CORE.Terminal.signalsOn = true;
	}
	else {
		CORE.Terminal.signalsOn = false;
	}

	EnableRawMode();

	// Time
	CORE.Time.current = GetTime();
	CORE.Time.previous = CORE.Time.current;
	CORE.Time.delta = 0;
	CORE.Time.frameCounter = 0;
	SetTargetFps(fps);
}

void CloseTui(void) {
	DisableRawMode();
	DisableBufferMode();
}

void SetTargetFps(int fps) {
	if(fps < 1) CORE.Time.target = 0.0;
	else CORE.Time.target = 1.0 / (double)fps;
}




void BeginDrawing(void) {

}

void EndDrawing(void) {
}




bool TuiShouldClose(void) {

}

bool IsAlternativeBufferOn(void) {

}

bool IsRawModeOn(void) {

}




int GetTuiWidth(void) {

}

int GetTuiHeight(void) {

}

vector2 GetCursorPosition(void) {

}

vector2 GetLockedCursorPosition(void) {

}

color GetBackgroundColor(void) {

}

color GetForegroundColor(void) {

}




void ShowCursor(void) {

}

void HideCursor(void) {

}

void LockCursor(void) {

}

void UnlockCursor(void) {

}




void SetBackgroundColor(color Color) {

}

void SetForegroundColor(color Color) {

}


void ClearBackground(color Color) {

}

void ClearScreen(void) {

}

void ClearLine(void) {

}

void ClearChar(void) {

}


void SetCursorPosition(float x, float y) {

}

void SetLockedCursorPosition(float x, float y) {

}

void MoveCursorDirectional(float up, float down, float left, float right) {

}

void MoveCursor(float x, float y) {

}




int GetKey(void) {
}

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

void WriteToBackBuffor(const char* to_add, size_t lenght) {

}




void EnableBufferMode(void) {
	WriteSysCall(STDOUT_FILENO, "\033[?1049h", 8);
	CORE.Terminal.altbuffOn = true;
}

void DisableBufferMode(void) {
	WriteSysCall(STDOUT_FILENO, "\033[?1049l", 8);
	CORE.Terminal.altbuffOn = false;
}
