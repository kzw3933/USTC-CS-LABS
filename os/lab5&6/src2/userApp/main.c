#include "../myOS/userInterface.h"   //interface from kernel

#include "shell.h"
#include "memTestCase.h"
#include "schedulerTestCase.h"



void myMain(void){   

    myPrintf(0x08,"**************************************************************\n");
    myPrintf(0x08,"Init Task:myMain() ----- Initing user tasks\n");
    myPrintf(0x08,"**************************************************************\n");

    init_timer();

    taskArrvQueueInit();

    switch (taskctl.sched.type) {
        case FCFS: { // FCFS Test
			initFCFSCases();
            break;
        }
        case PRIO: { // PRIO Test
            initPrioCases();
            break;
        }
        case SJF: { // SJF Test
            initSJFCases();
            break;
        }
    }
	tskEnd();
}
