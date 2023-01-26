#include "../../include/task.h"
#include "../../include/malloc.h"
#include "../../include/tick.h"
#include "../../include/myPrintk.h"
#include "../../include/interrupt.h"

// 任务启动(可依据当前任务控制器采取的调度策略决定是否抢占调度)
void tskStart(int tid){
    taskctl.tcbPool[tid].state = T_READY;
    taskctl.sched.enqueueTsk(&taskctl.tcbPool[tid]);
    if(taskctl.sched.preempt &&taskctl.nowTsk && taskctl.sched.nextTsk()){
        if(taskctl.sched.nextTsk() != taskctl.nowTsk){
            context_switch(&taskctl.nowTsk->topOfStack,BspContext);
        }
    }
}

// 任务结束
void tskEnd(void){
    taskctl.sched.dequeueTsk(taskctl.nowTsk->tid);
    destroyTsk(taskctl.nowTsk->tid);
    myPrintk(0x06,"task end! tid :%d\n",taskctl.nowTsk->tid);
    context_switch(&taskctl.nowTsk->topOfStack,BspContext);
}

// 任务创建
int createTsk(void (*tskBody)(void)){

    myTCB* tcb = taskctl.firstFreeTsk;
    if(tcb == 0)
        return -1;
    taskctl.firstFreeTsk = taskctl.firstFreeTsk->next;
    tcb->next = 0;

    initTskPara(&tcb->para);

    tcb->tskBody = tskBody;
    
    stack_init(&(tcb->topOfStack),tskBody);

    return tcb->tid;
}


// 任务销毁
void destroyTsk(int tid){
    taskctl.tcbPool[tid].state = T_UNALLOC;
    taskctl.tcbPool[tid].runTime = 0;
    taskctl.tcbPool[tid].lastScheduledTime = 0;
    taskctl.tcbPool[tid].para  = 0;
    taskctl.tcbPool[tid].tskBody = 0;
    taskctl.tcbPool[tid].next = taskctl.firstFreeTsk;
    taskctl.firstFreeTsk = &taskctl.tcbPool[tid];
}




// 空闲任务
void tskIdleBody(void) {
    context_switch(&taskctl.nowTsk->topOfStack,BspContext);
}


// 任务管理器初始化
void TaskCtlInit(void){

    myTCB* tcb;

    for(int i=0;i<MaxTaskNum;i++){
        tcb = &taskctl.tcbPool[i];
        tcb->tid = i;
        tcb->state = T_UNALLOC;
        tcb->runTime = 0;
        tcb->lastScheduledTime = 0;
        tcb->para  = 0;
        tcb->tskBody = 0;
        tcb->stack = (unsigned long*)malloc(STACK_SIZE);
        tcb->topOfStack = tcb->stack + STACK_SIZE-1;
        tcb->next =  (i== MaxTaskNum -1)? 0 : &taskctl.tcbPool[i+1];
    } 

    taskctl.rdyQueueFIFO = (taskQueueFIFO*)malloc(sizeof(taskQueueFIFO));
    taskctl.rdyQueuePRIO = (taskQueuePrio*)malloc(sizeof(taskQueuePrio));

    
    taskctl.arrvQueue = 0;
  
    taskctl.nowTsk = 0;

    taskctl.idleTsk = &taskctl.tcbPool[0];
    taskctl.idleTsk->tskBody = tskIdleBody;
    taskctl.idleTsk->state   = T_READY;
    taskctl.idleTsk->next    = 0;
    stack_init(&(taskctl.idleTsk->topOfStack),tskIdleBody);

    taskctl.firstFreeTsk = &taskctl.tcbPool[1];

    schedulerInit();

    //创建并启动初始任务
    int initTid = createTsk(tskInitBody);
    setTskPara(ARRV_TIME, 0, taskctl.tcbPool[initTid].para);
    setTskPara(EXEC_TIME, 0, taskctl.tcbPool[initTid].para); 
    setTskPara(PRIORITY, 0, taskctl.tcbPool[initTid].para);
    tskStart(initTid);
}

// 多任务调度启动
void startMultiTask(void) {
    BspContext = BspContextBase + STACK_SIZE - 1;
    taskctl.sched.schedule();
}

// 任务上下文切换
void context_switch(unsigned long **prevTskStkAddr, unsigned long *nextTskStk) {
    prevTSK_StackPtrAddr = prevTskStkAddr;
    nextTSK_StackPtr = nextTskStk;
    CTX_SW();
}

// 任务栈初始化
void stack_init(unsigned long **stk, void (*task)(void)) {
    *(*stk)-- = (unsigned long)0x08; // CS
    *(*stk)-- = (unsigned long)task; // eip
    // pushf
    *(*stk)-- = (unsigned long)0x0202; // flag registers
    // pusha
    *(*stk)-- = (unsigned long)0xAAAAAAAA; // eax
    *(*stk)-- = (unsigned long)0xCCCCCCCC; // ecx
    *(*stk)-- = (unsigned long)0xDDDDDDDD; // edx
    *(*stk)-- = (unsigned long)0xBBBBBBBB; // ebx
    *(*stk)-- = (unsigned long)0x44444444; // esp
    *(*stk)-- = (unsigned long)0x55555555; // ebp
    *(*stk)-- = (unsigned long)0x66666666; // esi
    **stk     = (unsigned long)0x77777777; // edi
}

