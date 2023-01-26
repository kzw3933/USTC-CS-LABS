#include "../include/task.h"
#include "../include/scheduler.h"

#include "../include/myPrintk.h"

extern void CTX_SW(void);

void  tskIdleBody(void){
    while(1){
        schedule();
    }
}


void tskEnqueue(TCB* tcb){
    if(!taskctl.fifoQueue.head)
        taskctl.fifoQueue.head = tcb;
    else 
        taskctl.fifoQueue.tail->next = tcb;
    taskctl.fifoQueue.tail = tcb;
}

void tskDequeue(TCB* tcb){
    taskctl.fifoQueue.head = taskctl.fifoQueue.head->next;
    if(tcb == taskctl.fifoQueue.tail)
        taskctl.fifoQueue.tail = 0;
}

TCB* nextFCFSTsk(void){
    if(!taskctl.fifoQueue.head)
        return taskctl.fifoQueue.idleTsk;
    else 
        return taskctl.fifoQueue.head;
}


void tskStart(TCB *tsk){
    tsk->state = T_READY;
    tskEnqueue(tsk);
}

void tskEnd(void){
    tskDequeue(nowTsk);
    destroyTsk(nowTsk->tid);
    schedule();
}


int createTsk(void (*tskBody)(void)){
    TCB* tcb = firstFreeTsk;
    //if allocate fail return -1
    if(tcb == 0)
        return -1;
    stack_init(&(firstFreeTsk->topOfStack),tskBody);

    // update firstFreeTsk
    firstFreeTsk = firstFreeTsk->next;

    // start new task
    tcb->next = 0;
    tskStart(tcb);
    return tcb->tid;
}

void destroyTsk(int tid){
    taskctl.tcbPool[tid].state = T_UNALLOC;
    taskctl.tcbPool[tid].next = firstFreeTsk;
    firstFreeTsk = &taskctl.tcbPool[tid];
}


void context_switch(unsigned long **prevTskStkAddr, unsigned long *nextTskStk) {
    prevTSK_StackPtrAddr = prevTskStkAddr;
    nextTSK_StackPtr = nextTskStk;
    CTX_SW();
}

void stack_init(unsigned long **stk, void (*task)(void)) {
    *(*stk)-- = (unsigned long)0x08; // CS
    *(*stk)-- = (unsigned long)task; // eip
    // pushf#endif
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



void TaskCtlInit(void){

    // initialize tcbPool
    TCB* tcb;
    for(int i=0;i<MaxTaskNum;i++){
        tcb = &taskctl.tcbPool[i];
        tcb->tid = i;
        tcb->state = T_UNALLOC;
        tcb->next =  (i== MaxTaskNum -1)? 0 : &taskctl.tcbPool[i+1];
        tcb->stack = (int*)malloc(STACK_SIZE);
        tcb->topOfStack = tcb->stack + STACK_SIZE-1;
    } 

    //initialize ready queue
    taskctl.fifoQueue.head = 0;
    taskctl.fifoQueue.tail = 0;

    //create idle task
    idleTsk = &taskctl.tcbPool[0];
    stack_init(&(idleTsk->topOfStack),tskIdleBody);
    taskctl.fifoQueue.idleTsk = idleTsk;

    firstFreeTsk = &taskctl.tcbPool[1];

    //create init task
    createTsk(initTskBody);
}



void startMultiTask(void) {
    BspContext = BspContextBase + STACK_SIZE - 1;
    nowTsk = nextFCFSTsk();
    // myPrintk(0x07,"first tid is %d\n",nowTsk->tid);
    context_switch(&BspContext, nowTsk->topOfStack);
}



















