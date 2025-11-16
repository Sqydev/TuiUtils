#include "../include/esclib.h"
#include "PrivateErrorProtocols.h"

#include <string.h>

void DrawChar(const char character, int posX, int posY, fontStyle Style, color Color) {
	vector2 cursorPositionBuff = GetCursorPosition();
	color foregroundColorBuff = GetForegroundColor();
	char foregroundStyleBuff = GetForegroundStyle();
	
	SetCursorPosition(posX, posY);

	SetForegroundColor(Color);
	SetForegroundStyle(Style);

	WriteToBackBuffor(&character, 1);

	SetCursorPosition(cursorPositionBuff.x, cursorPositionBuff.y);

	SetForegroundColor(foregroundColorBuff);
	SetForegroundStyle(foregroundStyleBuff);
}
void DrawCharV(const char character, vector2 position, fontStyle Style, color Color) {
	DrawChar(character, position.x, position.y, Style, Color);
}

void DrawTextRaw(const char* text, int posX, int posY, fontStyle Style, color Color, size_t lenght) {
	vector2 cursorPositionBuff = GetCursorPosition();
	color foregroundColorBuff = GetForegroundColor();
	char foregroundStyleBuff = GetForegroundStyle();
	
	SetCursorPosition(posX, posY);

	SetForegroundColor(Color);
	SetForegroundStyle(Style);

	WriteToBackBuffor(text, lenght);

	SetCursorPosition(cursorPositionBuff.x, cursorPositionBuff.y);

	SetForegroundColor(foregroundColorBuff);
	SetForegroundStyle(foregroundStyleBuff);
}
void DrawText(const char* text, int posX, int posY, fontStyle Style, color Color) {
	size_t lenght = strlen(text);
	DrawTextRaw(text, posX, posY, Style, Color, lenght);
}
void DrawTextV(const char* text, vector2 position, fontStyle Style, color Color) {
	DrawText(text, position.x, position.y, Style, Color);
}

void DrawTextGradientRaw(const char* text, int posX, int posY, fontStyle Style, color Left, color Right, size_t lenght) {
	vector2 cursorPositionBuff = GetCursorPosition();
	color foregroundColorBuff = GetForegroundColor();
	char foregroundStyleBuff = GetForegroundStyle();
	
	SetCursorPosition(posX, posY);

	SetForegroundStyle(Style);

	for(int i = 0; i < (int)lenght; i++) {
		// Sum alghorytm from THE internet

		float state_of_journey = (float)i / (float)(lenght - 1);

		color Color;

		Color.red= Left.red + (int)((Right.red- Left.red) * state_of_journey);
        Color.green = Left.green + (int)((Right.green - Left.green) * state_of_journey);
        Color.blue = Left.blue + (int)((Right.blue - Left.blue) * state_of_journey);

		SetForegroundColor(Color);

		WriteToBackBuffor(&text[i], 1);
	}

	SetCursorPosition(cursorPositionBuff.x, cursorPositionBuff.y);

	SetForegroundColor(foregroundColorBuff);
	SetForegroundStyle(foregroundStyleBuff);
}
void DrawTextGradient(const char* text, int posX, int posY, fontStyle Style, color Left, color Right) {
	size_t lenght = strlen(text);
	DrawTextGradientRaw(text, posX, posY, Style, Left, Right, lenght);
}
void DrawTextGradientV(const char* text, vector2 position, fontStyle Style, color Left, color Right) {
	DrawTextGradient(text, position.x, position.y, Style, Left, Right);
}




void DrawRectangleRaw(const char character, int posX, int posY, int width, int height, fontStyle Style, color Color) {
	char filler[width];
	memset(filler, character, sizeof(char) * width);

	for(int i = 0; i < height; i++) {
		DrawText(filler, posX, posY + i, Style, Color);
	}
}
void DrawRectangle(int posX, int posY, int width, int height, color Color) {
	DrawRectangleRaw(' ', posX, posY, width, height, STYLE_BACKGROUND, Color);
}
void DrawRectangleV(vector2 position, int width, int height, color Color) {
	DrawRectangle(position.x, position.y, width, height, Color);
}

//void DrawRectangleGradientVerticalRaw(const char character, int posX, int posY, int width, int height, fontStyle Style, color Top, color Bottom) {

//}
//void DrawRectangleGradientVertical(int posX, int posY, int width, int height, color Top, color Bottom) {

//}
//void DrawRectangleGradientVerticalV(vector2 position, int width, int height, color Top, color Bottom) {

//}

void DrawRectangleGradientHorizontalRaw(const char character, int posX, int posY, int width, int height, fontStyle Style, color Left, color Right) {
	char filler[width];
	memset(filler, character, sizeof(char) * width);

	for(int i = 0; i < height; i++) {
		DrawTextGradientRaw(filler, posX, posY + i, Style, Left, Right, width);
	}
}
void DrawRectangleGradientHorizontal(int posX, int posY, int width, int height, color Left, color Right) {
	DrawRectangleGradientHorizontalRaw(' ', posX, posY, width, height, STYLE_BACKGROUND, Left, Right);
}
void DrawRectangleGradientHorizontalV(vector2 position, int width, int height, color Left, color Right) {
	DrawRectangleGradientHorizontal(position.x, position.y, width, height, Left, Right);
}
