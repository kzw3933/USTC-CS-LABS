#include "include/uart.h"
#include "include/vga.h"
#include "include/myPrintk.h"

#include "include/interrupt.h"

#include "include/mem.h"

#include "include/task.h"
#include "include/tick.h"

#include "../userApp/userApp.h"


void pressAnyKeyToStart(void){
	myPrintk(0x07, "\nPlease prepare your uart device\n");
	
	myPrintk(0x06,"Please set OS scheduler scheme:\n");
	myPrintk(0x06,"[1] - FCFS scheduler\n");
	myPrintk(0x06,"[2] - PRIO scheduler\n");
	myPrintk(0x06,"[3] - SJF  scheduler\n");
	myPrintk(0x06,"Choice:");

	unsigned char c;

	while(1){
		c = uart_get_char();
		if(c <='3' && c >='1'){
			myPrintk(0x05,"%c\n",c);
			break;
		}
			
	}

	switch(c){
		case '1':
			taskctl.sched.type = FCFS;
			break;
		case '2':
			taskctl.sched.type = PRIO;
			break;
		case '3':
			taskctl.sched.type = SJF;
			break;
		default:
			taskctl.sched.type = FCFS;
			break;
	}

	myPrintk(0x05,"\npress any key to start!\n");

	uart_get_char();
	
}

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

void osStart(void){

	disable_interrupt();

	//Init
	clear_screen();
	myPrintk(0x02,"Initialing the OS...\n");

	init8253();
	init8259A();

	pMemInit();
	tick_hook_init();

	enable_interrupt();

	doSomeTestBefore();

	pressAnyKeyToStart();

	myPrintk(0x02,"Starting the OS...\n");

	TaskCtlInit();

	myPrintk(0x02,"START MULTITASKING......\n");
	startMultiTask();
	myPrintk(0x02,"STOP MULTITASKING.....SHUTDOWN\n");

	while(1);
}





