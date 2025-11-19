#ifndef ESCLIB_H
#define ESCLIB_H

#include <stdbool.h>
#include <stddef.h>

// NOTE: Order: Structs, macros, itp THEN functions

typedef struct color {
	unsigned char red;
	unsigned char green;
	unsigned char blue;
} color;

typedef struct vector2 {
	float x;
	float y;
} vector2;

typedef struct vector3 {
	float x;
	float y;
	float z;
} vector3;

typedef struct vector4 {
	float x;
	float y;
	float z;
	float w;
} vector4;

typedef struct intvector2 {
	int x;
	int y;
} intvector2;

typedef struct intvector3 {
	int x;
	int y;
	int z;
} intvector3;

typedef struct intvector4 {
	int x;
	int y;
	int z;
	int w;
} intvector4;

//typedef enum {
//} keyid;



// eCore
void InitTui(int fps, bool ShouldHideCursor, bool DisableStandardSignals);
void SetTargetFps(int fps);
void CloseTui(void);

void BeginDrawing(void);
void EndDrawing(void);

bool TuiShouldClose(void);
bool IsAltBuffOn(void);
bool IsRawModeOn(void);

int GetTuiWidth(void);
int GetTuiHeight(void);
vector2 GetCursorPosition(void);
vector2 GetLockedCursorPosition(void);
int GetKey(void);
double GetTime(void);

void SetCursorPosition(float x, float y);
void SetLockedCursorPosition(float x, float y);
void MoveCursorDirectional(float up, float down, float left, float right);
void MoveCursor(float x, float y);

void HideCursor(void);
void ShowCursor(void);
void LockCursor(void);
void UnlockCursor(void);

void ClearBackground(color Color);

// Super extra fns
void WriteToBackBuffor(const char* to_add, size_t lenght);

// Pretty useless fns for normal use
void EnableBufferMode(void);
void DisableBufferMode(void);



// eShapes
void DrawChar(const char character, int posX, int posY, color Color);
void DrawCharV(const char character, vector2 position, color Color);

void DrawTextRaw(const char* text, int posX, int posY, color Color, size_t lenght);
void DrawText(const char* text, int posX, int posY, color Color);
void DrawTextV(const char* text, vector2 position, color Color);

void DrawTextGradientRaw(const char* text, int posX, int posY, color Left, color Right, size_t lenght);
void DrawTextGradient(const char* text, int posX, int posY, color Left, color Right);
void DrawTextGradientV(const char* text, vector2 position, color Left, color Right);

void DrawRectangleRaw(const char character, int posX, int posY, int width, int height, color Color);
void DrawRectangle(int posX, int posY, int width, int height, color Color);
void DrawRectangleV(vector2 position, int width, int height, color Color);

void DrawRectangleGradientVerticalRaw(const char character, int posX, int posY, int width, int height, color Top, color Bottom);
void DrawRectangleGradientVertical(int posX, int posY, int width, int height, color Top, color Bottom);
void DrawRectangleGradientVerticalV(vector2 position, int width, int height, color Top, color Bottom);

void DrawRectangleGradientHorizontalRaw(const char character, int posX, int posY, int width, int height, color Left, color Right);
void DrawRectangleGradientHorizontal(int posX, int posY, int width, int height, color Left, color Right);
void DrawRectangleGradientHorizontalV(vector2 position, int width, int height, color Left, color Right);

#endif
