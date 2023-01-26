#include "wallClock.h"

void (*wallClock_hook)(int, int, int) = 0;
void oneTickUpdateWallClock(int HH, int MM, int SS){
	if(wallClock_hook) wallClock_hook(HH,MM,SS);
}

void setWallClockHook(void (*func)(int, int, int)) {
	wallClock_hook = func;
}

void setWallClock(int HH,int MM,int SS){
	//你需要填写这里
	char *ptr = (char*)0xB8F00;

	*ptr = (HH/10) +48;
	*(ptr+1) = 0x07;
	*(ptr+2) = (HH%10) +48;
	*(ptr+3) = 0x07;

	*(ptr+4) = ':';
	*(ptr+5) = 0x07;

	*(ptr+6)= (MM/10) +48;
	*(ptr+7) = 0x07;
	*(ptr+8) = (MM%10) +48;
	*(ptr+9) = 0x07;

	*(ptr+10) = ':';
	*(ptr+11) = 0x07;

	*(ptr+12) = (SS/10) +48;
	*(ptr+13) = 0x07;
	*(ptr+14) = (SS%10) +48;
	*(ptr+15) = 0x07;
	return;
}

void getWallClock(int *HH,int *MM,int *SS){
	//你需要填写这里
	char *ptr = (char*)0xB8F00;
	*HH = ((*ptr) -48)*10 + ((*(ptr+2))-48);
	*MM = ((*ptr+6) -48)*10 + ((*(ptr+8))-48);
	*SS = ((*ptr+12) -48)*10 + ((*(ptr+14))-48);
	return;
	
}
