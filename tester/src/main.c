#include "../../include/esclib.h"

#include <stdio.h>


void InitTest() {
	InitTui(60, false, false);

	while(1) {

	}

	CloseTui();
}

int main(void) {
	int Input = 0;

	printf("1. InitTui test\n");

	printf("Input: ");
	scanf("%d", &Input);

	if(Input == 1) { InitTest(); }
	else if(Input == 2) { return 0; }
	
	return 0;
}
