#include "include/i8253.h"
#include "include/i8259.h"
#include "include/irq.h"
#include "include/uart.h"
#include "include/vga.h"
#include "include/mem.h"
#include "include/myPrintk.h"
#include "include/task.h"
#include "../userApp/userApp.h"

// extern void myMain(void);		//TODO: to be generalized

void pressAnyKeyToStart(void){
	myPrintk(0x5,"Prepare uart device\n");
	myPrintk(0x5,"Then, press any key to start ...\n");

	uart_get_char();
}

void osStart(void){
	pressAnyKeyToStart(); // prepare for uart device
	init8259A();
	init8253();
	enable_interrupt();

	// clear_screen();

	pMemInit();  //after this, we can use kmalloc/kfree and malloc/free

	{
		unsigned long tmp = dPartitionAlloc(pMemHandler,100);
		dPartitionWalkByAddr(pMemHandler);
		dPartitionFree(pMemHandler,tmp);
		dPartitionWalkByAddr(pMemHandler);
	}

	// finished kernel init
	
	// clear_screen();

	// initialize taskctl
	myPrintk(0x02,"MULTITASK INITIALING......\n");
	TaskCtlInit();

	// start multi-task schedule
	myPrintk(0x02,"START MULTITASKING......\n");
	startMultiTask();
	myPrintk(0x02,"STOP MULTITASKING.....SHUTDOWN\n");

	while(1);
}



