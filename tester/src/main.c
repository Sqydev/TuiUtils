#include "../../include/esclib.h"

#include <string.h>
#include <unistd.h>
#include <stdio.h>

#define Superfps (1000000 / 60) 

void AltBuffModeSwitchTest() {
}

void ClearTest() {
}

void GetKeyTest() {
}

void FunnyCrsTest() {
}

void BackbufforTest() {
	printf("Adding word of ILOVETHECPROGRAMINGLANGUAGE every half a seccond(and then printing it)(It will print on 0, 0 becouse there's no Init)\n");

	WriteToBackBuffor("I", 1);

	usleep(500000);

	WriteToBackBuffor("LOVE", 4);

	usleep(500000);

	WriteToBackBuffor("THE", 3);

	usleep(500000);

	WriteToBackBuffor("C", 1);

	usleep(500000);

	WriteToBackBuffor("PROGRAMING", 10);

	usleep(500000);

	WriteToBackBuffor("LANGUAGE", 8);

	EndDrawing();

	printf("\nAnd now without double buffor(btw. I've done it with write cuz most terminals make semi double bufforing:) but the tty doesn't :0)\n");

	write(STDOUT_FILENO, "I", 1);
	usleep(500000);
	write(STDOUT_FILENO, "LOVE", 4);
	usleep(500000);
	write(STDOUT_FILENO, "THE", 3);
	usleep(500000);
	write(STDOUT_FILENO, "C", 1);
	usleep(500000);
	write(STDOUT_FILENO, "PROGRAMING", 10);
	usleep(500000);
	write(STDOUT_FILENO, "LANGUAGE", 8);
	usleep(500000);
}

void TuiLoopTest() {

	// BTW. THIS IS ABSOLUTE GARBAGE MADE IN NEED OF TESTING WITHOUT MAKING PROPER TOOLS YET
	// BTW. THIS IS ABSOLUTE GARBAGE MADE IN NEED OF TESTING WITHOUT MAKING PROPER TOOLS YET
	// BTW. THIS IS ABSOLUTE GARBAGE MADE IN NEED OF TESTING WITHOUT MAKING PROPER TOOLS YET
	// BTW. THIS IS ABSOLUTE GARBAGE MADE IN NEED OF TESTING WITHOUT MAKING PROPER TOOLS YET

	int tfps = 0;

	printf("Give targetfps(And btw. This test will do GetKey): ");

	scanf("%d", &tfps);

	InitTui(tfps, false);

	while(!TuiShouldClose()) {
		char key = GetKey();

		BeginDrawing();

		ClearBackground((color){0, 0, 0});

		WriteToBackBuffor(&key, 1);

		if(key == KEY_ESC) { break; }

		EndDrawing();
	}

	CloseTui();
}

void HUHTest() {
	InitTui(60, false);

	while(!TuiShouldClose()) {
		char key = GetKey();

		BeginDrawing();

		ClearBackground((color){0, 0, 0});

		WriteToBackBuffor("  S to show, H to hide", 22);

		if(key == KEY_S) { ShowCursor(); }
		if(key == KEY_H) { HideCursor(); }
		if(key == KEY_ESC) { break; }

		EndDrawing();
	}

	CloseTui();
}

void DrawCharTest() {
	InitTui(60, false);

	while(!TuiShouldClose()) {
		char key = GetKey();

		BeginDrawing();

		ClearBackground((color){0, 0, 0});

		// Old good copy and spam paste and manual things
		DrawCharV('H', (vector2){(float)GetTuiWidth() / 2 - 4, (float)GetTuiHeight() / 2}, STYLE_NORMAL, (color){255, 0, 0});
		DrawCharV('e', (vector2){(float)GetTuiWidth() / 2 - 3, (float)GetTuiHeight() / 2}, STYLE_BOLD, (color){0, 255, 0});
		DrawCharV('L', (vector2){(float)GetTuiWidth() / 2 - 2, (float)GetTuiHeight() / 2}, STYLE_DIMM, (color){0, 0, 255});
		DrawCharV('l', (vector2){(float)GetTuiWidth() / 2 - 1, (float)GetTuiHeight() / 2}, STYLE_ITALIC, (color){0, 255, 255});
		DrawCharV('O', (vector2){(float)GetTuiWidth() / 2    , (float)GetTuiHeight() / 2}, STYLE_UNDERLINE, (color){255, 128, 0});
		DrawCharV('T', (vector2){(float)GetTuiWidth() / 2 + 2, (float)GetTuiHeight() / 2}, STYLE_BACKGROUND, (color){128, 0, 255});
		DrawCharV('u', (vector2){(float)GetTuiWidth() / 2 + 3, (float)GetTuiHeight() / 2}, STYLE_HIDDEN, (color){0, 128, 255});
		DrawCharV('I', (vector2){(float)GetTuiWidth() / 2 + 4, (float)GetTuiHeight() / 2}, STYLE_STRIKETHROUGH, (color){128, 255, 0});


		if(key == KEY_ESC) { break; }

		EndDrawing();
	}

	CloseTui();
}

void DrawTextTest() {
	InitTui(60, false);

	while(!TuiShouldClose()) {
		char key = GetKey();

		BeginDrawing();

		ClearBackground((color){0, 0, 0});

		DrawTextV("Hello Tui", (vector2){(float)GetTuiWidth() / 2 - ((float)strlen("Hello Tui") / 2), (float)GetTuiHeight() / 2}, STYLE_NORMAL, (color){255, 255, 255});

		if(key == KEY_ESC) { break; }

		EndDrawing();
	}

	CloseTui();
}

void DrawTextGradientTest() {
	InitTui(60, false);

	while(!TuiShouldClose()) {
		char key = GetKey();

		BeginDrawing();

		ClearBackground((color){0, 0, 0});

		DrawTextGradientV("Hello Tui", (vector2){(float)GetTuiWidth() / 2 - ((float)strlen("Hello Tui") / 2), (float)GetTuiHeight() / 2}, STYLE_NORMAL, (color){255, 0, 0}, (color){0, 0, 255});
		DrawTextGradientV("                    ", (vector2){(float)GetTuiWidth() / 2 - ((float)strlen("                    ") / 2), (float)GetTuiHeight() / 2 + 2}, STYLE_BACKGROUND, (color){255, 0, 255}, (color){255, 255, 0});

		if(key == KEY_ESC) { break; }

		EndDrawing();
	}

	CloseTui();
}

void DrawRectangleTest() {
	InitTui(1, false);

	while(!TuiShouldClose()) {
		char key = GetKey();

		BeginDrawing();

		ClearBackground((color){0, 0, 0});

		DrawRectangleV((vector2){(float)GetTuiWidth() / 2 - 30, (float)GetTuiHeight() / 2 - 20}, 20, 20, (color){255, 255, 255});

		if(key == KEY_ESC) { break; }

		EndDrawing();
	}

	CloseTui();
}

void DrawRectangleGradientHorizontalTest() {
	InitTui(1, false);

	while(!TuiShouldClose()) {
		char key = GetKey();

		BeginDrawing();

		ClearBackground((color){0, 0, 0});

		DrawRectangleGradientHorizontalV((vector2){(float)GetTuiWidth() / 2 - 30, (float)GetTuiHeight() / 2 - 20}, 20, 20, (color){255, 0, 255}, (color){255, 255, 0});

		if(key == KEY_ESC) { break; }

		EndDrawing();
	}

	CloseTui();
}

int main() {
	int Input = 0;

	printf("1. Backbuffor WriteTo test\n");
	printf("2. TuiLoop test\n");
	printf("3. Hide/UnHide test\n");
	printf("4. DrawChar test\n");
	printf("5. DrawText test\n");
	printf("6. DrawTextGradient test\n");
	printf("7. DrawRectangle test\n");
	printf("8. DrawRectangleGradientHorizontal test\n");

	printf("Input: ");
	scanf("%d", &Input);

	//if(Input == 1) { AltBuffModeSwitchTest(); }
	//else if(Input == 2) { ClearTest(); }
	//else if(Input == 3) { GetKeyTest(); }
	//else if(Input == 4) { FunnyCrsTest(); }
	//else if(Input == 5) { ClearandFillTest(); }
	//else 
	if(Input == 1) { BackbufforTest(); }
	else if(Input == 2) { TuiLoopTest(); }
	else if(Input == 3) { HUHTest(); }
	else if(Input == 4) { DrawCharTest(); }
	else if(Input == 5) { DrawTextTest(); }
	else if(Input == 6) { DrawTextGradientTest(); }
	else if(Input == 7) { DrawRectangleTest(); }
	else if(Input == 8) { DrawRectangleGradientHorizontalTest(); }
	
	return 0;
}
