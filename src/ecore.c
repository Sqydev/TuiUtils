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
	struct {
		cell* backbuffer;
		int lenght;
	} Backbuffer;

	struct {

	} Tui;

	struct {

		#if defined(__APPLE__) || defined(__linux__)

		#elif defined(_WIN32) || defined(_WIN64)

		#endif

		bool signalsOn;

	} Terminal;

	struct {

	} Cursor;

	struct {

	} Time;
} CoreData;

CoreData CORE = { 0 };




static void WriteSysCall(int where, const void* what, size_t len) {
	#if defined(__APPLE__) || defined(__linux__)

		write(where, what, len);

	#elif defined(_WIN32) || defined(_WIN64)

		_write(where, what, (unsigned int)len);

	#endif
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
		// Here disableing signals at doing raw mode
	}
}

void CloseTui(void) {

}

void SetTargetFps(int fps) {

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




void ToggleBufferMode(void) {

}

void EnableBufferMode(void) {

}

void DisableBufferMode(void) {

}
