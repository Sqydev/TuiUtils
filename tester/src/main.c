#include "../../include/esclib.h"
#include "unistd.h"

#include <stdio.h>
#include <string.h>


void InitTest() {
	InitTui(60, false, false);

	while(1) {

	}

	CloseTui();
}

void GetSizesTest() {
	InitTui(60, false, false);

	int W = GetTuiWidth();
	int H = GetTuiHeight();

	while(!TuiShouldClose()) {
		BeginDrawing();

		int prevW = W;
		int prevH = H;
		W = GetTuiWidth();
		H = GetTuiHeight();

		if(prevW != W || prevH != H) {
			char buf[64];
   			int len = snprintf(buf, sizeof(buf), "Size changed: %d x %d\n", W, H);
    		write(STDOUT_FILENO, buf, len);
		}

		// Around 60 fps
		usleep(16666);
	}

	CloseTui();
}

void ClearBgTest() {
	InitTui(60, false, false);

	while(!TuiShouldClose()) {
		BeginDrawing();

		ClearTui((color){0, 0, 0});

		EndDrawing();
	}

	CloseTui();
}

void EndDrawingStressTest() {
	int In = 0;

	printf("OK, IT'S REALLY EPILEPTIC SO EPISEPCIA WARNING!!!!!!!!!!!!!!!!!\n");
	printf("Input 68 to go TO THE TEST: ");
	scanf("%d", &In);

	if(In == 68) {
		InitTui(60, false, false);

		int qwe = 0;

		while(!TuiShouldClose()) {
			BeginDrawing();

			if(qwe == 0) { ClearTui((color){0, 0, 0}); qwe++; }
			else if(qwe == 1) { ClearTui((color){255, 0, 0}); qwe++; }
			else if(qwe == 2) { ClearTui((color){0, 255, 0}); qwe++; }
			else if(qwe == 3) { ClearTui((color){0, 0, 255}); qwe++; }
			else if(qwe == 4) { ClearTui((color){255, 255, 0}); qwe++; }
			else if(qwe == 5) { ClearTui((color){0, 255, 255}); qwe++; }
			else if(qwe == 6) { ClearTui((color){255, 0, 255}); qwe = 0; }

			EndDrawing();
		}

		CloseTui();
	}
}

// NOTE: Only for linux/mac cuz fuck microsoft
int main(void) {
	int Input = 0;

	printf("1. InitTui test\n");
	printf("2. GetSizes test\n");
	printf("3. ClearBg test\n");
	printf("4. EndDrawingStresstest\n");

	printf("Input: ");
	scanf("%d", &Input);

	if(Input == 1) { InitTest(); }
	else if(Input == 2) { GetSizesTest(); }
	else if(Input == 3) { ClearBgTest(); }
	else if(Input == 4) { EndDrawingStressTest(); }
	
	return 0;
}
