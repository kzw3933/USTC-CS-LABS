#include "../../include/task.h"
#include "../../include/tick.h"
#include "../../include/myPrintk.h"

taskQueuePrio arrvQueue;

int compare_arrv_time(const myTCB *a, const myTCB *b) {
    return getTskPara(ARRV_TIME, a->para) - getTskPara(ARRV_TIME, b->para);
}

// 实现任务动态到达
void startArrivedTask_hook(void) {
	if (taskQueuePrioEmpty(taskctl.arrvQueue))
		return;

	myTCB *nextTsk = taskQueuePrioNext(taskctl.arrvQueue);
	
	if (get_time() >= getTskPara(ARRV_TIME, nextTsk->para)) {
		myPrintk(0x02,"Now timer is %d,Tid %d arrive\n",get_time(),nextTsk->tid);
		taskQueuePrioDequeue(taskctl.arrvQueue,nextTsk->tid);
		tskStart(nextTsk->tid);	
	}
	return ;
}
// 任务到达队列初始化
void taskArrvQueueInit(void) {
	taskctl.arrvQueue = &arrvQueue;
	taskQueuePrioInit(taskctl.arrvQueue, 0, compare_arrv_time);
	append2HookList(&tick_hook_list,startArrivedTask_hook);
}

// 任务加入到达队列
void enableTask(int tid) {
	if (taskctl.tcbPool[tid].para->arrTime== 0){
		myPrintk(0x02,"Now timer is %d,Tid %d arrive\n",get_time(),tid);
		tskStart(tid);
	}		
	else{
		taskQueuePrioEnqueue(taskctl.arrvQueue, &taskctl.tcbPool[tid]);
	}	
}