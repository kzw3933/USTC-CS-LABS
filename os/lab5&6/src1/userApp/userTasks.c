#include "../myOS/userInterface.h"

char *lineMessage = "********************************\n";

void Tsk0(void){      
	myPrintf(0x07, lineMessage);
	myPrintf(0x07, "*     Tsk0: HELLO WORLD!       *\n");
	myPrintf(0x07, lineMessage);
	
	tskEnd();   //the task is end
}

void Tsk1(void){
	myPrintf(0x07, lineMessage);
	myPrintf(0x07, "*     Tsk1: HELLO WORLD!       *\n");
	myPrintf(0x07, lineMessage);
	
	tskEnd();   //the task is end
}

void Tsk2(void){
	myPrintf(0x07, lineMessage);
	myPrintf(0x07, "*     Tsk2: HELLO WORLD!       *\n");
	myPrintf(0x07, lineMessage);
	
	tskEnd();  //the task is end
}