#include "../myOS/userInterface.h"   //interface from kernel

#include "shell.h"
#include "memTestCase.h"
#include "userTasks.h"

void wallClock_hook_main(void){
	int _h, _m, _s;
	char hhmmss[]="hh:mm:ss\0\0\0\0";

	getWallClock(&_h,&_m,&_s);
	sprintf(hhmmss,"%02d:%02d:%02d",_h,_m,_s);
	put_chars(hhmmss,0x7E,24,72);
}

void doSomeTestBefore(void){		
	setWallClock(18,59,59);		//set time 18:59:59
    	setWallClockHook(&wallClock_hook_main);
}

void myMain(void){   

	// myPrintf(0x07,"Here1!!!\n"); 

    doSomeTestBefore();

	createTsk(Tsk0);

	// myPrintf(0x07,"Here2!!!\n"); 
	createTsk(Tsk1);

	// myPrintf(0x07,"Here3!!!\n"); 
	createTsk(Tsk2);

	// myPrintf(0x07,"Here4!!!\n"); 

	initShell();
    memTestCaseInit();

	createTsk(startShell);
	
	TCB* tcb_ptr = nowTsk;
	while(tcb_ptr!=0){
		myPrintf(0x07,"tcb id is %d\n",tcb_ptr->tid);
		tcb_ptr = tcb_ptr->next;
	}

    tskEnd();
}
