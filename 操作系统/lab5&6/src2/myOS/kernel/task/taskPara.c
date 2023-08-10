#include "../../include/task.h"
#include "../../include/tick.h"
#include "../../include/malloc.h"
#include "../../include/myPrintk.h"

void task_execute(unsigned int wait_time) {
	// 
	while (taskctl.nowTsk->runTime <= wait_time) {
		// myPrintk(0x06,"runtime: %d\n",taskctl.nowTsk->runTime);
	};
}

void showCurrentTskParaInfo_hook(void) {
	if (get_tick_number() % TICK_FREQ != 0)
		return;

	if (taskctl.nowTsk == taskctl.idleTsk) // dont't show idleTsk's info
		return;

	myPrintk(0x7, "*********************************\n");
	myPrintk(0x7, "*  Current task tid : %-2d        *\n", taskctl.nowTsk);
	myPrintk(0x7, "*  Priority         : %-2d        *\n", getTskPara(PRIORITY, taskctl.nowTsk->para));
	myPrintk(0x7, "*  Arrive time      : %-2d        *\n", getTskPara(ARRV_TIME, taskctl.nowTsk->para));
	myPrintk(0x7, "*  Execute time     : %-2d / %2d   *\n", (taskctl.nowTsk->runTime + 50) / TICK_FREQ, getTskPara(EXEC_TIME, taskctl.nowTsk->para));
	myPrintk(0x7, "*********************************\n");
}

void initTskPara(tskPara **para) {
	*para = (tskPara *)malloc(sizeof(tskPara));
	(*para)->arrTime = 0;
	(*para)->exeTime = 0;
	(*para)->priority = 0;
}

void setTskPara(unsigned int option, unsigned int value, tskPara *para) {
	switch (option) {
		case PRIORITY:
			para->priority = value > MAX_PRIORITY ? MAX_PRIORITY : value;
			break;
		case EXEC_TIME:
			para->exeTime = value > MAX_EXEC_TIME ? MAX_EXEC_TIME : value;
			break;
		case ARRV_TIME:
			para->arrTime = value;
			break;
		default:
			break;
	}
}

unsigned int getTskPara(unsigned int option, tskPara *para) {
	switch (option) {
		case PRIORITY:
			return para->priority;
		case EXEC_TIME:
			return para->exeTime;
		case ARRV_TIME:
			return para->arrTime;
		default:
			return 0;
	}
}