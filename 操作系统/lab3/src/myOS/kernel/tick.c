#include "tick.h"
extern void oneTickUpdateWallClock(int HH, int MM, int SS);

void tick(void){
	//你需要填写这里
	system_ticks++;
	int show_ticks = system_ticks/100;
	SS = show_ticks %60;
	MM = ((show_ticks -SS)/60)%60;
	HH = ((show_ticks-SS-MM*60-SS)/3600)%24;
	oneTickUpdateWallClock(HH, MM, SS);

	return;
}

