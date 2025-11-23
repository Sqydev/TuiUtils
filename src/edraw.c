#include "../include/esclib.h"

#include <string.h>


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
