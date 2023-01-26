#include "../../include/task.h"
#include "../../include/tick.h"
#include "../../include/myPrintk.h"

// 调度器初始化
void schedulerInit(void) {
    switch (taskctl.sched.type) {
        case FCFS:
            taskctl.sched.schedulerInit = schedulerInitFCFS;
			taskctl.sched.preempt = 0;
            taskctl.sched.nextTsk = nextTskFCFS;
            taskctl.sched.enqueueTsk = enqueueTskFCFS;
            taskctl.sched.dequeueTsk = dequeueTskFCFS;
            taskctl.sched.schedule = scheduleFCFS;
            taskctl.sched.tick_hook = 0;
            break;

        case PRIO:
            taskctl.sched.schedulerInit = schedulerInitPrio;
			taskctl.sched.preempt = 1;
            taskctl.sched.nextTsk = nextTskPrio;
            taskctl.sched.enqueueTsk = enqueueTskPrio;
            taskctl.sched.dequeueTsk = dequeueTskPrio;
            taskctl.sched.schedule = schedulePrio;
            taskctl.sched.tick_hook = 0;
            break;
        case SJF:
            taskctl.sched.schedulerInit = schedulerInitSJF;
			taskctl.sched.preempt = 0;
            taskctl.sched.nextTsk = nextTskSJF;
            taskctl.sched.enqueueTsk = enqueueTskSJF;
            taskctl.sched.dequeueTsk = dequeueTskSJF;
            taskctl.sched.schedule = scheduleSJF;
            taskctl.sched.tick_hook = 0;
            break;
    }
    taskctl.sched.schedulerInit();
}

/* ******************************* FCFS调度器 **************************** */

myTCB * nextTskFCFS(void) {
    return taskQueueFIFONext(taskctl.rdyQueueFIFO);
}

void enqueueTskFCFS(myTCB *tsk) {
    taskQueueFIFOEnqueue(taskctl.rdyQueueFIFO, tsk);
}

void dequeueTskFCFS(int tid) {
	taskQueueFIFODequeue(taskctl.rdyQueueFIFO,tid);
}

void schedulerInitFCFS(void) {
    taskQueueFIFOInit(taskctl.rdyQueueFIFO);
}

void scheduleFCFS(void) {
	taskQueueFIFO * rdyQueueFCFS = taskctl.rdyQueueFIFO;
	while (1) {
		
		myTCB *preTsk = taskctl.nowTsk;
		
		if (taskQueueFIFOEmpty(rdyQueueFCFS)) {
			taskctl.nowTsk = taskctl.idleTsk;
		}
		else{	
			taskctl.nowTsk = taskQueueFIFONext(rdyQueueFCFS);
			myPrintk(0x05,"Tasknow id is %d\n",taskctl.nowTsk->tid);
		}
		if (taskctl.nowTsk == taskctl.idleTsk)
			continue; 		
		taskctl.nowTsk->state = T_RUN;		
		context_switch(&BspContext,taskctl.nowTsk->topOfStack);
	}
}


/******************************** SJF调度器*************************************/

int compare_exec_time(const myTCB *a, const myTCB *b) {

	if (getTskPara(EXEC_TIME, a->para) == getTskPara(EXEC_TIME, b->para)){
		return getTskPara(ARRV_TIME, a->para) - getTskPara(ARRV_TIME, b->para);
	}		
	else{
		return getTskPara(EXEC_TIME, a->para) - getTskPara(EXEC_TIME, b->para);
	}
}

myTCB * nextTskSJF(void) {
    return taskQueuePrioNext(taskctl.rdyQueuePRIO);
}

void enqueueTskSJF(myTCB *tsk) {
    taskQueuePrioEnqueue(taskctl.rdyQueuePRIO, tsk);
}

void dequeueTskSJF(int tid) {
    taskQueuePrioDequeue(taskctl.rdyQueuePRIO,tid);
}
void schedulerInitSJF(void) {
    taskQueuePrioInit(taskctl.rdyQueuePRIO, taskctl.taskNum, compare_exec_time);
}

void scheduleSJF(void) {
	taskQueuePrio * rdyQueueSJF = taskctl.rdyQueuePRIO;
	while (1) {	
		myTCB *preTsk = taskctl.nowTsk;

		if (taskQueuePrioEmpty(rdyQueueSJF)) {
			taskctl.nowTsk = taskctl.idleTsk;
		}
		else{
			taskctl.nowTsk = taskQueuePrioNext(rdyQueueSJF);
			myPrintk(0x05,"Tasknow id is %d\n",taskctl.nowTsk->tid);
		}
			

		if (preTsk == taskctl.idleTsk && taskctl.nowTsk == taskctl.idleTsk)
			continue; 

		taskctl.nowTsk->state = T_RUN;
		context_switch(&BspContext, taskctl.nowTsk->topOfStack);
	}
}


/*********************************Prio调度器**************************************/


int compare_priority(const myTCB *a, const myTCB *b) {
	if (getTskPara(PRIORITY, a->para) == getTskPara(PRIORITY, b->para))
		return getTskPara(ARRV_TIME, a->para) - getTskPara(ARRV_TIME, b->para);
	else
		return getTskPara(PRIORITY, a->para) - getTskPara(PRIORITY, b->para);
}

myTCB * nextTskPrio(void) {
    return taskQueuePrioNext(taskctl.rdyQueuePRIO);
}

void enqueueTskPrio(myTCB *tsk) {
    taskQueuePrioEnqueue(taskctl.rdyQueuePRIO, tsk);
}

void dequeueTskPrio(int tid) {
    taskQueuePrioDequeue(taskctl.rdyQueuePRIO,tid);
}

void schedulerInitPrio(void) {
    taskQueuePrioInit(taskctl.rdyQueuePRIO, taskctl.taskNum, compare_priority);
}

void schedulePrio(void) {
	
	taskQueuePrio* rdyQueuePrio = taskctl.rdyQueuePRIO;

	
	while (1) {
		myTCB *preTsk = taskctl.nowTsk;
		if (taskQueuePrioEmpty(rdyQueuePrio)) {
			taskctl.nowTsk = taskctl.idleTsk;
		}
		else{
			taskctl.nowTsk = taskQueuePrioNext(rdyQueuePrio);
			myPrintk(0x05,"Tasknow id is %d\n",taskctl.nowTsk->tid);
		}
		if (preTsk == taskctl.idleTsk && taskctl.nowTsk == taskctl.idleTsk)
			continue; 
		taskctl.nowTsk->state = T_RUN;	
		context_switch(&BspContext, taskctl.nowTsk->topOfStack);
	}
}